#include "fx_moving_text.h"
#include "resources.h"
#include "snoutlib/timer.h"
#include "snoutlib/mfont.h"
#include "snoutlib/misc.h"

FX_Moving_Text::FX_Moving_Text(const string &text, const vec2& startpos, const vec2& dir, const vec2& speed) :
  m_text(text),m_startpos(startpos),m_dir(dir),m_speed(speed),m_pos(m_startpos)
{
  m_start_time = g_timer->now();
  m_duration = 1.3;
  m_textsize = 0.5;
  m_textcolor = vec4(0,1,0,0);
  m_running = true;
}

FX_Moving_Text::~FX_Moving_Text()
{}

void FX_Moving_Text::update(void)
{
  if (!m_running) return;

  float fx_time = g_timer->now() - m_start_time;

  if (fx_time > m_duration) {
    m_running = false;
    return;
  }

  m_pos = m_startpos + (normalized(m_dir) * m_speed * fx_time);
  m_textcolor[3] = fade_out(0.3,m_duration,fx_time);
}

void FX_Moving_Text::draw(void)
{
  if (!m_running) return;

	g_resources.font->print_text_with_shadow(m_text.c_str(),m_pos, m_textsize, false, m_textcolor);
}

bool FX_Moving_Text::finished(void) { return !m_running; }
bool FX_Moving_Text::running(void) { return m_running; }
