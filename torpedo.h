#pragma once

#include "snoutlib/misc.h"
#include "snoutlib/staticmesh.h"
#include "snoutlib/timer.h"

class Torpedo
{
  StaticMesh **m_model;
  vec2 m_start_pos;
  vec2 m_pos;
  float m_start_time;
	vec2 m_vector;
	float m_velocity;
	float m_angle;
	float m_scale;
	bool m_alive;
public:
	OBB m_obb;

	Torpedo(vec2 start_pos=vec2(0.0,0.0),vec2 vector=vec2(1.0,1.0),float velocity=0.1);
	void update(float t=g_timer->now());
	void draw(void);

	bool alive(void);
	void explode(void);
  vec2 get_pos(void);
};
