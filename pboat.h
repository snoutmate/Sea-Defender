#pragma once

#include "snoutlib/misc.h"
#include "snoutlib/staticmesh.h"
#include "missile.h"

class PBoat
{
  StaticMesh **m_model;
  vec2 m_pos;
	float m_scale;
	float m_shift;

	int m_missiles;
public:
	PBoat(vec2 pos=vec2(0.0,0.0),float scale=0.13);
	void draw(void);
	vec2 get_pos(void);

	int missiles_left(void);
	Missile *fire_missile(vec2 dest,float radius);
	void reload_ammo(void);
};

