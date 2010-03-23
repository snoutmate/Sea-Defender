#ifdef WIN32
#  include <windows.h>
#endif

#include <cstdlib>

#include "background.h"
#include "snoutlib/loadingscreen.h"
#include "snoutlib/procedural.h"
#include "layout.h"
#include "resources.h"
#include "snoutlib/gldefs.h"
#include "snoutlib/timer.h"

extern LoadingScreen *g_loadingscreen;
extern Layouts *g_layouts;
extern float g_max_x;
extern float g_max_y;

extern GLenum g_src;
extern GLenum g_dst;

Sea::Sea() :
  m_vbo(0),m_vertcnt(0)
{
  const int steps = 160;
  const float step = (g_max_x*1.01f) / steps;

  float *verts = new float[steps*6];
  m_vertcnt = steps*2;

  for(int i=0;i<steps;++i) {
    float x = i * step;
    verts[i*6+0] = x; verts[i*6+1] = 1.0; verts[i*6+2] = 0.0;
    verts[i*6+3] = x; verts[i*6+4] = 0.0; verts[i*6+5] = 1.0;
  }

  my_glGenBuffers(1,&m_vbo);
  my_glBindBuffer(GL_ARRAY_BUFFER_ARB, m_vbo);
  my_glBufferData(GL_ARRAY_BUFFER_ARB, steps*6*sizeof(float),verts,GL_STATIC_DRAW);
  my_glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);

  delete[] verts;
}

float Sea::sea_func(float x)
{
  float time = get_time_con(16.0f * (float)M_PI);
  return (sin((x+(time/16.0f))*16.0f)/75.0f) + (sin((-x+(time/8.0f))*8.0f)/200.0f);
}

void Sea::draw(float pos,float max_amp, bool blend)
{
  pos -= 0.002; // adjust for texture aa
  // top part
  glColor(g_layouts->cur()->color_sea_top);

  if(blend) {
    glBindTexture(GL_TEXTURE_2D,g_resources.texture_seawaves_f);
//    glBindTexture(GL_TEXTURE_2D,g_resources.texture_seawaves);
  } else {
    glBindTexture(GL_TEXTURE_2D,g_resources.texture_seawaves);
  }

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  
  glEnable(GL_TEXTURE_2D);

	Shader* sh = g_resources.shader_seawaves;
  sh->use();

  float time = get_time_con(16.0f * (float) M_PI);
  my_glUniform1f(sh->get_uni_loc("time"),time);
  my_glUniform1f(sh->get_uni_loc("max_amp"),max_amp);
  my_glUniform1f(sh->get_uni_loc("amp_mod"),0.8f);
  my_glUniform1f(sh->get_uni_loc("vert_pos"),pos);

  my_glBindBuffer(GL_ARRAY_BUFFER_ARB, m_vbo);
  glVertexPointer(3, GL_FLOAT, 0, 0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawArrays(GL_TRIANGLE_STRIP,0,m_vertcnt);
  glDisableClientState(GL_VERTEX_ARRAY);
  my_glBindBuffer(GL_ARRAY_BUFFER_ARB, 0);
  
  sh->unuse();

  glDisable(GL_TEXTURE_2D);  

  // bottom part

  glColor(g_layouts->cur()->color_sea_middle);

  if (blend)
    glDisable(GL_BLEND);
  
  
  // bottom part  
  glBegin(GL_QUADS);
  glVertex2f(g_max_x, pos+max_amp);
  glVertex2f(0, pos+max_amp);
  glColor(g_layouts->cur()->color_sea_bottom);
  glVertex2f(0,g_max_y);
  glVertex2f(g_max_x,g_max_y);
  glEnd();

  glDisable(GL_BLEND);
}

Background::Background() :
  m_sea_pos(0),m_sea_amp(0),m_sea(NULL),m_sky(NULL)
{
  m_sky = new Sky();

  g_loadingscreen->update();

  m_sea_pos = 0.26f;
  m_sea_amp = 0.1f;

  m_sea = new Sea();

  g_loadingscreen->update();
}

Background::~Background()
{
  delete m_sea;
}

void Background::draw(void)
{}

void Background::draw_sea(bool blend)
{
  glPushMatrix();
  m_sea->draw(m_sea_pos,m_sea_amp,blend);
  glPopMatrix();
}

void Background::draw_sky(void)
{
  glPushMatrix();
  m_sky->draw(m_sea_pos + m_sea_amp);
  glPopMatrix();
}

void Background::darken_area(vec4 const &box,float amount)
{
  glColor3d(amount,amount,amount);
  glDisable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ZERO,GL_SRC_COLOR);
  glRectd(box[0],box[1],box[2],box[3]);
  glDisable(GL_BLEND);
}


Clouds::Clouds() :
  m_num_clouds(0), m_num_tex_row(0), m_time_offset(0.0), m_vbo(0),
  m_cloud_texture(0), m_circle_dl(0)
{
  float tex_y_scale = 0.6f; // to improve fillrate, clip the quad by Y size (the cloud is only in upper part of the texture)
  m_num_clouds = 50;
  m_num_tex_row = 8;

  generate_cloud_textures();

  g_loadingscreen->update();

  glLoadIdentity();

  float *cloud_data = new float[m_num_clouds * 6 * 4];

  int pos = 0;  
  for(unsigned int i=0;i<m_num_clouds;++i) {
    float color = RAND_0_1 * 0.25f + 0.75f;
    float speed = RAND_0_1 * 60.0f + 60.0f;
    float height = RAND_0_1 * 0.12f - 0.05f;
    float scale = (RAND_0_1 * 0.5f + 0.2f) * 0.25f;

    float t_step = 1.0f/m_num_tex_row;
  
    float tex_u0 = t_step * floor(RAND_0_1 * m_num_tex_row);
    float tex_v0 = t_step * floor(RAND_0_1 * m_num_tex_row);
    float tex_u1 = tex_u0 + t_step;
    float tex_v1 = tex_v0 + t_step * tex_y_scale;

    // quad
    cloud_data[pos++] = 0;
    cloud_data[pos++] = height;
    cloud_data[pos++] = tex_u0; cloud_data[pos++] = tex_v0; cloud_data[pos++] = color; cloud_data[pos++] = speed;

    cloud_data[pos++] = 0;
    cloud_data[pos++] = height + (scale * tex_y_scale);
    cloud_data[pos++] = tex_u0; cloud_data[pos++] = tex_v1; cloud_data[pos++] = color; cloud_data[pos++] = speed;

    cloud_data[pos++] = scale;
    cloud_data[pos++] = height + (scale * tex_y_scale);
    cloud_data[pos++] = tex_u1; cloud_data[pos++] = tex_v1; cloud_data[pos++] = color; cloud_data[pos++] = speed;

    cloud_data[pos++] = scale;
    cloud_data[pos++] = height;
    cloud_data[pos++] = tex_u1; cloud_data[pos++] = tex_v0; cloud_data[pos++] = color; cloud_data[pos++] = speed;
  }

  my_glGenBuffers(1,&m_vbo);
  my_glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  my_glBufferData(GL_ARRAY_BUFFER, pos * sizeof(float), cloud_data, GL_STATIC_DRAW);
  my_glBindBuffer(GL_ARRAY_BUFFER, 0);

  delete[] cloud_data;

  Shader *sh = g_resources.shader_clouds;
  sh->use();
  my_glBindAttribLocation(sh->get_prog(),0,"pos");
  my_glBindAttribLocation(sh->get_prog(),1,"data");
  my_glUniform1i(sh->get_uni_loc("tex"),0);
  sh->unuse();

  m_time_offset = (float)g_timer->now() - 140.0f;

  g_loadingscreen->update();
}

Clouds::~Clouds()
{
	if (m_cloud_texture)
		glDeleteTextures(1,&m_cloud_texture);	
}

void Clouds::draw(void)
{
  // draw_test_cloud

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(1,1,1,1);

  glBindTexture(GL_TEXTURE_2D,m_cloud_texture);

  Shader *sh = g_resources.shader_clouds;
  sh->use();
  my_glUniform1f(sh->get_uni_loc("time"),(float)g_timer->now() - m_time_offset);

  my_glBindBuffer(GL_ARRAY_BUFFER,m_vbo);
  my_glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,6*4,0);
  my_glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,6*4,(GLvoid *)(4*4));
  my_glBindBuffer(GL_ARRAY_BUFFER,0);

  my_glEnableVertexAttribArray(0);
  my_glEnableVertexAttribArray(1);

  glDrawArrays(GL_QUADS,0,m_num_clouds*4);

  my_glDisableVertexAttribArray(0);
  my_glDisableVertexAttribArray(1);

  sh->unuse();

  glDisable(GL_TEXTURE_2D);
  glDisable(GL_BLEND);
}

void Clouds::generate_cloud_textures(void)
{
  int t_size = g_resources.texture_clouds_size;
  float t_step = 1.0f / m_num_tex_row;

  RTT *m_rtt = new RTT_FBO(1,t_size,t_size);

  m_circle_dl = glGenLists(1);
  glNewList(m_circle_dl,GL_COMPILE);
  Procedural::draw_circle();
  glEndList();

  m_rtt->start_render();

  glClearColor(0.0,0.0,0.0,0.0);

  glViewport(0,0,t_size,t_size);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION); glLoadIdentity();
  glOrtho(0.0,1.0, 0.0,1.0, -1,1);
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();
  glScalef(t_step,t_step,0);

  for(unsigned int x=0;x<m_num_tex_row;++x) {
    for(unsigned int y=0;y<m_num_tex_row;++y) {
      glPushMatrix();
      glTranslatef((float)x,(float)y,0);
      draw_random_cloud();
      glPopMatrix();
    }
//    m_rtt->pause();
//    g_loadingscreen->update();
//    m_rtt->unpause();
  }

  m_rtt->end_render();
  m_cloud_texture = m_rtt->get_texid();

	delete m_rtt;

  g_loadingscreen->update();

  m_cloud_texture = move_texture_red_component(m_cloud_texture);

  g_loadingscreen->update();

  // glFinish()
}

void Clouds::draw_random_cloud(void)
{
  glPushMatrix();
  glTranslatef(0.5f,0.5f,0.0f);
  glScalef(3.4f,3.4f,0.0f);

  vec2_ary_t const &points = Procedural::circle_points(0,0,0.1f,8,1,-0.8f,0.5f);
  vec2_ary_t const &points2 = Procedural::circle_points(0,0,0.1f,8,1,+0.05f,0.5f);

  glBegin(GL_TRIANGLE_FAN);
  glVertex2d(0,0);
  FOR_EACH_CONST(vec2_ary_t,points,it) { glVertex(*it); }
  glEnd();

  for(unsigned int i=0;i<points.size();++i) {
    vec2 const &p = points[i];
    glPushMatrix();
    float scale = RAND_0_1 * 0.22f + 0.21f;
    float trans_x = RAND_0_1 * 0.01f;
    float trans_y = RAND_0_1 * 0.01f;

    if (i==0)
      trans_x -= 0.01f;

    float x = ((p[1] - trans_y) + (0.1f*scale));
    if (x>0)
      trans_y += x;

    glTranslated(p[0]+trans_x,p[1]-trans_y,0);
    glScaled(scale,scale,scale);
    glCallList(m_circle_dl);
    glPopMatrix();
  }

  glBegin(GL_TRIANGLE_FAN);
  glVertex2d(0,0);
  FOR_EACH_CONST(vec2_ary_t,points2,it) { glVertex(*it); }
  glEnd();
  glPopMatrix();
}

Sky::Sky() :
  m_clouds(new Clouds())
{
}

Sky::~Sky()
{
  delete m_clouds;
}

void Sky::draw(float y_pos)
{
  glDisable(GL_BLEND);
  glBegin(GL_QUADS);
  glColor(g_layouts->cur()->color_sky_top);
  glVertex4f(1.61f ,0.0f,-1.0f,1.0);
  glVertex4f(0.0f, 0.0f,-1.0f,1.0);
  glColor(g_layouts->cur()->color_sky_bottom);
  glVertex4f(0, y_pos,-1.0f,1.0);
  glVertex4f(1.61f, y_pos,-1.0f,1.0);
  glEnd();

  m_clouds->draw();
}
