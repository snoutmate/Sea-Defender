#include "credits.h"

#include "resources.h"

Credits::Credits()
{}

void Credits::draw(void)
{
//  t = Time.now().to_f % 1.2 / 1.2
// # glRotatef((1.0-t)*90,0,0,1)

	MFont *font = g_resources.font;

  glTranslatef(0,0.08,0);
  font->print_text("Code & GFX",		vec2(0,0.30),1.0,true,vec4(0,1,0.2,1.0));
  font->print_text("snoutmate@gmail.com",		vec2(0,0.36),1.0,true);
  font->print_text("Resources",		vec2(0,0.45),0.8,true,vec4(0,1,0.2,1.0));
  font->print_text("KRONIKA font - http://apostrophiclab.pedroreina.net",		vec2(0,0.50),0.5,true);
  font->print_text("GLFW library - http://glfw.sourceforge.net",		vec2(0,0.53),0.5,true);
  font->print_text("math3d++ - Trenkwalder Markus <trenki2@gmx.net>",		vec2(0,0.56),0.5,true);

  glTranslatef(0.82,0.68,0.0);
  glScalef(0.2,0.2,1.0);

  glColor3ub(0,0,0);
  g_resources.mesh_opengl_logo->draw();
  glColor3ub(64,64,64);
  glTranslatef(-0.005,-0.005,0.0);
  g_resources.mesh_opengl_logo->draw();

  glColor3ub(20,148,196);
  glTranslatef(-0.005,-0.005,0.0);
  g_resources.mesh_opengl_logo->draw();

  glLoadIdentity();
}

//void Credits::start(void)
//{
//
//}
//
//void Credits::stop(void)
//{
//
//}
//
//bool Credits::finished(void)
//{
//	return true;
//}
