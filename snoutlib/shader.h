#pragma once

#include "misc.h"

class Shader
{
  GLuint m_vs,m_fs,m_prog;
  void load(const char* vstext, const char* fstext);
public:
  Shader(const char* vstext,const char* fstext);
  void use(void);
  void enable(void); // alias for use
  void unuse(void);
  void disable(void);
	void relink(void);
  GLint get_uni_loc(const char* name);
  GLint get_prog(void);

};
