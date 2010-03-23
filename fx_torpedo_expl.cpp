
#include "fx_torpedo_expl.h"
//#include "pe_bubble_expl.h"
#include "pe_torpedo_expl.h"
#include "snoutlib/timer.h"

FX_Torpedo_Explosion::FX_Torpedo_Explosion(const vec2& pos,const vec2& dir) :
  m_pos(pos),m_dir(dir)
{
  m_torpedo_expl = new PE_Torpedo_Explosion(m_pos,m_dir);
  m_start_time = g_timer->now();
  m_duration = 2.0;
  m_running = true;
}

FX_Torpedo_Explosion::~FX_Torpedo_Explosion()
{
  delete m_torpedo_expl;
}


void FX_Torpedo_Explosion::update(void)
{
  if (!m_running) return;
  
  if (g_timer->now() - m_start_time > m_duration) {
    m_running = false;
    return;
  }

  m_torpedo_expl->update();

}

void FX_Torpedo_Explosion::draw(void)
{
  if (!m_running) return;

  m_torpedo_expl->draw();
  
}

bool FX_Torpedo_Explosion::finished(void)
{
  return !m_running;
}

bool FX_Torpedo_Explosion::running(void)
{
  return m_running;
}
