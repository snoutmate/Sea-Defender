#pragma once

#include "snoutlib/fx.h"
#include "snoutlib/particles.h"

class FX_Ship_Explosion : public FX
{
  vec2 m_pos;

  int m_num_trails;

  vector<ParticleEffect*> m_trails;

  float m_start_time;
  float m_duration;

  bool m_running;
public:
  FX_Ship_Explosion(const vec2& pos);
  ~FX_Ship_Explosion();
  void update(void);
  void draw(void);
  bool finished(void);
  bool running(void);
};
