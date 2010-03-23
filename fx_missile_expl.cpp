
#include "fx_missile_expl.h"
#include "pe_bubble_expl.h"
#include "snoutlib/timer.h"

FX_Missile_Explosion::FX_Missile_Explosion(const vec2& pos) :
  m_pos(pos)
{
  m_bubble_expl = new PE_Bubble_Explosion(m_pos);
  m_start_time = g_timer->now();
  m_duration = 1.0;
  m_running = true;
}

FX_Missile_Explosion::~FX_Missile_Explosion()
{
  delete m_bubble_expl;
}


void FX_Missile_Explosion::update(void)
{
  if (!m_running) return;
  
  if (g_timer->now() - m_start_time > m_duration) {
    m_running = false;
    return;
  }

  m_bubble_expl->update();

}

void FX_Missile_Explosion::draw(void)
{
  if (!m_running) return;

  m_bubble_expl->draw();
  
}

bool FX_Missile_Explosion::finished(void)
{
  return !m_running;
}

bool FX_Missile_Explosion::running(void)
{
  return m_running;
}
