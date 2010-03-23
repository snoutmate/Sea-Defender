#pragma once

#include "snoutlib/particles.h"
#include "snoutlib/misc.h"
#include "layout.h"

class PE_Torpedo_Explosion : public ParticleEffect
{
  vec2 m_dir;
  vec2 m_p1,m_p2;

	void update_single_particle(Particle& p,float delta_t,float p_time);
	void emit_single_particle(Particle& p);

public:
	PE_Torpedo_Explosion(vec2 const& pos,vec2 const& dir);
	
};
