
#include "pe_smoketrail.h"
#include "resources.h"
#include "background.h"

extern GLuint g_smoke_texture;

PE_SmokeTrail::PE_SmokeTrail(vec2 const& pos, vec2 const& speed, vec2 const& gravity) :
	ParticleEffect(pos,40.0,120,3.0,&g_resources.texture_smoke),
  m_speed(speed),m_gravity(gravity)
{
}

void PE_SmokeTrail::update_single_particle(Particle& p,float delta_t,float p_time)
{
  if (p_time<0.2)
  	p.cur_size += 0.05 * delta_t;
	else 
  	p.cur_size += 0.015 * delta_t;
  
	p.cur_color[3] = 1.0-(p_time*0.5);

  p.cur_pos += p.cur_speed * delta_t;

	float sea_y = Sea::sea_func(p.cur_pos[0]) + 0.26;
	if (p.cur_pos[1]>sea_y)
    p.alive = false;

}

void PE_SmokeTrail::emit_single_particle(Particle& p)
{
	p.cur_pos = m_pos + vec2(RAND_0_1 * 0.005f,RAND_0_1 * 0.005f);
	p.cur_size = 0.01 + RAND_0_1 * 0.01; 
//  float col = rand_range(0.0,0.4);
  float col = rand_range(0.1,0.4);
	p.cur_color = vec4(col,col,col,0.0);
  p.cur_speed = vec2(-0.03,0.0) + normalized(vec2(rand_range(-1.0,1.0),rand_range(-1.0,1.0))) * 0.01;
}

void PE_SmokeTrail::update_emitter(float delta_t,float p_time)
{
  if (m_speed[0]<0.0)
    m_speed[0] += 0.05 * delta_t; // dampen sideways speed
  else
    m_speed[0] -= 0.05 * delta_t; // dampen sideways speed
  
  m_speed += m_gravity * delta_t; // add gravity

  m_pos += (m_speed * delta_t);
}
