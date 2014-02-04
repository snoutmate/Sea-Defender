#pragma once

#include <GL/glfw.h>

#include "snoutlib/misc.h"
#include "snoutlib/staticmesh.h"

#include "pe_smoke.h"
#include "pe_bubbles.h"

class Boat
{
	static const int m_num_bubble_streams = 5;
	static int m_names_used[3];
	
  StaticMesh **m_model;
  vec2 m_pos;
  string m_name;
  float m_scale;

	float m_pos_shift;
	float m_rot;

	bool m_damaged;
	bool m_sinking;
	float m_hit_pos;

	float m_sinking_start_time;
	float m_sinking_start_pos;
	float m_sinking_start_rot;

	ParticleEffect *m_smoke;	
	ParticleEffect *m_bbl[m_num_bubble_streams];
	float m_bbl_emitpos_x[m_num_bubble_streams];

	string randname(int index);

public:
	OBB m_obb;

  Boat(vec2 pos,int index,float scale=0.41);
	~Boat();

  void draw(void);
	void hit(float x);
  bool is_sinking(void);
  bool is_alive(void);
};
