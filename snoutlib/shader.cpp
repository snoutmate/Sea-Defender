#include "shader.h"

#include <stdlib.h>
#include "gldefs.h"

Shader::Shader(const char* vstext,const char* fstext) :
  m_vs(0),m_fs(0),m_prog(0)
{
  if (vstext)
    m_vs = my_glCreateShader(GL_VERTEX_SHADER);

  if (fstext)
    m_fs = my_glCreateShader(GL_FRAGMENT_SHADER);

  m_prog = my_glCreateProgram();

  load(vstext,fstext);
}

void Shader::load(const char* vstext, const char* fstext)
{
  GLint tmp=0;
  if (vstext) {
    my_glShaderSource(m_vs, 1, &vstext, NULL);
    my_glCompileShader(m_vs);
    my_glGetShaderiv(m_vs,GL_COMPILE_STATUS,&tmp);
    if (tmp==GL_FALSE) {
      printf("VS compile failed\n");
#ifndef NDEBUG
      my_glGetShaderiv(m_vs,GL_INFO_LOG_LENGTH,&tmp);
      GLchar* log = new GLchar[tmp];
      my_glGetShaderInfoLog(m_vs,tmp,NULL,log);
      printf("VS compile log:\n%s\n",log);
      delete[] log;
#endif
      exit(-1);
    }
    my_glAttachShader(m_prog,m_vs);
  }

  if (fstext) {
    my_glShaderSource(m_fs, 1, &fstext, NULL);
    my_glCompileShader(m_fs);
    my_glGetShaderiv(m_fs,GL_COMPILE_STATUS,&tmp);
    if (tmp==GL_FALSE) {
      printf("FS compile failed\n");
#ifndef NDEBUG
      my_glGetShaderiv(m_fs,GL_INFO_LOG_LENGTH,&tmp);
      GLchar* log = new GLchar[tmp];
      my_glGetShaderInfoLog(m_fs,tmp,NULL,log);
      printf("FS compile log:\n%s\n",log);
      delete[] log;
#endif
      exit(-1);
    }
    my_glAttachShader(m_prog,m_fs);
  }
  
  my_glLinkProgram(m_prog);

  my_glGetProgramiv(m_prog,GL_LINK_STATUS,&tmp);
  if (tmp==GL_FALSE) {
    printf("Shader compile failed\n");
#ifndef NDEBUG
    my_glGetProgramiv(m_prog,GL_INFO_LOG_LENGTH,&tmp);
    GLchar* log = new GLchar[tmp];
    my_glGetProgramInfoLog(m_prog,tmp,NULL,log);
    printf("Shader compile log:\n%s\n",log);
    delete[] log;
#endif
    exit(-1);
  }
}

void Shader::relink(void) { my_glLinkProgram(m_prog); }

void Shader::use(void) { my_glUseProgram(m_prog); }
void Shader::enable(void) { use(); }

void Shader::unuse(void) { my_glUseProgram(0); }
void Shader::disable(void) { unuse(); }


GLint Shader::get_uni_loc(const char* name)
{
  return my_glGetUniformLocation(m_prog,name);
}

GLint Shader::get_prog(void)
{
  return m_prog;  
}
