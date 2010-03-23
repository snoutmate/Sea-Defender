#include "torpedo.h"
#include "resources.h"
#include "game.h"
#include "fx_torpedo_expl.h"

Torpedo::Torpedo(vec2 start_pos,vec2 vector,float velocity) :
	m_start_pos(start_pos),m_pos(m_start_pos),m_start_time(g_timer->now()),
	m_vector(normalized(vector)), m_velocity(velocity),
  m_scale(0.08), m_alive(true), m_obb(AABB(0,0,0,0))
{
	m_angle = acos( dot( m_vector,normalized(vec2(-1,0.0)) ) );
  m_model = &g_resources.mesh_torpedo;
}

void Torpedo::update(float t)
{
	float tdiff = t - m_start_time;

	if (m_alive)
	  m_pos = m_start_pos + m_vector * (tdiff * m_velocity);
}


void Torpedo::draw(void)
{
  glPushMatrix();

  // trailline
	if (m_alive) {
    draw_aa_line(Line(m_pos,m_start_pos),2.0,vec4(0.0,0.0,0.0,1.0));
	}

  glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_ALPHA );
  glEnable(GL_BLEND);
  glColor4f(0.0,0,0,1.0);

	// torpedo
  glTranslatef(m_pos[0],m_pos[1],0.0);
  glScalef(m_scale,m_scale,1.0);
  glRotatef(m_angle * 180.0/M_PI,0,0,1);

  m_obb = OBB((*m_model)->m_aabb); // update boundingbox

	if (m_alive)
	  (*m_model)->draw();

  glDisable(GL_BLEND);

	//m_obb.draw(false);

  glPopMatrix();
}

bool Torpedo::alive(void)
{
	return m_alive;
}

void Torpedo::explode(void)
{
	m_alive = false;

  if (g_current_game)
    g_current_game->add_effect(new FX_Torpedo_Explosion(m_pos,m_vector), 0);
}

// returns position of the front of the torpedo
vec2 Torpedo::get_pos(void)
{
  return m_pos + (m_vector * (0.08/2.0));
}
