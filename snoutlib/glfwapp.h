#pragma once
#ifdef WIN32
#  include <windows.h>
#endif

#include <GL/glfw.h>
#include <assert.h>

#include "settings.h"

class Glfwapp
{
public:
  GLint m_viewport[4];
  bool m_letterboxed;
  int m_window_w;
  int m_window_h;
  bool m_vsync;
  
  Glfwapp(const char *title,Settings& settings);
  ~Glfwapp();

  void set_viewport(void);
  void set_ortho2D(void);

  bool window_opened(void);
  bool window_closed(void);

  void init_frame(void);
  void end_frame(void);

  void set_vsync(void);
  void reset_vsync(void);
};
