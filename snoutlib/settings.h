#pragma once
#include "misc.h"
#include <string>

class Settings
{
public:
  unsigned int m_res_x;
  unsigned int m_res_y;
  bool m_fullscreen;
  unsigned int m_fsaa;
  bool m_vsync;
  unsigned int m_mouse_sensitivity;

  string m_fname;

  Settings();
  ~Settings();

  void read_from_file(const string &filename);
  void save_to_file(void);
};
