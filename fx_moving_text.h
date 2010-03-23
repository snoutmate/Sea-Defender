#pragma once

#include <string>
#include "snoutlib/fx.h"

using namespace std;


class FX_Moving_Text : public FX
{
  string m_text;
  vec2 m_startpos;
  vec2 m_dir;
  vec2 m_speed;
  float m_textsize;
  vec4 m_textcolor;

  vec2 m_pos;
  
  float m_start_time;
  float m_duration;

  bool m_running;
public:
  FX_Moving_Text(const string& text,const vec2& startpos, const vec2& dir, const vec2& speed);
  ~FX_Moving_Text();
  void update(void);
  void draw(void);
  bool finished(void);
  bool running(void);
};
