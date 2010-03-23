
#include "procedural.h"
#include "gldefs.h"
#include "perlin.h"
#include "rtt_fbo.h"

vec2_ary_t Procedural::circle_points(float pos_x,float pos_y,float r,float n,float w_x,float w_y,float arc)
{
	vec2_ary_t points;
	for(int i=0;i<=n;i++) {
		float angle = (i / n) * 2 * (float) M_PI * arc;
		float x = pos_x + r * cos(angle) * w_x;
		float y = pos_y + r * sin(angle) * w_y;
		vec2 p(x,y);
		points.push_back(p);
	}
	return points;
}

void Procedural::draw_circle(float size,float segments)
{
	vec2_ary_t points = Procedural::circle_points(0,0,size,segments);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0,0);
	FOR_EACH_CONST(vec2_ary_t,points,it) { glVertex(*it); }
	glEnd();
}

void Procedural::draw_circle(Circle &circle,float segments)
{
  vec2_ary_t points = Procedural::circle_points(circle.pos[0],circle.pos[1],circle.r,segments);

	glBegin(GL_TRIANGLE_FAN);
	glVertex(circle.pos);
	FOR_EACH_CONST(vec2_ary_t,points,it) { glVertex(*it); }
	glEnd();
}

vec2_ary_t Procedural::bezier_cubic_curve(vec2 pa,vec2 pb,vec2 pc,vec2 pd,int segments)
{
	vec2_ary_t verts;
	for(float b=0.0; b<=1.0f; b+= (1.0f/segments)) {
		if (b==0.0) // first point already added before calling this func (in SVG path evaluation)
			continue;

		float a = 1.0f - b;
		float x = pa[0]*a*a*a + pb[0]*3*a*a*b + pc[0]*3*a*b*b + pd[0]*b*b*b;
		float y = pa[1]*a*a*a + pb[1]*3*a*a*b + pc[1]*3*a*b*b + pd[1]*b*b*b;
		verts.push_back(vec2(x,y));
	}
	return verts;
}

vec2_ary_t Procedural::bezier_quadratic_curve(vec2 pa,vec2 pb,vec2 pc,int segments)
{
	vec2_ary_t verts;
	for(float b=0.0f; b<=1.0f; b+= (1.0f/segments)) {
		if (b==0.0f) // first point already added before calling this func (in SVG path evaluation)
			continue;

		float a = 1.0f - b;
		float x = pa[0]*a*a + pb[0]*2*a*b + pc[0]*b*b;
		float y = pa[1]*a*a + pb[1]*2*a*b + pc[1]*b*b;
		verts.push_back(vec2(x,y));
	}
	return verts;
}

GLuint Procedural::smoke_particle_texture(int size)
{
	GLuint texture=0;

	// smoke particle texture
	float *data = new float[size*size];
	
	Perlin *p = new Perlin(4,2.0, 0.5,1234); // octaves, freq, amp, seed

	float half_size = size / 2;
	for(int y=0;y<size;++y) {
		for(int x=0;x<size;++x) {
			// perlin noise
			data[y*size+x] = 1.0-((p->Get((float)x/size,(float)y/size))+0.5); // perlin texture coord in 0.0..1.0

			// spotlight texture, strongest at center, fade out towards corners
			float dist = sqrt((x-half_size)*(x-half_size) + (y-half_size)*(y-half_size));
			dist /= half_size;
			if (dist>1.0)
				dist = 1.0;
			
			// combine perlin with the spotlight
			data[y*size+x] *= (1.0-dist);
		}
	}

	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,size,size,0,GL_ALPHA,GL_FLOAT,data);
	my_glGenerateMipmap(GL_TEXTURE_2D);

	delete p;
	delete[] data;

	return texture;
}

RTT *setup_rtt(int t_size,GLenum format=GL_RGB)
{
  RTT *rtt = new RTT_FBO(1,t_size,t_size,false,false,format);
  rtt->start_render();

  glClearColor(0.0,0.0,0.0,0.0);
  glViewport(0,0,t_size,t_size);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION); glLoadIdentity();
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	return rtt;
}

GLuint finish_rtt(RTT *rtt)
{
  rtt->end_render();
  GLuint texture = rtt->get_texid();
	delete rtt;
	return texture;
}

GLuint Procedural::bubble_particle_texture(int t_size)
{
	RTT *rtt = setup_rtt(t_size,GL_RGBA);

	glColor4f(1.0,1.0,1.0,1.0);
	draw_circle(0.9,64);

	glColor4f(0.0,0.0,0.0,0.0);
	draw_circle(0.8,64);

	return finish_rtt(rtt);
}

GLuint Procedural::circle_texture(int t_size)
{
	RTT *rtt = setup_rtt(t_size,GL_RGBA);

	glColor4f(1.0,1.0,1.0,1.0);
	draw_circle(0.9,64);

	return finish_rtt(rtt);
}


#define Q_CIRCLE \
glColor4f(1.0,1.0,1.0,1.0); \
draw_circle(0.8,64); \
glColor4f(0.0,0.0,0.0,0.0); \
draw_circle(0.6,64); \
glLoadIdentity();

GLuint Procedural::crosshair_texture(int t_size)
{
	RTT *rtt = setup_rtt(t_size,GL_RGBA);

	int size=1.0;
	
	glTranslatef(-size,-size,0.0); Q_CIRCLE
	glTranslatef(+size,+size,0.0); Q_CIRCLE
	glTranslatef(+size,-size,0.0); Q_CIRCLE
	glTranslatef(-size,+size,0.0); Q_CIRCLE

  // trim
  glColor4f(0,0,0,0);
  for (int i=0;i<4;++i) {
    glBegin(GL_QUADS);
    glVertex2f(-1.0,-1.0);
    glVertex2f(-0.9,-1.0);
    glVertex2f(-0.9, 1.0);
    glVertex2f(-1.0, 1.0);
    glEnd();

    glRotatef(90,0,0,1);
  }

  glColor4f(1.0,1.0,1.0,1.0);
  draw_circle(0.2,64);

	return finish_rtt(rtt);
}

GLuint Procedural::cross_texture(int t_size)
{
	RTT *rtt = setup_rtt(t_size,GL_RGBA);

  glColor4f(1.0,1.0,1.0,1.0);
  glRotatef(45,0,0,1);
  glScalef(1.8,0.2,1.0);
  draw_textured_quad(1.0, 0,false);

  glLoadIdentity();
  glRotatef(90+45,0,0,1);
  glScalef(1.8,0.2,1.0);
  draw_textured_quad(1.0, 0,false);

	return finish_rtt(rtt);
}

GLuint Procedural::missile_mark_texture(int t_size)
{
	RTT *rtt = setup_rtt(t_size,GL_RGBA);

	glColor4f(0.3,0.3,0.3,0.3);
	draw_circle(0.9,64);

	glColor4f(0.8,0.8,0.8,0.8);
  glLoadIdentity();
  glRotatef(45,0,0,1);
  glScalef(1.0,0.05,1.0);
  draw_textured_quad(0.5, 0,false);
  glLoadIdentity();
  glRotatef(90+45,0,0,1);
  glScalef(1.0,0.05,1.0);
  draw_textured_quad(0.5, 0,false);

	return finish_rtt(rtt);
}

GLuint Procedural::seawaves_texture(void)
{
	RTT *rtt = setup_rtt(128,GL_RGBA);
//  glColor4f(0,0,0,0.0);
//  draw_textured_quad(1.0, 0,false);
//  glColor4f(1.0,1.0,1.0,0.0);
  glColor4f(1.0,1.0,1.0,0.0);
  draw_textured_quad(2.0, 0,false);

  glTranslatef(0.0,-1.0,0.0);  
  glColor4f(1.0,1.0,1.0,0.5);
  draw_textured_quad(3.9, 0,false);

  GLuint texture = finish_rtt(rtt);
  
  glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);

	return texture;
}


GLuint Procedural::seawaves_texture_f(void)
{
	RTT *rtt = setup_rtt(128,GL_RGBA);
  glColor4f(1.0,1.0,1.0,0.0);
  draw_textured_quad(2.0, 0,false);

  glTranslatef(0.0,-1.0,0.0);  
  glColor4f(1.0,1.0,1.0,1.0);
  draw_textured_quad(3.9, 0,false);

  GLuint texture = finish_rtt(rtt);
  
  glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);

	return texture;
}

GLuint Procedural::line_texture(void)
{
	RTT *rtt = setup_rtt(128,GL_RGBA);
  glColor4f(1.0,1.0,1.0,0.0);
  draw_textured_quad(2.0, 0,false);

  glColor4f(1.0,1.0,1.0,1.0);
  glScalef(3.0,0.5,1.0);
  draw_textured_quad(1.0, 0,false);

  GLuint texture = finish_rtt(rtt);
  
  glBindTexture(GL_TEXTURE_2D,texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

	return texture;
}

//GLuint Procedural::template(int t_size)
//{
//	RTT *rtt = setup_rtt(t_size);
//  // draw here
//
//	return finish_rtt(rtt);
//}
