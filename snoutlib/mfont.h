#pragma once
//#ifdef WIN32
//#  include <windows.h>
//#endif

#include "gldefs.h"
#include "misc.h"
#include "svg.h"
#include "tesselate.h"
#include "loadingscreen.h"
#include "rtt_fbo.h"
#include "shader.h"
#include "bbox.h"

extern LoadingScreen *g_loadingscreen;

class Glyph
{
public:
  Glyph(float advance,vec2_ary_t verts=vec2_ary_t());
  Glyph(void);

  float m_advance;
  //int m_start_at;
  //int m_cnt;
  //pathset_t m_pathset; // only used when drawing outline
	vec2_ary_t m_verts;
};

class MFont
{
	int m_texture_chars_per_row; 
  float m_texture_step;

  float m_divisor;
  int m_start_char;
  int m_end_char;
  float m_prescale;
  float m_line_sep;
  float m_ascent;
  float m_origin_shift;
  float m_horiz_advance;
  float m_em_scale;

  GLuint m_vbo;
  float_ary_t m_vbo_data;
  std::vector<Glyph> m_glyphs;

  unsigned int m_data_ptr;
  const unsigned char * const &m_data;

  GLuint m_texture;

	bool m_external_draw;

  void read_from_data(void);
  float get_num(void);
  vec2 get_vec2(void);
  int get_byte(void);
  path_t read_path(void);
  void build_vbo(void);
  void start_drawing(void);
  void stop_drawing(void);
  void set_scale(float scale);
  float center_text_pos(const char *text, float scale=1.0,float width=1.6);
  float get_advance(char ch);
  void print_char(char ch);

	void create_texture(void);

public:
  MFont(const unsigned char *data,float prescale=1.0);
  ~MFont();

  void setup_drawing(void);
  void finish_drawing(void);
  float line_sep(void);
  float size_of_text(const char *text,float scale);

  void print_text(const char *text, vec2 const &pos, float scale=1.0,bool centered=false,vec4 const &color=vec4(1.0,1.0,1.0,1.0));
  void print_text_with_shadow(const char *text, vec2 const &pos, float scale=1.0,bool centered=false,vec4 const &color=vec4(1.0,1.0,1.0,1.0));

  AABB bbox(const char* text, float scale,const vec2 &pos,bool centered=false);
};
