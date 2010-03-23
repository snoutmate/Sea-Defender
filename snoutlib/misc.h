#pragma once

#ifdef WIN32
#  include <windows.h>
#endif

#include <GL/glfw.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <algorithm>
//#include <GL/gl.h>
  
#include "../math3d++/math3d++.h"
#include <vector>
#include <list>
#include <sstream>
#include <map>

using namespace math3d;

using namespace std;

typedef pair<string,string> str_pair_t;
typedef vector<str_pair_t> vec_str_pair_t;
typedef vector<vec2> vec2_ary_t;
typedef vector<double> double_ary_t;
typedef vector<float> float_ary_t;
typedef vector<string> str_ary_t;
typedef map<string,string> map_str_str_t;

double_ary_t vec2_ary_to_double_ary(vec2_ary_t v2ary);
float_ary_t vec2_ary_to_float_ary(vec2_ary_t v2ary);

extern float g_max_x;
extern float g_max_y;

#define FOR_EACH_CONST(_type_,_container_,_name_) \
for(_type_::const_iterator _name_ = _container_.begin(),_name_##_end=_container_.end();_name_!=_name_##_end;++_name_) 

#define FOR_EACH(_type_,_container_,_name_) \
for(_type_::iterator _name_ = _container_.begin(),_name_##_end=_container_.end();_name_!=_name_##_end;++_name_) 

// constrain time to repeated 0..c
float get_time_con(float c);
float get_time_con_time(float t, float c);

void save_texture_as_ppm(GLuint texid,const char *fname);
void draw_textured_quad(float size, GLuint texture,bool blend=false);

struct Line;
void draw_aa_line(Line l,float width = 2.0,vec4 color = vec4(1.0,1.0,1.0,1.0),float stipple=0.0);

GLuint move_texture_red_component(GLuint texid);

template <class T>
string to_string (const T& t)
{
  stringstream ss;
  ss << t;
  return ss.str();
}

template <class T>
T from_string (const string& s)
{
  T t;
  stringstream ss(s);
  ss >> t;
  return t;
}

template <class T>
T clamp(const T& val, const T& min,const T& max)
{
  if(val > max)
    return max;
  else if(val < min)
    return min;

  return val;
}

static inline float clamp_0_1(float val)
{
  return clamp<float>(val,0.0,1.0);
}

#define RAND_0_1 ((float)rand()/RAND_MAX)
#define RAND_1_1 rand_range(-1.0,1.0)

#define LERP(t, a, b) ( a + t * (b - a) )
#define INV_LERP(t, a, b) ( (t - a) / (b - a) )

static inline float rand_range(float min,float max)
{
  return LERP(RAND_0_1,min,max);
}

static inline vec2 lerp(float t,const vec2& a,const vec2& b)
{
  return vec2(
    LERP(t,a[0],b[0]),
    LERP(t,a[1],b[1])
    );
}

static inline float fade_in(float start_at,float full_at,float t)
{ return clamp<float>(INV_LERP(t,start_at,full_at),0.0,1.0); }

static inline float fade_out(float start_at,float clear_at,float t)
{ return 1.0 - fade_in(start_at,clear_at,t); }

static inline float fade_in_out(float in_start,float in_end, float out_start, float out_end, float t)
{
  if (t<=in_end)
    return fade_in(in_start,in_end,t);
  else 
    return fade_out(out_start,out_end,t);
}


// 
class FPScounter
{
  float m_interval;
  unsigned int m_frames;
  float m_old_t;
  float m_fps;
public:
  FPScounter(float interval=1.0);
  
  float next_frame(void);
};

class Gl
{
public:
  // supported OGL version 
  static bool is_available(int major,int minor)
  {
    int glver[3] = {0};
    glfwGetGLVersion(&glver[0],&glver[1],&glver[2]);

    if (glver[0]>major || (glver[0]==major && glver[1] >=minor))
      return true;
    else
      return false;
  }
  
  // check for extension support
  static bool is_available(const char *extension)
  {
    return (glfwExtensionSupported(extension) == GL_TRUE ? true : false);
  }
 
};

class Mouse
{
  float m_x,m_y;
  int m_last_pos_x,m_last_pos_y;
  float m_accel;
  
public:  
  Mouse() : m_x(g_max_x/2.0), m_y(g_max_y/2.0), m_last_pos_x(0),m_last_pos_y(0)
  {
    glfwSetMousePos(0,0);
    m_accel = 0.001; // 0.01 too fast, 0.001 normal , 0.0001 too slow, 
  }

  void set_accel(int a) // 1..10
  {
    float r = ((10.0-a+1.0) / 10.0) * 2.0;
    float d = pow((float)10.0,r);
    m_accel = 0.01 / d;
  }

  void update(void)
  {
    int pos_x,pos_y;
    glfwGetMousePos(&pos_x,&pos_y);

    float delta_x = (float)(m_last_pos_x - pos_x) * m_accel;
    float delta_y = (float)(m_last_pos_y - pos_y) * m_accel;

//    printf("x: %i -> %i, y: %i -> %i\n",m_last_pos_x,pos_x,m_last_pos_y,pos_y);

    m_last_pos_x = pos_x;
    m_last_pos_y = pos_y;
  
    m_x -= delta_x;
    m_y -= delta_y;

    if (m_x<0.0) m_x = 0.0;
    if (m_x>g_max_x) m_x = g_max_x;
    if (m_y<0.0) m_y = 0.0;
    if (m_y>g_max_y) m_y = g_max_y;
  }

  void draw(GLuint texture)
  {
    float crosshair_size = 0.04;
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(m_x,m_y,0.0);

    float t = get_time_con_time(glfwGetTime(), 4.0);
    float angle = t/4.0 * 360.0;
    glRotatef(angle,0,0,1);
    glColor4f(1.0,1.0,1.0,1.0);
    draw_textured_quad(crosshair_size, texture, true);
    
    glPopMatrix();
  }

  vec2 get_pos(void)
  {
    return vec2(m_x,m_y);
  }
};
