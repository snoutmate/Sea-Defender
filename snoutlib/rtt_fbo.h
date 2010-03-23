#pragma once

#include "misc.h"
#include "rtt.h"

class RTT_FBO : public RTT
{
  bool m_msaa;
  bool m_depth_tex;

  unsigned int m_nbuffers;
  unsigned int m_width;
  unsigned int m_height;

  GLuint m_fbo;
  GLuint m_fbo_msaa;
  GLuint m_msaa_depth;
  GLuint m_msaa_rb;
  GLuint m_depth;
	GLuint *m_tex_ids;

  void bind(unsigned int n=0);
  void unbind(void);
public:

  RTT_FBO(unsigned int nbuffers = 1,unsigned int size_x = 0, unsigned int size_y = 0,
          bool msaa = true, bool depth_tex = false,GLenum textureformat=GL_RGB);
  ~RTT_FBO();

  void start_render(unsigned int num);
  void end_render(void);
  unsigned int get_texid(unsigned int num=0);
};
