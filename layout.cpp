
#include "layout.h"

Layouts::Layouts() :
  m_current_layout(NULL),m_layouts()
{
  setup_layouts();
}

Layouts::~Layouts()
{
  FOR_EACH_CONST(layouts_t,m_layouts,it)
    delete it->second;
}

void Layouts::add_layout(Layout *l,const string &name)
{
  m_layouts[name] = l;
  m_layout_names.push_back(name);
}

void Layouts::setup_layouts(void)
{
  Layout *l;

  l = new Layout();
  l->color_sea_top    = vec4(0.25f,0.4f,0.55f,1.0f);
  l->color_sea_middle = vec4(0.25f,0.4f,0.55f,0.5f);
  l->color_sea_bottom = vec4(0.1f,0.4f,0.3f,0.5f);
  l->color_sky_top    = vec4(0.35f,0.55f,0.75f,1.0);
  l->color_sky_bottom = vec4(0.35f,0.55f,0.75f,1.0);
	l->color_fire_start = vec4(1.0,0.784,0.1804,1.0);
	l->bottom_pass			= false;
  add_layout(l,"Default");

//  l = new Layout();
//  l->color_sea_top    = vec4(0.25f,0.4f,0.55f,1.0f);
//  l->color_sea_middle = vec4(0.25f,0.4f,0.55f,0.5f);
//  l->color_sea_bottom = vec4(0.1f,0.4f,0.3f,0.5f);
//  l->color_sky_top    = vec4(119/255.0f,125/255.0f,199/255.0f,0.0);
//  l->color_sky_bottom = vec4(248/255.0f,83/255.0f,19/255.0f,0.0);
//	l->color_fire_start = vec4(1.0,0.784,0.1804,1.0);
//	l->bottom_pass			= true;
//  add_layout(l,"Dawn");

  l = new Layout();
  l->color_sea_top    = vec4(0.25f,0.4f,0.55f,1.0);
  l->color_sea_middle = vec4(0.25f,0.4f,0.55f,0.5);
  l->color_sea_bottom = vec4(0.25f,0.4f,0.55f,0.5);
  l->color_sky_top    = vec4(0.35f,0.55f,0.75f,1.0);
  l->color_sky_bottom = vec4(0.35f,0.55f,0.75f,1.0);
	l->color_fire_start = vec4(0.0,0.0,0.0,1.0);
	l->bottom_pass			= false;
  add_layout(l,"Clear");

  l = new Layout();
  l->color_sea_top    = vec4(0.55f,0.55f,0.55f,1.0);
  l->color_sea_middle = vec4(0.55f,0.55f,0.55f,0.5);
  l->color_sea_bottom = vec4(0.33f,0.33f,0.33f,0.5);
  l->color_sky_top    = vec4(0.0,0.0,0.0,1.0);
  l->color_sky_bottom = vec4(1.0,1.0,1.0,1.0);
	l->color_fire_start = vec4(1.0,1.0,1.0,1.0);
	l->bottom_pass			= true;
  add_layout(l,"B&W");

  // default
  set_layout("Default");
}

void Layouts::set_layout(const string &name)
{
  m_current_layout = m_layouts[name];
}

Layout *Layouts::cur(void)
{
  return m_current_layout;
}

str_ary_t Layouts::get_names(void)
{
  return m_layout_names;
}
