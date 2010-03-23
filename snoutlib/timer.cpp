#include <GL/glfw.h>

#include "timer.h"

Timer::Timer(void) :
  m_time(0.0f), m_speed(1.0f),
  m_last_update_t((float) glfwGetTime()),
  m_pause(false)
{
}

float Timer::get_no_update(void)
{
  return m_time;
}

float Timer::get(void)
{
//  update();
  return m_time;
}
float Timer::now(void) {return get();}

void Timer::update(void)
{
  float t = (float) glfwGetTime();
  if (m_pause) {
		m_last_update_t = t;
		return;
	}
  float step = t - m_last_update_t;
  m_time += step * m_speed;
  m_last_update_t = t;
}

void Timer::set_speed(float speed)
{
  m_speed = speed;
}

void Timer::pause(void) { m_pause = true; }
void Timer::unpause(void) { m_pause = false; }
void Timer::pause_toggle(void) { m_pause = !m_pause; }
bool Timer::is_paused(void) { return m_pause; }


Timer *g_timer;
