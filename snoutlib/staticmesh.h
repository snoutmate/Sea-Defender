#pragma once

#include "misc.h"
#include "svg.h"
#include "tesselate.h"
#include "bbox.h"
#include "gldefs.h"

class StaticMesh
{
  static const unsigned int m_divisor = 256*256;
  static const unsigned int m_segments = 16;
  unsigned int m_vertexcount;

  GLuint m_vbo;
	GLuint m_texture;
  float_ary_t m_vbo_data;

  unsigned int m_data_ptr;
  const unsigned char * m_data;
  unsigned int m_data_size;

  void load(void);
  float get_num(void);
  vec2 get_vec2(void);
  int get_byte(void);
  path_t read_path(void);
  void build_vbo(void);

	void render_to_texture(int t_size);

public:
	AABB m_aabb;

  StaticMesh(const unsigned char* data,unsigned int data_size);
	~StaticMesh();
  void draw(void);
 
};
