#pragma once

#include "snoutlib/fx.h"
#include "snoutlib/particles.h"

class FX_Missile_Explosion : public FX
{
  vec2 m_pos;
  ParticleEffect *m_bubble_expl;

  float m_start_time;
  float m_duration;

  bool m_running;
public:
  FX_Missile_Explosion(const vec2& pos);
  ~FX_Missile_Explosion();
  void update(void);
  void draw(void);
  bool finished(void);
  bool running(void);
};
