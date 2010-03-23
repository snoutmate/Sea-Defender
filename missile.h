#pragma once

#include "snoutlib/misc.h"
#include "snoutlib/staticmesh.h"
#include "snoutlib/timer.h"
#include "snoutlib/bbox.h"
#include "snoutlib/particles.h"
#include "fx_missile_expl.h"

class Missile
{
  StaticMesh **m_model;

  vec2 m_start_pos;
  vec2 m_pos;
  float m_start_time;
  vec2 m_dest_pos;
  float m_len;

	vec2 m_vector;
	float m_velocity;
	float m_angle;
	bool m_alive;
	bool m_exploding;

  float m_radius;
  float m_explosion_start;
  float m_explosion_duration;

	void explode(void);
public:
	static float m_scale;

  Circle m_explosion_circle;

	Missile(vec2 start_pos=vec2(0.0,0.0),vec2 dest=vec2(1.0,1.0),float radius=0.1);
	void update(float t=g_timer->now());
	void draw(void);
  void draw_path(void);
  void draw_dest(void);

	bool alive(void);
  bool exploding(void);

};
