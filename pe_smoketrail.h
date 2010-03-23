#pragma once

#include "snoutlib/particles.h"
#include "snoutlib/misc.h"
#include "layout.h"

class PE_SmokeTrail : public ParticleEffect
{
  vec2 m_speed;
  vec2 m_gravity;

	void update_single_particle(Particle& p,float delta_t,float p_time);
	void emit_single_particle(Particle& p);

	void update_emitter(float delta_t,float p_time);

public:
	PE_SmokeTrail(vec2 const& pos, vec2 const& speed, vec2 const& gravity);

	
};
