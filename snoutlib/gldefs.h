#pragma once

#ifdef WIN32
#  include <windows.h>
#endif

#include <GL/glfw.h>

#ifdef MACOSX
#  include <OpenGL/glu.h>
#else 
#  include <GL/glu.h>
#endif

#include "gl-enums.h"

#define GLFUNCDEF APIENTRY *

void load_exts_entry_points(void);

#ifndef MACOSX
typedef char GLchar;
typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
#endif

// FBO
typedef GLboolean (GLFUNCDEF PFNGLISRENDERBUFFEREXTPROC) (GLuint renderbuffer);
typedef void (GLFUNCDEF PFNGLBINDRENDERBUFFEREXTPROC) (GLenum target, GLuint renderbuffer);
typedef void (GLFUNCDEF PFNGLDELETERENDERBUFFERSEXTPROC) (GLsizei n, const GLuint *renderbuffers);
typedef void (GLFUNCDEF PFNGLGENRENDERBUFFERSEXTPROC) (GLsizei n, GLuint *renderbuffers);
typedef void (GLFUNCDEF PFNGLRENDERBUFFERSTORAGEEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLFUNCDEF PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef GLboolean (GLFUNCDEF PFNGLISFRAMEBUFFEREXTPROC) (GLuint framebuffer);
typedef void (GLFUNCDEF PFNGLBINDFRAMEBUFFEREXTPROC) (GLenum target, GLuint framebuffer);
typedef void (GLFUNCDEF PFNGLDELETEFRAMEBUFFERSEXTPROC) (GLsizei n, const GLuint *framebuffers);
typedef void (GLFUNCDEF PFNGLGENFRAMEBUFFERSEXTPROC) (GLsizei n, GLuint *framebuffers);
typedef GLenum (GLFUNCDEF PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) (GLenum target);
typedef void (GLFUNCDEF PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLFUNCDEF PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLFUNCDEF PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLFUNCDEF PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLFUNCDEF PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void (GLFUNCDEF PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
typedef void (GLFUNCDEF PFNGLBLITFRAMEBUFFEREXTPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void (GLFUNCDEF PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
extern PFNGLISRENDERBUFFEREXTPROC                       my_glIsRenderBuffer;
extern PFNGLBINDRENDERBUFFEREXTPROC                     my_glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSEXTPROC                  my_glDeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERSEXTPROC                     my_glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEEXTPROC                  my_glRenderbufferStorage;
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC           my_glGetRenderbufferParameteriv;
extern PFNGLISFRAMEBUFFEREXTPROC                        my_glIsFramebuffer;
extern PFNGLBINDFRAMEBUFFEREXTPROC                      my_glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSEXTPROC                   my_glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSEXTPROC                      my_glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC               my_glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE1DEXTPROC                 my_glFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                 my_glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTURE3DEXTPROC                 my_glFramebufferTexture3D;
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC              my_glFramebufferRenderbuffer;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC  my_glGetFramebufferAttachmentParameteriv;
extern PFNGLGENERATEMIPMAPEXTPROC                       my_glGenerateMipmap;
extern PFNGLBLITFRAMEBUFFEREXTPROC                      my_glBlitFramebuffer;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC       my_glRenderbufferStorageMultisample;

// shaders
typedef void (GLFUNCDEF PFNGLATTACHSHADERPROC) (GLuint, GLuint);
typedef void (GLFUNCDEF PFNGLBINDATTRIBLOCATIONPROC) (GLuint, GLuint, const GLchar *);
typedef void (GLFUNCDEF PFNGLCOMPILESHADERPROC) (GLuint);
typedef GLuint (GLFUNCDEF PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (GLFUNCDEF PFNGLCREATESHADERPROC) (GLenum);
typedef void (GLFUNCDEF PFNGLDELETEPROGRAMPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLDELETESHADERPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLDETACHSHADERPROC) (GLuint, GLuint);
typedef void (GLFUNCDEF PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLGETACTIVEATTRIBPROC) (GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);
typedef void (GLFUNCDEF PFNGLGETACTIVEUNIFORMPROC) (GLuint, GLuint, GLsizei, GLsizei *, GLint *, GLenum *, GLchar *);
typedef void (GLFUNCDEF PFNGLGETATTACHEDSHADERSPROC) (GLuint, GLsizei, GLsizei *, GLuint *);
typedef GLint (GLFUNCDEF PFNGLGETATTRIBLOCATIONPROC) (GLuint, const GLchar *);
typedef void (GLFUNCDEF PFNGLGETPROGRAMIVPROC) (GLuint, GLenum, GLint *);
typedef void (GLFUNCDEF PFNGLGETPROGRAMINFOLOGPROC) (GLuint, GLsizei, GLsizei *, GLchar *);
typedef void (GLFUNCDEF PFNGLGETSHADERIVPROC) (GLuint, GLenum, GLint *);
typedef void (GLFUNCDEF PFNGLGETSHADERINFOLOGPROC) (GLuint, GLsizei, GLsizei *, GLchar *);
typedef void (GLFUNCDEF PFNGLGETSHADERSOURCEPROC) (GLuint, GLsizei, GLsizei *, GLchar *);
typedef GLint (GLFUNCDEF PFNGLGETUNIFORMLOCATIONPROC) (GLuint, const GLchar *);
typedef void (GLFUNCDEF PFNGLGETUNIFORMFVPROC) (GLuint, GLint, GLfloat *);
typedef void (GLFUNCDEF PFNGLGETUNIFORMIVPROC) (GLuint, GLint, GLint *);
typedef void (GLFUNCDEF PFNGLGETVERTEXATTRIBDVPROC) (GLuint, GLenum, GLdouble *);
typedef void (GLFUNCDEF PFNGLGETVERTEXATTRIBFVPROC) (GLuint, GLenum, GLfloat *);
typedef void (GLFUNCDEF PFNGLGETVERTEXATTRIBIVPROC) (GLuint, GLenum, GLint *);
typedef void (GLFUNCDEF PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint, GLenum, GLvoid* *);
typedef GLboolean (GLFUNCDEF PFNGLISPROGRAMPROC) (GLuint);
typedef GLboolean (GLFUNCDEF PFNGLISSHADERPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLLINKPROGRAMPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLSHADERSOURCEPROC) (GLuint, GLsizei, const GLchar* *, const GLint *);
typedef void (GLFUNCDEF PFNGLUSEPROGRAMPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLUNIFORM1FPROC) (GLint, GLfloat);
typedef void (GLFUNCDEF PFNGLUNIFORM2FPROC) (GLint, GLfloat, GLfloat);
typedef void (GLFUNCDEF PFNGLUNIFORM3FPROC) (GLint, GLfloat, GLfloat, GLfloat);
typedef void (GLFUNCDEF PFNGLUNIFORM4FPROC) (GLint, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (GLFUNCDEF PFNGLUNIFORM1IPROC) (GLint, GLint);
typedef void (GLFUNCDEF PFNGLUNIFORM2IPROC) (GLint, GLint, GLint);
typedef void (GLFUNCDEF PFNGLUNIFORM3IPROC) (GLint, GLint, GLint, GLint);
typedef void (GLFUNCDEF PFNGLUNIFORM4IPROC) (GLint, GLint, GLint, GLint, GLint);
typedef void (GLFUNCDEF PFNGLUNIFORM1FVPROC) (GLint, GLsizei, const GLfloat *);
typedef void (GLFUNCDEF PFNGLUNIFORM2FVPROC) (GLint, GLsizei, const GLfloat *);
typedef void (GLFUNCDEF PFNGLUNIFORM3FVPROC) (GLint, GLsizei, const GLfloat *);
typedef void (GLFUNCDEF PFNGLUNIFORM4FVPROC) (GLint, GLsizei, const GLfloat *);
typedef void (GLFUNCDEF PFNGLUNIFORM1IVPROC) (GLint, GLsizei, const GLint *);
typedef void (GLFUNCDEF PFNGLUNIFORM2IVPROC) (GLint, GLsizei, const GLint *);
typedef void (GLFUNCDEF PFNGLUNIFORM3IVPROC) (GLint, GLsizei, const GLint *);
typedef void (GLFUNCDEF PFNGLUNIFORM4IVPROC) (GLint, GLsizei, const GLint *);
typedef void (GLFUNCDEF PFNGLUNIFORMMATRIX2FVPROC) (GLint, GLsizei, GLboolean, const GLfloat *);
typedef void (GLFUNCDEF PFNGLUNIFORMMATRIX3FVPROC) (GLint, GLsizei, GLboolean, const GLfloat *);
typedef void (GLFUNCDEF PFNGLUNIFORMMATRIX4FVPROC) (GLint, GLsizei, GLboolean, const GLfloat *);
typedef void (GLFUNCDEF PFNGLVALIDATEPROGRAMPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB1DPROC) (GLuint, GLdouble);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB1DVPROC) (GLuint, const GLdouble *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB1FPROC) (GLuint, GLfloat);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB1FVPROC) (GLuint, const GLfloat *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB1SPROC) (GLuint, GLshort);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB1SVPROC) (GLuint, const GLshort *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB2DPROC) (GLuint, GLdouble, GLdouble);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB2DVPROC) (GLuint, const GLdouble *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB2FPROC) (GLuint, GLfloat, GLfloat);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB2FVPROC) (GLuint, const GLfloat *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB2SPROC) (GLuint, GLshort, GLshort);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB2SVPROC) (GLuint, const GLshort *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB3DPROC) (GLuint, GLdouble, GLdouble, GLdouble);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB3DVPROC) (GLuint, const GLdouble *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB3FPROC) (GLuint, GLfloat, GLfloat, GLfloat);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB3FVPROC) (GLuint, const GLfloat *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB3SPROC) (GLuint, GLshort, GLshort, GLshort);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB3SVPROC) (GLuint, const GLshort *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4NBVPROC) (GLuint, const GLbyte *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4NIVPROC) (GLuint, const GLint *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4NSVPROC) (GLuint, const GLshort *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4NUBPROC) (GLuint, GLubyte, GLubyte, GLubyte, GLubyte);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4NUBVPROC) (GLuint, const GLubyte *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4NUIVPROC) (GLuint, const GLuint *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4NUSVPROC) (GLuint, const GLushort *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4BVPROC) (GLuint, const GLbyte *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4DPROC) (GLuint, GLdouble, GLdouble, GLdouble, GLdouble);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4DVPROC) (GLuint, const GLdouble *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4FPROC) (GLuint, GLfloat, GLfloat, GLfloat, GLfloat);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4FVPROC) (GLuint, const GLfloat *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4IVPROC) (GLuint, const GLint *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4SPROC) (GLuint, GLshort, GLshort, GLshort, GLshort);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4SVPROC) (GLuint, const GLshort *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4UBVPROC) (GLuint, const GLubyte *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4UIVPROC) (GLuint, const GLuint *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIB4USVPROC) (GLuint, const GLushort *);
typedef void (GLFUNCDEF PFNGLVERTEXATTRIBPOINTERPROC) (GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid *);
extern PFNGLATTACHSHADERPROC	my_glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC	my_glBindAttribLocation;
extern PFNGLCOMPILESHADERPROC	my_glCompileShader;
extern PFNGLCREATEPROGRAMPROC	my_glCreateProgram;
extern PFNGLCREATESHADERPROC	my_glCreateShader;
extern PFNGLDELETEPROGRAMPROC	my_glDeleteProgram;
extern PFNGLDELETESHADERPROC	my_glDeleteShader;
extern PFNGLDETACHSHADERPROC	my_glDetachShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC	my_glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC	my_glEnableVertexAttribArray;
extern PFNGLGETACTIVEATTRIBPROC	my_glGetActiveAttrib;
extern PFNGLGETACTIVEUNIFORMPROC	my_glGetActiveUniform;
extern PFNGLGETATTACHEDSHADERSPROC	my_glGetAttachedShaders;
extern PFNGLGETATTRIBLOCATIONPROC	my_glGetAttribLocation;
extern PFNGLGETPROGRAMIVPROC	my_glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC	my_glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC	my_glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC	my_glGetShaderInfoLog;
extern PFNGLGETSHADERSOURCEPROC	my_glGetShaderSource;
extern PFNGLGETUNIFORMLOCATIONPROC	my_glGetUniformLocation;
extern PFNGLGETUNIFORMFVPROC	my_glGetUniformfv;
extern PFNGLGETUNIFORMIVPROC	my_glGetUniformiv;
extern PFNGLGETVERTEXATTRIBDVPROC	my_glGetVertexAttribdv;
extern PFNGLGETVERTEXATTRIBFVPROC	my_glGetVertexAttribfv;
extern PFNGLGETVERTEXATTRIBIVPROC	my_glGetVertexAttribiv;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC	my_glGetVertexAttribPointerv;
extern PFNGLISPROGRAMPROC	my_glIsProgram;
extern PFNGLISSHADERPROC	my_glIsShader;
extern PFNGLLINKPROGRAMPROC	my_glLinkProgram;
extern PFNGLSHADERSOURCEPROC	my_glShaderSource;
extern PFNGLUSEPROGRAMPROC	my_glUseProgram;
extern PFNGLUNIFORM1FPROC	my_glUniform1f;
extern PFNGLUNIFORM2FPROC	my_glUniform2f;
extern PFNGLUNIFORM3FPROC	my_glUniform3f;
extern PFNGLUNIFORM4FPROC	my_glUniform4f;
extern PFNGLUNIFORM1IPROC	my_glUniform1i;
extern PFNGLUNIFORM2IPROC	my_glUniform2i;
extern PFNGLUNIFORM3IPROC	my_glUniform3i;
extern PFNGLUNIFORM4IPROC	my_glUniform4i;
extern PFNGLUNIFORM1FVPROC	my_glUniform1fv;
extern PFNGLUNIFORM2FVPROC	my_glUniform2fv;
extern PFNGLUNIFORM3FVPROC	my_glUniform3fv;
extern PFNGLUNIFORM4FVPROC	my_glUniform4fv;
extern PFNGLUNIFORM1IVPROC	my_glUniform1iv;
extern PFNGLUNIFORM2IVPROC	my_glUniform2iv;
extern PFNGLUNIFORM3IVPROC	my_glUniform3iv;
extern PFNGLUNIFORM4IVPROC	my_glUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC	my_glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC	my_glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC	my_glUniformMatrix4fv;
extern PFNGLVALIDATEPROGRAMPROC	my_glValidateProgram;
extern PFNGLVERTEXATTRIB1DPROC	my_glVertexAttrib1d;
extern PFNGLVERTEXATTRIB1DVPROC	my_glVertexAttrib1dv;
extern PFNGLVERTEXATTRIB1FPROC	my_glVertexAttrib1f;
extern PFNGLVERTEXATTRIB1FVPROC	my_glVertexAttrib1fv;
extern PFNGLVERTEXATTRIB1SPROC	my_glVertexAttrib1s;
extern PFNGLVERTEXATTRIB1SVPROC	my_glVertexAttrib1sv;
extern PFNGLVERTEXATTRIB2DPROC	my_glVertexAttrib2d;
extern PFNGLVERTEXATTRIB2DVPROC	my_glVertexAttrib2dv;
extern PFNGLVERTEXATTRIB2FPROC	my_glVertexAttrib2f;
extern PFNGLVERTEXATTRIB2FVPROC	my_glVertexAttrib2fv;
extern PFNGLVERTEXATTRIB2SPROC	my_glVertexAttrib2s;
extern PFNGLVERTEXATTRIB2SVPROC	my_glVertexAttrib2sv;
extern PFNGLVERTEXATTRIB3DPROC	my_glVertexAttrib3d;
extern PFNGLVERTEXATTRIB3DVPROC	my_glVertexAttrib3dv;
extern PFNGLVERTEXATTRIB3FPROC	my_glVertexAttrib3f;
extern PFNGLVERTEXATTRIB3FVPROC	my_glVertexAttrib3fv;
extern PFNGLVERTEXATTRIB3SPROC	my_glVertexAttrib3s;
extern PFNGLVERTEXATTRIB3SVPROC	my_glVertexAttrib3sv;
extern PFNGLVERTEXATTRIB4NBVPROC	my_glVertexAttrib4Nbv;
extern PFNGLVERTEXATTRIB4NIVPROC	my_glVertexAttrib4Niv;
extern PFNGLVERTEXATTRIB4NSVPROC	my_glVertexAttrib4Nsv;
extern PFNGLVERTEXATTRIB4NUBPROC	my_glVertexAttrib4Nub;
extern PFNGLVERTEXATTRIB4NUBVPROC	my_glVertexAttrib4Nubv;
extern PFNGLVERTEXATTRIB4NUIVPROC	my_glVertexAttrib4Nuiv;
extern PFNGLVERTEXATTRIB4NUSVPROC	my_glVertexAttrib4Nusv;
extern PFNGLVERTEXATTRIB4BVPROC	my_glVertexAttrib4bv;
extern PFNGLVERTEXATTRIB4DPROC	my_glVertexAttrib4d;
extern PFNGLVERTEXATTRIB4DVPROC	my_glVertexAttrib4dv;
extern PFNGLVERTEXATTRIB4FPROC	my_glVertexAttrib4f;
extern PFNGLVERTEXATTRIB4FVPROC	my_glVertexAttrib4fv;
extern PFNGLVERTEXATTRIB4IVPROC	my_glVertexAttrib4iv;
extern PFNGLVERTEXATTRIB4SPROC	my_glVertexAttrib4s;
extern PFNGLVERTEXATTRIB4SVPROC	my_glVertexAttrib4sv;
extern PFNGLVERTEXATTRIB4UBVPROC	my_glVertexAttrib4ubv;
extern PFNGLVERTEXATTRIB4UIVPROC	my_glVertexAttrib4uiv;
extern PFNGLVERTEXATTRIB4USVPROC	my_glVertexAttrib4usv;
extern PFNGLVERTEXATTRIBPOINTERPROC	my_glVertexAttribPointer;

//VBO
typedef void (GLFUNCDEF PFNGLBINDBUFFERPROC) (GLenum, GLuint);
typedef void (GLFUNCDEF PFNGLDELETEBUFFERSPROC) (GLsizei, const GLuint *);
typedef void (GLFUNCDEF PFNGLGENBUFFERSPROC) (GLsizei, GLuint *);
typedef GLboolean (GLFUNCDEF PFNGLISBUFFERPROC) (GLuint);
typedef void (GLFUNCDEF PFNGLBUFFERDATAPROC) (GLenum, GLsizeiptr, const GLvoid *, GLenum);
typedef void (GLFUNCDEF PFNGLBUFFERSUBDATAPROC) (GLenum, GLintptr, GLsizeiptr, const GLvoid *);
typedef void (GLFUNCDEF PFNGLGETBUFFERSUBDATAPROC) (GLenum, GLintptr, GLsizeiptr, GLvoid *);
typedef GLvoid* (GLFUNCDEF PFNGLMAPBUFFERPROC) (GLenum, GLenum);
typedef GLboolean (GLFUNCDEF PFNGLUNMAPBUFFERPROC) (GLenum);
typedef void (GLFUNCDEF PFNGLGETBUFFERPARAMETERIVPROC) (GLenum, GLenum, GLint *);
typedef void (GLFUNCDEF PFNGLGETBUFFERPOINTERVPROC) (GLenum, GLenum, GLvoid* *);
extern PFNGLBINDBUFFERPROC	my_glBindBuffer;
extern PFNGLDELETEBUFFERSPROC	my_glDeleteBuffers;
extern PFNGLGENBUFFERSPROC	my_glGenBuffers;
extern PFNGLISBUFFERPROC	my_glIsBuffer;
extern PFNGLBUFFERDATAPROC	my_glBufferData;
extern PFNGLBUFFERSUBDATAPROC	my_glBufferSubData;
extern PFNGLGETBUFFERSUBDATAPROC	my_glGetBufferSubData;
extern PFNGLMAPBUFFERPROC	my_glMapBuffer;
extern PFNGLUNMAPBUFFERPROC	my_glUnmapBuffer;
extern PFNGLGETBUFFERPARAMETERIVPROC	my_glGetBufferParameteriv;
extern PFNGLGETBUFFERPOINTERVPROC	my_glGetBufferPointerv;
