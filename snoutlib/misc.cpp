
#include <algorithm>

#include "misc.h"
#include "timer.h"
#include "gldefs.h"
#include "bbox.h"
#include "../resources.h"


extern Timer *g_timer;


FPScounter::FPScounter(float interval) :
  m_interval(interval),m_frames(0),
  m_old_t((float)glfwGetTime()),
  m_fps(0.0f)
{
}

float FPScounter::next_frame(void)
{
  m_frames++;
	float t = (float) glfwGetTime();
  if ((t - m_old_t)>m_interval) {
    m_fps = m_frames / (t - m_old_t);
    m_frames = 0;
    m_old_t = (float) t;
  }
  return m_fps;
}

double_ary_t vec2_ary_to_double_ary(vec2_ary_t v2ary)
{
  double_ary_t out;
  out.reserve(v2ary.size()*2);
  for(vec2_ary_t::const_iterator it=v2ary.begin();it!=v2ary.end();++it) {
    out.push_back((*it)[0]); // x
    out.push_back((*it)[1]); // y
  }
  return out;
}

float_ary_t vec2_ary_to_float_ary(vec2_ary_t v2ary)
{
  float_ary_t out;
  out.reserve(v2ary.size()*2);
  for(vec2_ary_t::const_iterator it=v2ary.begin();it!=v2ary.end();++it) {
    out.push_back((*it)[0]); // x
    out.push_back((*it)[1]); // y
  }
  return out;
}

float get_time_con(float c)
{
  return get_time_con_time((float)g_timer->now(), c);
}

float get_time_con_time(float t, float c)
{
  float tmp;
  float tc = t / c;
  float fract = modf(tc,&tmp);
  return fract * c;
}

void save_texture_as_ppm(GLuint texid,const char *fname)
{
	GLint width=0,height=0;
	glBindTexture(GL_TEXTURE_2D,texid);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&height);

	unsigned char *data = new unsigned char[width*height*3];

	glGetTexImage(GL_TEXTURE_2D,0,GL_RGB,GL_UNSIGNED_BYTE,data);

	FILE *f = fopen(fname,"wb");
	if (!f)
		return;
	fprintf(f,"P6\n%i %i\n255\n",width,height);
	fwrite(data, 1,width*height*3,f);
	fclose(f);
	
	glBindTexture(GL_TEXTURE_2D,0);
}

void draw_textured_quad(float size, GLuint texture,bool blend)
{
	if (blend) {
		glEnable(GL_BLEND);
    glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
 	} else {
		glDisable(GL_BLEND);
	}
  
  size /= 2.0;

  if (texture) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture);
  }

	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex2f(-size,-size);
	glTexCoord2f(0.0,1.0); glVertex2f(-size, size);
	glTexCoord2f(1.0,1.0); glVertex2f( size, size);
	glTexCoord2f(1.0,0.0); glVertex2f( size,-size);
	glEnd();
	if (blend)
		glDisable(GL_BLEND);
  if (texture) 
  	glDisable(GL_TEXTURE_2D);
}

void draw_aa_line(Line l,float width,vec4 color,float stipple)
{
  // stretch line to quad
  vec2 linedir = normalized(l.p2 - l.p1); // line direction vector
  linedir *= 0.001 * width; // scale normalized vector with line width
  vec2 p1_a = vec2(+linedir[1],-linedir[0]) + l.p1; // rotate vector 90 degrees and add to p1
  vec2 p1_b = vec2(-linedir[1],+linedir[0]) + l.p1; // rotate vector 270 degrees and add to p1
  vec2 p2_a = vec2(+linedir[1],-linedir[0]) + l.p2; // rotate vector 90 degrees and add to p2
  vec2 p2_b = vec2(-linedir[1],+linedir[0]) + l.p2; // rotate vector 270 degrees and add to p2

  float len = length(l.p2 - l.p1);

  glBindTexture(GL_TEXTURE_2D,g_resources.texture_line);

  len *= stipple;

  g_resources.shader_line->use();
  my_glUniform1f(g_resources.shader_line->get_uni_loc("stipple"),clamp_0_1(stipple));

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  glColor(color);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0,0.0); glVertex(p1_a);
	glTexCoord2f(0.0,1.0); glVertex(p1_b);
	glTexCoord2f(len,1.0); glVertex(p2_b);
	glTexCoord2f(len,0.0); glVertex(p2_a);
	glEnd();
  g_resources.shader_line->unuse();

  glDisable(GL_TEXTURE_2D);
}


// copy RGBA texture's R channel to new 1-component texture, downsamples 2x (uses first mipmap), deletes original texture
// - used because some cards doesn't support direct FBO rendering to single-component texture
//   and sticking with RGBA even when we use only 1 channel (font, models etc.) kills both performance and VRAM
// - downsampling is done because we render to FBO at 2x size to supersample
//   (FBO multisampling is sadly not supported on many configurations, especially with pre-8xxx nvidia cards on Mac OS X)
GLuint move_texture_red_component(GLuint texid)
{
  // TODO: PBO
  int width,height;
  glBindTexture(GL_TEXTURE_2D, texid);
  glGetTexLevelParameteriv(GL_TEXTURE_2D,1,GL_TEXTURE_WIDTH,&width);
  glGetTexLevelParameteriv(GL_TEXTURE_2D,1,GL_TEXTURE_HEIGHT,&height);

  unsigned char*data = new unsigned char[width*height];
  glGetTexImage(GL_TEXTURE_2D,1,GL_RED,GL_UNSIGNED_BYTE,data);

  glDeleteTextures(1,&texid);

  //
  GLuint newtexid = 0;
  glGenTextures(1,&newtexid);
  
  glBindTexture(GL_TEXTURE_2D, newtexid);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  
  glTexImage2D(GL_TEXTURE_2D, 0, 1, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
 	my_glGenerateMipmap(GL_TEXTURE_2D);

  delete[] data;
  
  return newtexid;
}


vec3 hsv2rgb(vec3 const& hsv)
{
  float h = hsv[0] * 6.0f;
  float s = hsv[1];
  float v = hsv[2];
  vec3 out;

  int i = floor(h);
  float f = h - i;
  if ( !(i&1) )  // if i is even
    f = 1 - f;

  float m = v * (1 - s);
  float n = v * (1 - s * f);

  switch (i) {
    default:
    case 6:
    case 0: return vec3(v, n, m);
    case 1: return vec3(n, v, m);
    case 2: return vec3(m, v, n);
    case 3: return vec3(m, n, v);
    case 4: return vec3(n, m, v);
    case 5: return vec3(v, m, n);
  }
  /* not reached */
}


vec3 rgb2hsv(vec3 const& rgb)
{
  float r = rgb[0];
  float g = rgb[1];
  float b = rgb[2];

  float x = min(min(r,g),b);
  float v = max(max(r,g),b);

	if(v == x) // greyscale => undefined hue
    return vec3(0, 0, v);

	float f = (r == x) ? g - b : ((g == x) ? b - r : r - g);
	int i = (r == x) ? 3 : ((g == x) ? 5 : 1);

  return vec3((i - f /(v - x))/6.0f, (v - x)/v, v);
}

extern "C" void __cxa_pure_virtual (void) {}
