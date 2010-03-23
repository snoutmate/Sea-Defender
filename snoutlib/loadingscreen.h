#pragma once

#include "glfwapp.h"

class LoadingScreen
{
  Glfwapp& m_ctx;
  int m_counter;
  float m_step;

public:
  LoadingScreen(Glfwapp& ctx);
  ~LoadingScreen();

  void draw_progress_indicator(void);
  void update(void);
};
