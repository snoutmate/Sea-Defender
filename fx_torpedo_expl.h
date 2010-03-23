#pragma once

#include "snoutlib/fx.h"
#include "snoutlib/particles.h"

class FX_Torpedo_Explosion : public FX
{
  vec2 m_pos;
  vec2 m_dir; // direction vector
  ParticleEffect *m_torpedo_expl;

  float m_start_time;
  float m_duration;

  bool m_running;
public:
  FX_Torpedo_Explosion(const vec2& pos,const vec2& dir);
  ~FX_Torpedo_Explosion();
  void update(void);
  void draw(void);
  bool finished(void);
  bool running(void);
};
