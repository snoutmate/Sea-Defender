#pragma once

class Timer
{
  float m_time;
  float m_speed;
  float m_last_update_t;
  bool m_pause;
public:
  Timer(void);
  float get_no_update(void); // get the time that was current at the last call of update()
  float get(void);
  float now(void); // alias for get
  void update(void);
  void set_speed(float speed);
  void pause(void);
  void unpause(void);
  void pause_toggle(void);
	bool is_paused(void);
};

extern Timer *g_timer;

