#include "gldefs.h"

// FBO 
PFNGLISRENDERBUFFEREXTPROC                       my_glIsRenderBuffer;
PFNGLBINDRENDERBUFFEREXTPROC                     my_glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSEXTPROC                  my_glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSEXTPROC                     my_glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEEXTPROC                  my_glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC           my_glGetRenderbufferParameteriv;
PFNGLISFRAMEBUFFEREXTPROC                        my_glIsFramebuffer;
PFNGLBINDFRAMEBUFFEREXTPROC                      my_glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSEXTPROC                   my_glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSEXTPROC                      my_glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC               my_glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC                 my_glFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                 my_glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC                 my_glFramebufferTexture3D;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC              my_glFramebufferRenderbuffer;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC  my_glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPEXTPROC                       my_glGenerateMipmap;
PFNGLBLITFRAMEBUFFEREXTPROC                      my_glBlitFramebuffer;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC       my_glRenderbufferStorageMultisample;

// shaders

PFNGLATTACHSHADERPROC	my_glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC	my_glBindAttribLocation;
PFNGLCOMPILESHADERPROC	my_glCompileShader;
PFNGLCREATEPROGRAMPROC	my_glCreateProgram;
PFNGLCREATESHADERPROC	my_glCreateShader;
PFNGLDELETEPROGRAMPROC	my_glDeleteProgram;
PFNGLDELETESHADERPROC	my_glDeleteShader;
PFNGLDETACHSHADERPROC	my_glDetachShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC	my_glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC	my_glEnableVertexAttribArray;
PFNGLGETACTIVEATTRIBPROC	my_glGetActiveAttrib;
PFNGLGETACTIVEUNIFORMPROC	my_glGetActiveUniform;
PFNGLGETATTACHEDSHADERSPROC	my_glGetAttachedShaders;
PFNGLGETATTRIBLOCATIONPROC	my_glGetAttribLocation;
PFNGLGETPROGRAMIVPROC	my_glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC	my_glGetProgramInfoLog;
PFNGLGETSHADERIVPROC	my_glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC	my_glGetShaderInfoLog;
PFNGLGETSHADERSOURCEPROC	my_glGetShaderSource;
PFNGLGETUNIFORMLOCATIONPROC	my_glGetUniformLocation;
PFNGLGETUNIFORMFVPROC	my_glGetUniformfv;
PFNGLGETUNIFORMIVPROC	my_glGetUniformiv;
PFNGLGETVERTEXATTRIBDVPROC	my_glGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC	my_glGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC	my_glGetVertexAttribiv;
PFNGLGETVERTEXATTRIBPOINTERVPROC	my_glGetVertexAttribPointerv;
PFNGLISPROGRAMPROC	my_glIsProgram;
PFNGLISSHADERPROC	my_glIsShader;
PFNGLLINKPROGRAMPROC	my_glLinkProgram;
PFNGLSHADERSOURCEPROC	my_glShaderSource;
PFNGLUSEPROGRAMPROC	my_glUseProgram;
PFNGLUNIFORM1FPROC	my_glUniform1f;
PFNGLUNIFORM2FPROC	my_glUniform2f;
PFNGLUNIFORM3FPROC	my_glUniform3f;
PFNGLUNIFORM4FPROC	my_glUniform4f;
PFNGLUNIFORM1IPROC	my_glUniform1i;
PFNGLUNIFORM2IPROC	my_glUniform2i;
PFNGLUNIFORM3IPROC	my_glUniform3i;
PFNGLUNIFORM4IPROC	my_glUniform4i;
PFNGLUNIFORM1FVPROC	my_glUniform1fv;
PFNGLUNIFORM2FVPROC	my_glUniform2fv;
PFNGLUNIFORM3FVPROC	my_glUniform3fv;
PFNGLUNIFORM4FVPROC	my_glUniform4fv;
PFNGLUNIFORM1IVPROC	my_glUniform1iv;
PFNGLUNIFORM2IVPROC	my_glUniform2iv;
PFNGLUNIFORM3IVPROC	my_glUniform3iv;
PFNGLUNIFORM4IVPROC	my_glUniform4iv;
PFNGLUNIFORMMATRIX2FVPROC	my_glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC	my_glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC	my_glUniformMatrix4fv;
PFNGLVALIDATEPROGRAMPROC	my_glValidateProgram;
PFNGLVERTEXATTRIB1DPROC	my_glVertexAttrib1d;
PFNGLVERTEXATTRIB1DVPROC	my_glVertexAttrib1dv;
PFNGLVERTEXATTRIB1FPROC	my_glVertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC	my_glVertexAttrib1fv;
PFNGLVERTEXATTRIB1SPROC	my_glVertexAttrib1s;
PFNGLVERTEXATTRIB1SVPROC	my_glVertexAttrib1sv;
PFNGLVERTEXATTRIB2DPROC	my_glVertexAttrib2d;
PFNGLVERTEXATTRIB2DVPROC	my_glVertexAttrib2dv;
PFNGLVERTEXATTRIB2FPROC	my_glVertexAttrib2f;
PFNGLVERTEXATTRIB2FVPROC	my_glVertexAttrib2fv;
PFNGLVERTEXATTRIB2SPROC	my_glVertexAttrib2s;
PFNGLVERTEXATTRIB2SVPROC	my_glVertexAttrib2sv;
PFNGLVERTEXATTRIB3DPROC	my_glVertexAttrib3d;
PFNGLVERTEXATTRIB3DVPROC	my_glVertexAttrib3dv;
PFNGLVERTEXATTRIB3FPROC	my_glVertexAttrib3f;
PFNGLVERTEXATTRIB3FVPROC	my_glVertexAttrib3fv;
PFNGLVERTEXATTRIB3SPROC	my_glVertexAttrib3s;
PFNGLVERTEXATTRIB3SVPROC	my_glVertexAttrib3sv;
PFNGLVERTEXATTRIB4NBVPROC	my_glVertexAttrib4Nbv;
PFNGLVERTEXATTRIB4NIVPROC	my_glVertexAttrib4Niv;
PFNGLVERTEXATTRIB4NSVPROC	my_glVertexAttrib4Nsv;
PFNGLVERTEXATTRIB4NUBPROC	my_glVertexAttrib4Nub;
PFNGLVERTEXATTRIB4NUBVPROC	my_glVertexAttrib4Nubv;
PFNGLVERTEXATTRIB4NUIVPROC	my_glVertexAttrib4Nuiv;
PFNGLVERTEXATTRIB4NUSVPROC	my_glVertexAttrib4Nusv;
PFNGLVERTEXATTRIB4BVPROC	my_glVertexAttrib4bv;
PFNGLVERTEXATTRIB4DPROC	my_glVertexAttrib4d;
PFNGLVERTEXATTRIB4DVPROC	my_glVertexAttrib4dv;
PFNGLVERTEXATTRIB4FPROC	my_glVertexAttrib4f;
PFNGLVERTEXATTRIB4FVPROC	my_glVertexAttrib4fv;
PFNGLVERTEXATTRIB4IVPROC	my_glVertexAttrib4iv;
PFNGLVERTEXATTRIB4SPROC	my_glVertexAttrib4s;
PFNGLVERTEXATTRIB4SVPROC	my_glVertexAttrib4sv;
PFNGLVERTEXATTRIB4UBVPROC	my_glVertexAttrib4ubv;
PFNGLVERTEXATTRIB4UIVPROC	my_glVertexAttrib4uiv;
PFNGLVERTEXATTRIB4USVPROC	my_glVertexAttrib4usv;
PFNGLVERTEXATTRIBPOINTERPROC	my_glVertexAttribPointer;

// VBO
PFNGLBINDBUFFERPROC	my_glBindBuffer;
PFNGLDELETEBUFFERSPROC	my_glDeleteBuffers;
PFNGLGENBUFFERSPROC	my_glGenBuffers;
PFNGLISBUFFERPROC	my_glIsBuffer;
PFNGLBUFFERDATAPROC	my_glBufferData;
PFNGLBUFFERSUBDATAPROC	my_glBufferSubData;
PFNGLGETBUFFERSUBDATAPROC	my_glGetBufferSubData;
PFNGLMAPBUFFERPROC	my_glMapBuffer;
PFNGLUNMAPBUFFERPROC	my_glUnmapBuffer;
PFNGLGETBUFFERPARAMETERIVPROC	my_glGetBufferParameteriv;
PFNGLGETBUFFERPOINTERVPROC	my_glGetBufferPointerv;

#include <stdio.h>

void load_exts_entry_points(void)
{
  // commented-out functions aren't used in the app and even the entry points are eliminated during compilation (so no size increase)

	// FBO (GL 3.0 or EXT_framebuffer_objects)
  my_glIsRenderBuffer = (PFNGLISRENDERBUFFEREXTPROC)(glfwGetProcAddress("glIsRenderBufferEXT"));
  my_glBindRenderbuffer = (PFNGLBINDRENDERBUFFEREXTPROC)(glfwGetProcAddress("glBindRenderbufferEXT"));
  my_glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSEXTPROC)(glfwGetProcAddress("glDeleteRenderbuffersEXT"));
  my_glGenRenderbuffers = (PFNGLGENRENDERBUFFERSEXTPROC)(glfwGetProcAddress("glGenRenderbuffersEXT"));
  my_glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEEXTPROC)(glfwGetProcAddress("glRenderbufferStorageEXT"));
  my_glGetRenderbufferParameteriv = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)(glfwGetProcAddress("glGetRenderbufferParameterivEXT"));
  my_glIsFramebuffer = (PFNGLISFRAMEBUFFEREXTPROC)(glfwGetProcAddress("glIsFramebufferEXT"));
  my_glBindFramebuffer = (PFNGLBINDFRAMEBUFFEREXTPROC)(glfwGetProcAddress("glBindFramebufferEXT"));
  my_glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSEXTPROC)(glfwGetProcAddress("glDeleteFramebuffersEXT"));
  my_glGenFramebuffers = (PFNGLGENFRAMEBUFFERSEXTPROC)(glfwGetProcAddress("glGenFramebuffersEXT"));
  my_glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)(glfwGetProcAddress("glCheckFramebufferStatusEXT"));
  my_glFramebufferTexture1D = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)(glfwGetProcAddress("glFramebufferTexture1DEXT"));
  my_glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)(glfwGetProcAddress("glFramebufferTexture2DEXT"));
  my_glFramebufferTexture3D = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)(glfwGetProcAddress("glFramebufferTexture3DEXT"));
  my_glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)(glfwGetProcAddress("glFramebufferRenderbufferEXT"));
  my_glGetFramebufferAttachmentParameteriv = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)(glfwGetProcAddress("glGetFramebufferAttachmentParameterivEXT"));
  my_glGenerateMipmap = (PFNGLGENERATEMIPMAPEXTPROC)(glfwGetProcAddress("glGenerateMipmapEXT"));
  my_glBlitFramebuffer = (PFNGLBLITFRAMEBUFFEREXTPROC)(glfwGetProcAddress("glBlitFramebufferEXT"));
  my_glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC)(glfwGetProcAddress("glRenderbufferStorageMultisampleEXT"));

	// shaders (GL 2.0 or ARB_shader_objects + ARB_vertex_shader + ARB_fragment_shader + ARB_shading_language_100)
  my_glAttachShader = (PFNGLATTACHSHADERPROC)(glfwGetProcAddress("glAttachObjectARB"));
  my_glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)(glfwGetProcAddress("glBindAttribLocationARB"));
  my_glCompileShader = (PFNGLCOMPILESHADERPROC)(glfwGetProcAddress("glCompileShaderARB"));
  my_glCreateProgram = (PFNGLCREATEPROGRAMPROC)(glfwGetProcAddress("glCreateProgramObjectARB"));
  my_glCreateShader = (PFNGLCREATESHADERPROC)(glfwGetProcAddress("glCreateShaderObjectARB"));
  my_glDeleteProgram = (PFNGLDELETEPROGRAMPROC)(glfwGetProcAddress("glDeleteObjectARB"));
  my_glDeleteShader = (PFNGLDELETESHADERPROC)(glfwGetProcAddress("glDeleteObjectARB"));
  my_glDetachShader = (PFNGLDETACHSHADERPROC)(glfwGetProcAddress("glDetachObjectARB"));
  my_glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)(glfwGetProcAddress("glDisableVertexAttribArrayARB"));
  my_glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)(glfwGetProcAddress("glEnableVertexAttribArrayARB"));
  my_glGetActiveAttrib = (PFNGLGETACTIVEATTRIBPROC)(glfwGetProcAddress("glGetActiveAttribARB"));
  my_glGetActiveUniform = (PFNGLGETACTIVEUNIFORMPROC)(glfwGetProcAddress("glGetActiveUniformARB"));
  my_glGetAttachedShaders = (PFNGLGETATTACHEDSHADERSPROC)(glfwGetProcAddress("glGetAttachedObjectsARB"));
  my_glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)(glfwGetProcAddress("glGetAttribLocationARB"));
  my_glGetProgramiv = (PFNGLGETPROGRAMIVPROC)(glfwGetProcAddress("glGetObjectParameterivARB"));
  my_glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)(glfwGetProcAddress("glGetInfoLogARB"));
  my_glGetShaderiv = (PFNGLGETSHADERIVPROC)(glfwGetProcAddress("glGetObjectParameterivARB"));
  my_glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)(glfwGetProcAddress("glGetInfoLogARB"));
  my_glGetShaderSource = (PFNGLGETSHADERSOURCEPROC)(glfwGetProcAddress("glGetShaderSourceARB"));
  my_glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)(glfwGetProcAddress("glGetUniformLocationARB"));
  my_glGetUniformfv = (PFNGLGETUNIFORMFVPROC)(glfwGetProcAddress("glGetUniformfvARB"));
  my_glGetUniformiv = (PFNGLGETUNIFORMIVPROC)(glfwGetProcAddress("glGetUniformivARB"));
  my_glGetVertexAttribdv = (PFNGLGETVERTEXATTRIBDVPROC)(glfwGetProcAddress("glGetVertexAttribdvARB"));
  my_glGetVertexAttribfv = (PFNGLGETVERTEXATTRIBFVPROC)(glfwGetProcAddress("glGetVertexAttribfvARB"));
  my_glGetVertexAttribiv = (PFNGLGETVERTEXATTRIBIVPROC)(glfwGetProcAddress("glGetVertexAttribivARB"));
  my_glGetVertexAttribPointerv = (PFNGLGETVERTEXATTRIBPOINTERVPROC)(glfwGetProcAddress("glGetVertexAttribPointervARB"));
  //my_glIsProgram = (PFNGLISPROGRAMPROC)(glfwGetProcAddress("glIsProgram"));
  //my_glIsShader = (PFNGLISSHADERPROC)(glfwGetProcAddress("glIsShader"));
  my_glLinkProgram = (PFNGLLINKPROGRAMPROC)(glfwGetProcAddress("glLinkProgramARB"));
  my_glShaderSource = (PFNGLSHADERSOURCEPROC)(glfwGetProcAddress("glShaderSourceARB"));
  my_glUseProgram = (PFNGLUSEPROGRAMPROC)(glfwGetProcAddress("glUseProgramObjectARB"));
  my_glUniform1f = (PFNGLUNIFORM1FPROC)(glfwGetProcAddress("glUniform1fARB"));
  my_glUniform2f = (PFNGLUNIFORM2FPROC)(glfwGetProcAddress("glUniform2fARB"));
  my_glUniform3f = (PFNGLUNIFORM3FPROC)(glfwGetProcAddress("glUniform3fARB"));
  my_glUniform4f = (PFNGLUNIFORM4FPROC)(glfwGetProcAddress("glUniform4fARB"));
  my_glUniform1i = (PFNGLUNIFORM1IPROC)(glfwGetProcAddress("glUniform1iARB"));
  my_glUniform2i = (PFNGLUNIFORM2IPROC)(glfwGetProcAddress("glUniform2iARB"));
  my_glUniform3i = (PFNGLUNIFORM3IPROC)(glfwGetProcAddress("glUniform3iARB"));
  my_glUniform4i = (PFNGLUNIFORM4IPROC)(glfwGetProcAddress("glUniform4iARB"));
  my_glUniform1fv = (PFNGLUNIFORM1FVPROC)(glfwGetProcAddress("glUniform1fvARB"));
  my_glUniform2fv = (PFNGLUNIFORM2FVPROC)(glfwGetProcAddress("glUniform2fvARB"));
  my_glUniform3fv = (PFNGLUNIFORM3FVPROC)(glfwGetProcAddress("glUniform3fvARB"));
  my_glUniform4fv = (PFNGLUNIFORM4FVPROC)(glfwGetProcAddress("glUniform4fvARB"));
  my_glUniform1iv = (PFNGLUNIFORM1IVPROC)(glfwGetProcAddress("glUniform1ivARB"));
  my_glUniform2iv = (PFNGLUNIFORM2IVPROC)(glfwGetProcAddress("glUniform2ivARB"));
  my_glUniform3iv = (PFNGLUNIFORM3IVPROC)(glfwGetProcAddress("glUniform3ivARB"));
  my_glUniform4iv = (PFNGLUNIFORM4IVPROC)(glfwGetProcAddress("glUniform4ivARB"));
  my_glUniformMatrix2fv = (PFNGLUNIFORMMATRIX2FVPROC)(glfwGetProcAddress("glUniformMatrix2fvARB"));
  my_glUniformMatrix3fv = (PFNGLUNIFORMMATRIX3FVPROC)(glfwGetProcAddress("glUniformMatrix3fvARB"));
  my_glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)(glfwGetProcAddress("glUniformMatrix4fvARB"));
  my_glValidateProgram = (PFNGLVALIDATEPROGRAMPROC)(glfwGetProcAddress("glValidateProgramARB"));
  //my_glVertexAttrib1d = (PFNGLVERTEXATTRIB1DPROC)(glfwGetProcAddress("glVertexAttrib1dARB"));
  //my_glVertexAttrib1dv = (PFNGLVERTEXATTRIB1DVPROC)(glfwGetProcAddress("glVertexAttrib1dvARB"));
  //my_glVertexAttrib1f = (PFNGLVERTEXATTRIB1FPROC)(glfwGetProcAddress("glVertexAttrib1fARB"));
  //my_glVertexAttrib1fv = (PFNGLVERTEXATTRIB1FVPROC)(glfwGetProcAddress("glVertexAttrib1fvARB"));
  //my_glVertexAttrib1s = (PFNGLVERTEXATTRIB1SPROC)(glfwGetProcAddress("glVertexAttrib1sARB"));
  //my_glVertexAttrib1sv = (PFNGLVERTEXATTRIB1SVPROC)(glfwGetProcAddress("glVertexAttrib1svARB"));
  //my_glVertexAttrib2d = (PFNGLVERTEXATTRIB2DPROC)(glfwGetProcAddress("glVertexAttrib2dARB"));
  //my_glVertexAttrib2dv = (PFNGLVERTEXATTRIB2DVPROC)(glfwGetProcAddress("glVertexAttrib2dvARB"));
  //my_glVertexAttrib2f = (PFNGLVERTEXATTRIB2FPROC)(glfwGetProcAddress("glVertexAttrib2fARB"));
  //my_glVertexAttrib2fv = (PFNGLVERTEXATTRIB2FVPROC)(glfwGetProcAddress("glVertexAttrib2fvARB"));
  //my_glVertexAttrib2s = (PFNGLVERTEXATTRIB2SPROC)(glfwGetProcAddress("glVertexAttrib2sARB"));
  //my_glVertexAttrib2sv = (PFNGLVERTEXATTRIB2SVPROC)(glfwGetProcAddress("glVertexAttrib2svARB"));
  //my_glVertexAttrib3d = (PFNGLVERTEXATTRIB3DPROC)(glfwGetProcAddress("glVertexAttrib3dARB"));
  //my_glVertexAttrib3dv = (PFNGLVERTEXATTRIB3DVPROC)(glfwGetProcAddress("glVertexAttrib3dvARB"));
  //my_glVertexAttrib3f = (PFNGLVERTEXATTRIB3FPROC)(glfwGetProcAddress("glVertexAttrib3fARB"));
  //my_glVertexAttrib3fv = (PFNGLVERTEXATTRIB3FVPROC)(glfwGetProcAddress("glVertexAttrib3fvARB"));
  //my_glVertexAttrib3s = (PFNGLVERTEXATTRIB3SPROC)(glfwGetProcAddress("glVertexAttrib3sARB"));
  //my_glVertexAttrib3sv = (PFNGLVERTEXATTRIB3SVPROC)(glfwGetProcAddress("glVertexAttrib3svARB"));
  //my_glVertexAttrib4Nbv = (PFNGLVERTEXATTRIB4NBVPROC)(glfwGetProcAddress("glVertexAttrib4NbvARB"));
  //my_glVertexAttrib4Niv = (PFNGLVERTEXATTRIB4NIVPROC)(glfwGetProcAddress("glVertexAttrib4NivARB"));
  //my_glVertexAttrib4Nsv = (PFNGLVERTEXATTRIB4NSVPROC)(glfwGetProcAddress("glVertexAttrib4NsvARB"));
//  my_glVertexAttrib4Nub = (PFNGLVERTEXATTRIB4NUBPROC)(glfwGetProcAddress("glVertexAttrib4NubARB"));
  //my_glVertexAttrib4Nubv = (PFNGLVERTEXATTRIB4NUBVPROC)(glfwGetProcAddress("glVertexAttrib4NubvARB"));
  //my_glVertexAttrib4Nuiv = (PFNGLVERTEXATTRIB4NUIVPROC)(glfwGetProcAddress("glVertexAttrib4NuivARB"));
  //my_glVertexAttrib4Nusv = (PFNGLVERTEXATTRIB4NUSVPROC)(glfwGetProcAddress("glVertexAttrib4NusvARB"));
  //my_glVertexAttrib4bv = (PFNGLVERTEXATTRIB4BVPROC)(glfwGetProcAddress("glVertexAttrib4bvARB"));
  //my_glVertexAttrib4d = (PFNGLVERTEXATTRIB4DPROC)(glfwGetProcAddress("glVertexAttrib4dARB"));
  //my_glVertexAttrib4dv = (PFNGLVERTEXATTRIB4DVPROC)(glfwGetProcAddress("glVertexAttrib4dvARB"));
  //my_glVertexAttrib4f = (PFNGLVERTEXATTRIB4FPROC)(glfwGetProcAddress("glVertexAttrib4fARB"));
  //my_glVertexAttrib4fv = (PFNGLVERTEXATTRIB4FVPROC)(glfwGetProcAddress("glVertexAttrib4fvARB"));
  //my_glVertexAttrib4iv = (PFNGLVERTEXATTRIB4IVPROC)(glfwGetProcAddress("glVertexAttrib4ivARB"));
  //my_glVertexAttrib4s = (PFNGLVERTEXATTRIB4SPROC)(glfwGetProcAddress("glVertexAttrib4sARB"));
  //my_glVertexAttrib4sv = (PFNGLVERTEXATTRIB4SVPROC)(glfwGetProcAddress("glVertexAttrib4svARB"));
  //my_glVertexAttrib4ubv = (PFNGLVERTEXATTRIB4UBVPROC)(glfwGetProcAddress("glVertexAttrib4ubvARB"));
  //my_glVertexAttrib4uiv = (PFNGLVERTEXATTRIB4UIVPROC)(glfwGetProcAddress("glVertexAttrib4uivARB"));
  //my_glVertexAttrib4usv = (PFNGLVERTEXATTRIB4USVPROC)(glfwGetProcAddress("glVertexAttrib4usvARB"));
  my_glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)(glfwGetProcAddress("glVertexAttribPointerARB"));

	// VBO (GL 1.5 or GL_ARB_vertex_buffer_object)
  my_glBindBuffer = (PFNGLBINDBUFFERPROC)(glfwGetProcAddress("glBindBufferARB"));
  my_glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)(glfwGetProcAddress("glDeleteBuffersARB"));
  my_glGenBuffers = (PFNGLGENBUFFERSPROC)(glfwGetProcAddress("glGenBuffersARB"));
  my_glIsBuffer = (PFNGLISBUFFERPROC)(glfwGetProcAddress("glIsBufferARB"));
  my_glBufferData = (PFNGLBUFFERDATAPROC)(glfwGetProcAddress("glBufferDataARB"));
  my_glBufferSubData = (PFNGLBUFFERSUBDATAPROC)(glfwGetProcAddress("glBufferSubDataARB"));
  my_glGetBufferSubData = (PFNGLGETBUFFERSUBDATAPROC)(glfwGetProcAddress("glGetBufferSubDataARB"));
  my_glMapBuffer = (PFNGLMAPBUFFERPROC)(glfwGetProcAddress("glMapBufferARB"));
  my_glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)(glfwGetProcAddress("glUnmapBufferARB"));
  my_glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)(glfwGetProcAddress("glGetBufferParameterivARB"));
  my_glGetBufferPointerv = (PFNGLGETBUFFERPOINTERVPROC)(glfwGetProcAddress("glGetBufferPointervARB"));
}
