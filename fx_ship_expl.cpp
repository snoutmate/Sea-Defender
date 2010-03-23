
#include "fx_ship_expl.h"
#include "pe_smoketrail.h"
#include "snoutlib/timer.h"

FX_Ship_Explosion::FX_Ship_Explosion(const vec2& pos) :
  m_pos(pos),m_num_trails(5)
{
  vec2 gravity = vec2(0.0,0.4);

  for(int i=0;i<m_num_trails;++i)
    m_trails.push_back(new PE_SmokeTrail(m_pos,vec2(rand_range(-0.4, 0.4),rand_range(-0.6,-0.3)),gravity));

  m_start_time = g_timer->now();
  m_duration = 5.0;
  m_running = true;
}

FX_Ship_Explosion::~FX_Ship_Explosion()
{
  for(int i=0;i<m_num_trails;++i)
    delete m_trails[i];
}


void FX_Ship_Explosion::update(void)
{
  if (!m_running) return;
  
  if (g_timer->now() - m_start_time > m_duration) {
    m_running = false;
    return;
  }

  for(int i=0;i<m_num_trails;++i) m_trails[i]->update();
}

void FX_Ship_Explosion::draw(void)
{
  if (!m_running) return;

  for(int i=0;i<m_num_trails;++i) m_trails[i]->draw();
}

bool FX_Ship_Explosion::finished(void) { return !m_running; }
bool FX_Ship_Explosion::running(void) { return m_running; }
