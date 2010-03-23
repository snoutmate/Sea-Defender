#pragma once

#include "misc.h"
#include "bbox.h"

class Procedural
{
public:
  static vec2_ary_t circle_points(float pos_x,float pos_y,float r=0.1,float n=32,float w_x=1,float w_y=1,float arc=1);

  static void draw_circle(float size=0.1,float segments=32);
  static void draw_circle(Circle &circle,float segments=32);

  static vec2_ary_t bezier_cubic_curve(vec2 pa,vec2 pb,vec2 pc,vec2 pd,int segments);
  static vec2_ary_t bezier_quadratic_curve(vec2 pa,vec2 pb,vec2 pc,int segments);

	static GLuint smoke_particle_texture(int size);
	static GLuint bubble_particle_texture(int t_size);
	static GLuint circle_texture(int t_size);
	static GLuint crosshair_texture(int t_size);
	static GLuint cross_texture(int t_size);
	static GLuint missile_mark_texture(int t_size);
  static GLuint seawaves_texture(void);
  static GLuint seawaves_texture_f(void);
  static GLuint line_texture(void);
};
