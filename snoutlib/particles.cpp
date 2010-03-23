
#include "particles.h"
#include "timer.h"
#include "../resources.h"

extern Timer *g_timer;

ParticleEffect::ParticleEffect(vec2 const &pos,float emit_rate, unsigned int num_particles,
                               float particle_ttl,GLuint *texture,unsigned int mep) :
	m_texture(texture), m_pos(pos), m_emit_rate(emit_rate), m_num_particles(num_particles),
  m_particle_ttl(particle_ttl),m_mep(mep),m_emitting(true)
{
	m_particles = new Particle[m_num_particles];	
	m_pstore = new pstore_s[m_num_particles * 4];

	for(unsigned int i=0;i<m_num_particles;++i) {
		m_particles[i].alive = false;

		// prefill constants for corner and texcoord
		m_pstore[i*4+0].c_x = 0.0;	m_pstore[i*4+0].c_y = 0.0;
		m_pstore[i*4+1].c_x = 0.0;	m_pstore[i*4+1].c_y = 1.0;
		m_pstore[i*4+2].c_x = 1.0;	m_pstore[i*4+2].c_y = 1.0;
		m_pstore[i*4+3].c_x = 1.0;	m_pstore[i*4+3].c_y = 0.0;
	}

	m_particles_to_emit = 0.0;
  m_start_time = g_timer->now();
	m_last_update_time = g_timer->now();
  m_lifetime_num_emitted = 0;
}

ParticleEffect::~ParticleEffect()
{
	delete[] m_pstore;
	delete[] m_particles;
}

void ParticleEffect::update_emitter(float delta_t,float pe_time)
{
  
}


void ParticleEffect::update(void)
{
	float cur_t = g_timer->now();
	float delta_t = cur_t - m_last_update_time;
	m_last_update_time = cur_t;

  float pe_time = cur_t - m_start_time;

	update_emitter(delta_t,pe_time);

	for(unsigned int i=0;i<m_num_particles;++i) {
		Particle &p = m_particles[i];

		if (p.alive == false)
			continue;

		// eliminate dead particles
		float p_time = cur_t - p.start_time;
		if (p_time > m_particle_ttl) {
			p.alive = false;
			continue;
		}

		// update live particle
		update_single_particle(p,delta_t,p_time);
	}

	// emit new particles
	m_particles_to_emit += delta_t * m_emit_rate;


	while (m_particles_to_emit>1.0f) {
    if (m_mep!=0 && m_lifetime_num_emitted>=m_mep)
      break;

		m_particles_to_emit -= 1.0f;

		// emit particle
		int pos = -1;
		// find empty space
		for(unsigned int i=0;i<m_num_particles;++i) {
			if (!m_particles[i].alive) {
				pos = i;
				break;
			}
		}

		if (pos==-1) { // free space not available ?
//			printf("particle overflow\n");
			break;
		}
		
		if (!m_emitting)
			continue;
		
		Particle &p = m_particles[pos];

		p.alive = true;
		p.start_time = cur_t;

		emit_single_particle(p);

    m_lifetime_num_emitted++;
	}

}

void ParticleEffect::draw(void)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,*m_texture);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(1.0,1.0,1.0,0.0);

	Shader *sh = g_resources.shader_particles;
	sh->use();

	int ppos=0;
	for(unsigned int i=0;i<m_num_particles;++i) {
		Particle const &p = m_particles[i];
		if (!p.alive)
			continue;

		for(int i=0;i<4;++i) {
			m_pstore[ppos].pos = p.cur_pos;
			m_pstore[ppos].size = p.cur_size;
			m_pstore[ppos].color = p.cur_color;
			ppos++;
		}
	}

	// TODO: VBO, shaders
  // TODO: 32-byte alignment
  my_glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(pstore_s),&m_pstore[0].pos.data);
  my_glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(pstore_s),&m_pstore[0].c_x);
  my_glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,sizeof(pstore_s),&m_pstore[0].size);
  my_glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,sizeof(pstore_s),&m_pstore[0].color.data);

	for(int i=0;i<4;++i) my_glEnableVertexAttribArray(i);

	glDrawArrays(GL_QUADS,0,ppos);

	for(int i=0;i<4;++i) my_glDisableVertexAttribArray(i);

	sh->unuse();

	glDisable(GL_TEXTURE_2D);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_BLEND);
}

void ParticleEffect::stop_emitting(void)
{
	m_emitting = false;
}

void ParticleEffect::start_emitting(void)
{
  m_emitting = true;
}

unsigned int ParticleEffect::num_live_particles(void)
{
	unsigned int live_particles = 0;
	for(unsigned int i=0;i<m_num_particles;++i)
		if (m_particles[i].alive)
			++live_particles;

	return live_particles;
}

void ParticleEffect::set_pos(vec2 const& pos)
{
	m_pos = pos;
}
