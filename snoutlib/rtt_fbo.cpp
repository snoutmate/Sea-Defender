#include "rtt_fbo.h"
#include <stdlib.h>
#include "gldefs.h"

//#define FBO_DEBUG 1

RTT_FBO::RTT_FBO(unsigned int nbuffers, unsigned int size_x, unsigned int size_y, bool msaa, bool depth_tex,GLenum textureformat) :
  m_msaa(msaa), m_depth_tex(depth_tex), m_nbuffers(nbuffers), m_width(size_x), m_height(size_y),
  m_fbo(0),m_fbo_msaa(0),m_msaa_depth(0),m_msaa_rb(0),m_depth(false),m_tex_ids(NULL)

{
  if (m_msaa &&
		(
			glfwExtensionSupported("GL_EXT_framebuffer_multisample") == GL_FALSE || 
			glfwExtensionSupported("GL_EXT_framebuffer_blit") == GL_FALSE)
		)
  {
		printf("FBO multisampling not supported, disabling\n");
		m_msaa = false;
  }

	m_tex_ids = new GLuint[m_nbuffers];

  if (size_x==0) {
    GLint params[4] = {0};
    glGetIntegerv(GL_VIEWPORT,params);
    m_width = params[2];
    m_height = params[3];
  }

  m_depth_tex = depth_tex;
  
  #ifdef FBO_DEBUG
    printf("new FBO -> m_msaa: %s, size: %i x %i, depthtex: %s, nbuffers: %i \n",
           m_msaa == true ? "true" : "false",
           m_width, m_height,
           m_depth_tex == true ? "true" : "false",
           m_nbuffers
           );
  #endif

  my_glGenFramebuffers(1,&m_fbo);
  my_glGenFramebuffers(1,&m_fbo_msaa);

  my_glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_fbo);

  if (m_depth_tex) {
    glGenTextures(1,&m_depth);
    glBindTexture(GL_TEXTURE_2D, m_depth);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,  m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    my_glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depth, 0);
  } else { // no depth texture needed, just use renderbuffer
    my_glGenRenderbuffers(1,&m_depth);
    my_glBindRenderbuffer(GL_RENDERBUFFER_EXT, m_depth);
    my_glRenderbufferStorage(GL_RENDERBUFFER_EXT,GL_DEPTH_COMPONENT, m_width, m_height);
    my_glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_depth);
  }

  glGenTextures(nbuffers,m_tex_ids);
  for(unsigned int i=0;i<m_nbuffers;++i) {
    GLuint t=m_tex_ids[i];

    glBindTexture(GL_TEXTURE_2D, t);
    glEnable(GL_TEXTURE_2D);    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    glTexImage2D(GL_TEXTURE_2D, 0, textureformat, m_width, m_height, 0, textureformat, GL_UNSIGNED_BYTE, 0);
    my_glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT + i, GL_TEXTURE_2D, t, 0);
    glDisable(GL_TEXTURE_2D);    
  }

  GLenum status = my_glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

  if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
    printf("FBO error: %i\n",status);
    exit(-1);
  }

  // multisampling, create second FBO with multisampled framebuffer
  if (m_msaa) {

    my_glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_fbo_msaa);
    my_glGenRenderbuffers(1,&m_msaa_depth);
    my_glGenRenderbuffers(1,&m_msaa_rb);

    GLint samples=0;
    glGetIntegerv(GL_MAX_SAMPLES_EXT,&samples);
    if (samples>2) samples=2;
//    printf("%i\n",samples);

    my_glBindRenderbuffer(GL_RENDERBUFFER_EXT, m_msaa_depth);
    my_glRenderbufferStorageMultisample(GL_RENDERBUFFER_EXT,samples,GL_DEPTH_COMPONENT, m_width, m_height);
    my_glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_msaa_depth);

    my_glBindRenderbuffer(GL_RENDERBUFFER_EXT, m_msaa_rb);
    my_glRenderbufferStorageMultisample(GL_RENDERBUFFER_EXT,samples,textureformat, m_width, m_height);
    my_glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, m_msaa_rb);
    status = my_glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

    if (status != GL_FRAMEBUFFER_COMPLETE_EXT) {
      printf("MSAA FBO error: %i\n",status);
      exit(-1);
    }
  }
  
  my_glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
}

void RTT_FBO::start_render(unsigned int num)
{
  bind(num);
}

void RTT_FBO::end_render(void)
{

  if (m_msaa) {
    my_glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, m_fbo_msaa);
    my_glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, m_fbo);
    my_glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
  }
  
  unbind();

//	if (m_msaa) {
	// FIXME: mipmapy pro vsechny buffer textury


	glBindTexture(GL_TEXTURE_2D, m_tex_ids[0]);
	glEnable(GL_TEXTURE_2D);

	my_glGenerateMipmap(GL_TEXTURE_2D);
	glDisable(GL_TEXTURE_2D);
//	}
}

  
void RTT_FBO::bind(unsigned int n)
{
  if (m_msaa) {
    my_glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_fbo_msaa);
    //glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT);
  } else {
    my_glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_fbo);
    //glDrawBuffer(GL_COLOR_ATTACHMENT0_EXT+n);
  }
}

void RTT_FBO::unbind(void)
{
  my_glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
//  glDrawBuffer(GL_BACK);
}

unsigned int RTT_FBO::get_texid(unsigned int num)
{
  return m_tex_ids[num];
}

RTT_FBO::~RTT_FBO()
{
	delete[] m_tex_ids;

  if (m_msaa) {
		my_glBindFramebuffer(GL_FRAMEBUFFER_EXT,m_fbo_msaa);
    my_glDeleteRenderbuffers(1,&m_msaa_depth);
    my_glDeleteRenderbuffers(1,&m_msaa_rb);
	  my_glDeleteFramebuffers(1,&m_fbo_msaa);
  }

  my_glBindFramebuffer(GL_FRAMEBUFFER_EXT,m_fbo);

  if (m_depth_tex)
    glDeleteTextures(1,&m_depth);
	else
    my_glDeleteRenderbuffers(1,&m_depth);

  my_glDeleteFramebuffers(1,&m_fbo);
  my_glBindFramebuffer(GL_FRAMEBUFFER_EXT,0);

//  glDeleteTextures(m_nbuffers,m_tex_ids); // deleted by caller
}

RTT::~RTT() {}
