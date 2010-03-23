#ifdef WIN32
#  include <windows.h>
#endif
#include <GL/glfw.h>

#include "boat.h"
#include "background.h"
#include "resources.h"
#include "snoutlib/mfont.h"
#include "snoutlib/timer.h"
#include "game.h"
#include "fx_ship_expl.h"


Boat::Boat(vec2 pos,string name,float scale) :
  m_pos(pos),m_name(name),m_scale(scale),
	m_pos_shift(0.0),m_rot(0.0),
	m_damaged(false),m_sinking(false), m_obb(AABB(0,0,0,0))
{
  m_model = &g_resources.mesh_tanker;

	m_smoke = NULL;
	for(int i=0;i<m_num_bubble_streams;++i)
		m_bbl[i] = NULL;
}

Boat::~Boat()
{
	delete m_smoke;
	for(int i=0;i<m_num_bubble_streams;++i)
		delete m_bbl[i];
}

void Boat::hit(float x)
{
  float hit_pos = x - m_pos[0];
  hit_pos = clamp<float>(hit_pos,-0.15,+0.15);

	if (!m_damaged) {
		m_damaged = true;
		m_smoke = new PE_Smoke(vec2(hit_pos,0.0));

    m_hit_pos = hit_pos;

	} else if (!m_sinking) {
		m_sinking = true;
		m_sinking_start_time = g_timer->now();
		m_sinking_start_pos = m_pos_shift;
		m_sinking_start_rot = m_rot;

		for(int i=0;i<m_num_bubble_streams;++i) {
			m_bbl[i] = new PE_Bubbles(vec2(0.0,0.0));
			m_bbl_emitpos_x[i] = ((float)i/m_num_bubble_streams)+0.1-0.5 + (RAND_0_1 - 0.5)*0.175;
		}
	}

  vec2 expl_pos = vec2(m_pos[0]+hit_pos,m_pos[1]+m_pos_shift-0.00);
  g_current_game->add_effect(new FX_Ship_Explosion(expl_pos),0);
}

void Boat::draw(void)
{
	if (m_sinking) {
		float delta_t = g_timer->now() - m_sinking_start_time;
	
		if (delta_t>25.0) // fully sunk
			return;
		
		// slow start
		if (delta_t<1.0) {
			m_pos_shift = m_sinking_start_pos + 0.03*delta_t*delta_t;
			m_rot = m_sinking_start_rot + m_sinking_start_rot*5.0*delta_t*delta_t;
		} else {
			m_pos_shift = m_sinking_start_pos + 0.05*delta_t + 0.03 - 0.05;
			m_rot = m_sinking_start_rot + m_sinking_start_rot*5.0*delta_t;
		}

		if (delta_t>0.5)
			m_smoke->stop_emitting();

	} else {
		m_pos_shift = Sea::sea_func(m_pos[0]-0.05f) * 0.35f;  // pos
		m_rot = Sea::sea_func(m_pos[0]) * 0.2f * 333; // rot
	}

	// draw smoke
	if (m_damaged) {
		m_smoke->set_pos(vec2(m_pos[0]+m_hit_pos,m_pos[1]+m_pos_shift-0.01));
		m_smoke->update();
		m_smoke->draw();
	}

  glPushMatrix();

  glTranslatef(m_pos[0],m_pos[1]+m_pos_shift,0.0);
  glScalef(m_scale,m_scale,1.0f);
  glRotatef(m_rot,0,0,1);

	// draw bubbles
	if (m_sinking) {
		mat4 mt = glGetCurrentMatrix(GL_MODELVIEW_MATRIX);

		for(int i=0;i<m_num_bubble_streams;++i) {
			vec2 pos = vec2(transformPoint(mt,vec3(m_bbl_emitpos_x[i],0.0,0.0)));
			m_bbl[i]->set_pos(pos);
			m_bbl[i]->update();
			m_bbl[i]->draw();
		}
	}

	// update boundingbox
  m_obb = OBB((*m_model)->m_aabb);
	
	// draw model
  glDisable(GL_TEXTURE_2D);
  glColor3f(0,0,0);
  (*m_model)->draw();

  // draw name
  float text_scale = 0.8f;
  float text_pos = 0.4f - g_resources.font->size_of_text(m_name.c_str(),text_scale);
  g_resources.font->print_text(m_name.c_str(),vec2(text_pos,0.0),text_scale,false,vec4(1.0f,1.0f,1.0f,1.0f));

  glPopMatrix();
}

bool Boat::is_sinking(void)
{
  return m_sinking;
}

bool Boat::is_alive(void)
{
  return !m_sinking;
}
