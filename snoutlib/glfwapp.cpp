
#include "glfwapp.h"

extern float g_max_x;
extern float g_max_y;

Glfwapp::Glfwapp(const char *title,Settings& settings) :
  m_letterboxed(true),m_window_w(0),m_window_h(0),m_vsync(false)
{
  int res = glfwInit();
  assert(res == GL_TRUE);
  float fixed_ratio = 1.6f;

  // if fixed_ratio is set, we letterbox the image if
  // the window/screen has different width/height ratio
  int w = settings.m_res_x;
  int h = settings.m_res_y;

  int window_mode = (settings.m_fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW);

  float win_ratio = (float)w/(float)h;

  if (win_ratio < fixed_ratio) {
    int new_h = (int) (w / fixed_ratio);
    int shift = (int) ((h - new_h) / 2.0f);
    m_viewport[0] = 0;
    m_viewport[1] = shift;
    m_viewport[2] = w;
    m_viewport[3] = new_h;
    m_letterboxed = true;
  } else if (win_ratio > fixed_ratio) {
    int new_w = (int) (h * fixed_ratio);
    int shift = (int) ((w - new_w) / 2.0f);
    m_viewport[0] = shift;
    m_viewport[1] = 0;
    m_viewport[2] = new_w;
    m_viewport[3] = h;
    m_letterboxed = true;
  } else {
    m_viewport[0] = 0;
    m_viewport[1] = 0;
    m_viewport[2] = w;
    m_viewport[3] = h;
    m_letterboxed = false;
  }

  glfwOpenWindowHint(GLFW_FSAA_SAMPLES,settings.m_fsaa);
  glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE,GL_TRUE);
  
  res = glfwOpenWindow( w, h, 8,8,8, 8, 0, 0, window_mode );
	if (res != GL_TRUE) {
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES,0);
	  res = glfwOpenWindow( w, h, 8,8,8, 8, 0, 0, window_mode );
		settings.m_fsaa = 0;
	}
  assert(res == GL_TRUE);

  if (window_mode == GLFW_WINDOW) {
    GLFWvidmode desktop_mode;
    glfwGetDesktopMode(&desktop_mode);
    glfwSetWindowPos((int)((desktop_mode.Width - w)/2.0f),(int)((desktop_mode.Height - h)/2.0f));
  }

  m_vsync = (settings.m_vsync);
  set_vsync();

//  glfwDisable(GLFW_AUTO_POLL_EVENTS);
  glfwEnable(GLFW_STICKY_KEYS);
  glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);
  glfwEnable(GLFW_SYSTEM_KEYS);

  glfwEnable(GLFW_KEY_REPEAT);

  glfwDisable(GLFW_MOUSE_CURSOR);

  glfwSetWindowTitle(title);

  m_window_w = w;
  m_window_h = h;

  if (m_letterboxed)
    m_window_h = (int) (w / fixed_ratio);
}

void Glfwapp::set_vsync(void)
{
  if (m_vsync)
    glfwSwapInterval(1);
}

void Glfwapp::reset_vsync(void)
{
  glfwSwapInterval(0);
}

Glfwapp::~Glfwapp()
{
  glfwCloseWindow();  
  glfwTerminate();
}


void Glfwapp::set_viewport(void)
{
  if (m_letterboxed)
    glDisable(GL_SCISSOR_TEST);

  glClearColor(0,0,0,255);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glViewport(m_viewport[0],m_viewport[1],m_viewport[2],m_viewport[3]);
  
  if (m_letterboxed) {
    glScissor(m_viewport[0],m_viewport[1],m_viewport[2],m_viewport[3]);
    glEnable(GL_SCISSOR_TEST);
  }
}

// ortho mode is set so that height of drawable area equals to 1.0 opengl units
// with origin at top left (thus width equals to widow w/h ratio)
void Glfwapp::set_ortho2D(void)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,(float) (m_window_w / (float)m_window_h),1.0,0, -1,1);

  g_max_x = m_window_w / (float) m_window_h;
  g_max_y = 1.0;

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

bool Glfwapp::window_opened(void)
{
  return (glfwGetWindowParam(GLFW_OPENED) == GL_TRUE);
}

bool Glfwapp::window_closed(void) { return !window_opened(); }

void Glfwapp::init_frame(void)
{
  glfwPollEvents();
  set_viewport();
}

void Glfwapp::end_frame(void)
{
  glfwSwapBuffers();
}
