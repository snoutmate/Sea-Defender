#include "loadingscreen.h"
#include "procedural.h"

LoadingScreen::LoadingScreen(Glfwapp& ctx) :
  m_ctx(ctx), m_counter(0), m_step((float)1.0/122.0f)
{
}

void LoadingScreen::draw_progress_indicator(void)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  vec2_ary_t points = Procedural::circle_points(
      0.0,0.0,0.2f,
      64,1,1,
      m_counter * m_step
  );

  vec2_ary_t points2 = Procedural::circle_points(
      0.0,0.0,0.05f,
      64,1,1,
      m_counter * m_step
  );
  
  glColor3f(0.6f,0.6f,1.0);
  
  glScalef(1.0f/1.6f,-1.0,0.0);

  glRotatef(90,0,0,-1);

  glBegin(GL_TRIANGLE_STRIP);
  for(unsigned int i=0;i<points.size();++i) {
    glVertex(points[i]);
    glVertex(points2[i]);
  }
  glEnd();
}

void LoadingScreen::update(void)
{
  // save all state
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glPushAttrib(GL_VIEWPORT_BIT);

  // init empty frame
  m_ctx.init_frame();
  m_ctx.set_ortho2D();
  m_counter++;

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

  draw_progress_indicator();

  m_ctx.end_frame();

  // we're setting scissor in set_ortho2D
  glDisable(GL_SCISSOR_TEST);

  // restore state
  glColor3f(1,1,1);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glPopAttrib();

//	printf("%i\n",m_counter);
}
