
#include "pe_torpedo_expl.h"
#include "resources.h"

extern Layouts *g_layouts;
extern GLuint g_smoke_texture;

PE_Torpedo_Explosion::PE_Torpedo_Explosion(vec2 const& pos,vec2 const &dir) :
	ParticleEffect(pos,1000.0,80.0,2.0,&g_resources.texture_smoke,80.0),
  m_dir(dir)
{
  // two points denoting start and end torpedo in worldspace coords
  m_p1 = pos - normalized(m_dir)*0.04;
  m_p2 = pos + normalized(m_dir)*0.04;
}

void PE_Torpedo_Explosion::update_single_particle(Particle& p,float delta_t,float p_time)
{
	// update pos
  float norm_ptime = (p_time/1.8);
  float inv_norm_ptime = 1.0- norm_ptime;
  float color_f = inv_norm_ptime * inv_norm_ptime;

  if (color_f < 0.02)
    p.alive = false;

	p.cur_color[3] = color_f;

	p.cur_pos += p.cur_speed * inv_norm_ptime * delta_t;

	p.cur_size = 0.02 + (norm_ptime * norm_ptime)*0.125;
  if (p.cur_size>0.05)
    p.cur_size = 0.05;

}

void PE_Torpedo_Explosion::emit_single_particle(Particle& p)
{
  p.cur_pos = lerp(RAND_0_1,m_p1,m_p2) + vec2(RAND_0_1 * 0.002f, RAND_0_1 * 0.002f);
	p.cur_speed = normalized(vec2(RAND_1_1,RAND_1_1))*rand_range(0.01,0.1);
	p.cur_size = 0.0;
  float col = rand_range(0.0,0.1);
	p.cur_color = vec4(col,col,col,1);
}
