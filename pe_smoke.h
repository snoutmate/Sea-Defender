#pragma once

#include "snoutlib/particles.h"
#include "snoutlib/misc.h"
#include "layout.h"

class PE_Smoke : public ParticleEffect
{
	void update_single_particle(Particle& p,float delta_t,float p_time);
	void emit_single_particle(Particle& p);

public:
	PE_Smoke(vec2 const& pos);
	
};
