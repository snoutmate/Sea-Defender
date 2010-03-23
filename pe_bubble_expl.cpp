#include "pe_bubble_expl.h"
#include "resources.h"
#include "background.h"

extern Layouts *g_layouts;

PE_Bubble_Explosion::PE_Bubble_Explosion(vec2 const& pos) :
	ParticleEffect(pos,2000.0,300,3.0,&g_resources.texture_cross,300)
{
}

void PE_Bubble_Explosion::update_single_particle(Particle& p,float delta_t,float p_time)
{
	p.cur_pos += (p.cur_vector * p.cur_speed *((3.0-p_time))) * delta_t;

	p.cur_size += 0.01 * delta_t;

  if (p.cur_speed[0]>0.01)
    p.cur_speed[0] -= 0.05 * delta_t;

  if (p.cur_speed[1]>0.01)
    p.cur_speed[1] -= 0.05 * delta_t;

	p.cur_color[3] = 1.1-p_time*2.0;

	if (p.cur_color[3]<0.01)
		p.alive = false;
}

void PE_Bubble_Explosion::emit_single_particle(Particle& p)
{
	p.cur_pos = m_pos + vec2(RAND_0_1 * 0.005f,RAND_0_1 * 0.005f);
	p.cur_speed = vec2(0.01+RAND_0_1*0.04,0.01+RAND_0_1*0.04);
	p.cur_vector = normalized(vec2((RAND_0_1*2.0-1.0),(RAND_0_1*2.0-1.0)));

//	p.cur_size = 0.001;
	p.cur_size = 0.004;
//	p.cur_color = vec4(0.0,0.0,0.0,1.0);
	p.cur_color = vec4(1.0,1.0,1.0,1.0);
}
