
#include "pe_bubbles.h"
#include "resources.h"
#include "background.h"

extern Layouts *g_layouts;

PE_Bubbles::PE_Bubbles(vec2 const& pos) :
	ParticleEffect(pos,50.0,150,3.0,&g_resources.texture_bubble)
{
}

void PE_Bubbles::update_single_particle(Particle& p,float delta_t,float p_time)
{
	// add slight random deviation
	p.cur_pos += (p.cur_speed + (RAND_0_1-0.5)*0.1) * delta_t;

	// horizontal movement to sine wave, shifted by preset random factor in user data
	p.cur_pos[0] += sin(p_time*10+p.user_data[0]*10)*0.06 * delta_t;

	// increase the size with time
	p.cur_size += 0.005 * delta_t;

	// increase upwards speed with time
	p.cur_speed[1] = -p_time*0.1;

	// dampen alpha over time
	if (p.cur_color[3]>0.0)
		p.cur_color[3] -= 0.5 * delta_t;

	if (p.cur_color[3]<=0.0)
		p.alive = false;

	// disable particles above sea level
	float sea_y = Sea::sea_func(p.cur_pos[0]) + 0.263;
	if (p.cur_pos[1]<sea_y)
		p.alive = false;
}

void PE_Bubbles::emit_single_particle(Particle& p)
{
	p.cur_pos = m_pos + vec2(RAND_0_1 * 0.01f,RAND_0_1 * 0.01f);
	p.cur_speed = vec2((-0.01+RAND_0_1*0.02),-0.1);
//	p.cur_size = 0.005;
	p.cur_size = 0.003;
	p.cur_color = vec4(1.0,1.0,1.0,1.0);
	p.user_data = vec4(RAND_0_1,0,0,0);
}

