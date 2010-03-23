
#include "pe_smoke.h"
#include "resources.h"

//extern Timer *g_timer;
extern Layouts *g_layouts;
extern GLuint g_smoke_texture;

PE_Smoke::PE_Smoke(vec2 const& pos) :
	ParticleEffect(pos,100.0,500,5.0,&g_resources.texture_smoke)
{
}

void PE_Smoke::update_single_particle(Particle& p,float delta_t,float p_time)
{
	// update pos
	p.cur_pos += p.cur_speed * delta_t;

	// wind pulls to the left
	p.cur_speed[0] -= 0.02 * delta_t;

	// dampen upwards speed (0,0 is left upper corner so -up,+down)
	p.cur_speed[1] += 0.02 * delta_t;
	if (p.cur_speed[1]>-0.04) // clamp
		p.cur_speed[1]=-0.04;

	// increase size with time
	p.cur_size += 0.015 * delta_t;

	// color fade to black (fire->smoke), constant alpha
	p.cur_color -= 2.0 * delta_t;
	p.cur_color[3] = 1.0;

	if ((p.cur_pos[1] + p.cur_size/2.0)<0.0)
		p.alive = false;
}

void PE_Smoke::emit_single_particle(Particle& p)
{
	p.cur_pos = m_pos + vec2(RAND_0_1 * 0.02f,RAND_0_1 * 0.01f);
	p.cur_speed = vec2((-0.02+RAND_0_1*0.04),-0.1);
	p.cur_size = 0.01;
	p.cur_color = g_layouts->cur()->color_fire_start;
}
