#pragma once

#include <GL/glfw.h>

#include "snoutlib/misc.h"
#include "snoutlib/shader.h"
#include "snoutlib/rtt_fbo.h"

class Clouds
{
  unsigned int m_num_clouds;
  unsigned int m_num_tex_row;
  float m_time_offset;
  GLuint m_vbo;
  GLuint m_cloud_texture;
  GLuint m_circle_dl;

  void generate_cloud_textures(void);
  void draw_random_cloud(void);

  Clouds(Clouds &b2) {}
  Clouds &operator=(const Clouds &b2) { return *this;}

public:
  Clouds();
  ~Clouds();
  void draw(void);
};

class Sky
{
  Clouds *m_clouds;

  Sky(Sky &b2) {}
  Sky &operator=(const Sky &b2) { return *this;}
public:
  Sky();
  ~Sky();
  void draw(float pos);
};

class Sea
{
  GLuint m_vbo;
  GLuint m_vertcnt;

  Sea(Sea &b2) {}
  Sea &operator=(const Sea &b2) { return *this;}
public:
  Sea();
  static float sea_func(float x);
  void draw(float pos,float max_amp, bool blend);
 
};

class Background
{
  float m_sea_pos;
  float m_sea_amp;
  Sea *m_sea;
  Sky *m_sky;

  Background(Background &b2) {}
  Background &operator=(const Background &b2) { return *this;}
public:
  Background();
  ~Background();
  void draw(void);
  void draw_sea(bool blend);
  void draw_sky(void);
  static void darken_area(vec4 const &box,float amount=0.4f);
};

