
#include "missile.h"

#include "resources.h"

#include "game.h"


float Missile::m_scale = 0.04;

Missile::Missile(vec2 start_pos,vec2 dest,float radius) :
  m_start_pos(start_pos),m_pos(m_start_pos),m_start_time(g_timer->now()),
  m_dest_pos(dest), m_alive(true), m_exploding(false), m_radius(radius),
  m_explosion_start(0.0),
  m_explosion_circle(Circle(vec2(0.0,0.0),0.0))
  
{
  m_velocity = 0.25;
  m_vector = normalized(dest - start_pos);
  m_angle = acos( dot( normalized(vec2(+1.0,0.0)),m_vector ) );
  m_model = &g_resources.mesh_missile;
  m_angle += M_PI;
  m_explosion_duration = 0.4;
  m_len = length(m_dest_pos - m_start_pos);
}

void Missile::update(float t)
{
  float tdiff = t - m_start_time;

  if (m_alive) {
    m_pos = m_start_pos + m_vector * (tdiff * m_velocity);
    float len = length(m_pos - m_start_pos);
    if (len>=m_len) {
      explode();
    }
  }

  if (m_exploding) {
    float explosion_time_0_1 = (g_timer->now() - m_explosion_start) / m_explosion_duration;

    m_explosion_circle = Circle(m_dest_pos, m_radius * explosion_time_0_1);
    
    if (g_timer->now() > (m_explosion_start + m_explosion_duration)) {
      m_exploding = false;
    }
  }
}

void Missile::draw(void)
{
  if (!m_alive) return;
  
  glPushMatrix();

  glColor4f(0,0,0,1.0);
  glBlendFunc(GL_ZERO,GL_ONE_MINUS_SRC_ALPHA );
  glEnable(GL_BLEND);

  // missile
  glTranslatef(m_pos[0],m_pos[1],0.0);
  glScalef(m_scale,m_scale,1.0);
  glRotatef(m_angle * 180.0/M_PI,0,0,1);

  (*m_model)->draw();

  glDisable(GL_BLEND);

  glPopMatrix();
}

void Missile::draw_path(void)
{
  if (!m_alive) return;

  draw_aa_line(Line(m_dest_pos,m_pos),2.0,vec4(1.0,1.0,1.0,1.0),16.0);
}

void Missile::draw_dest(void)
{
  if (!m_alive) return;

  glLoadIdentity();
  glColor4f(1,1,1,1);
  glTranslatef(m_dest_pos[0],m_dest_pos[1],0.0);
  draw_textured_quad(m_radius*2.01, g_resources.texture_missile_mark,true);
  glLoadIdentity();
}

bool Missile::alive(void)
{
  return m_alive;
}

bool Missile::exploding(void)
{
  return m_exploding;
}

void Missile::explode(void)
{
  m_exploding = true;
  m_alive = false;
  m_explosion_start = g_timer->now();

  if (g_current_game)
    g_current_game->add_effect(new FX_Missile_Explosion(m_pos), 0);
}

