#pragma once

#include "snoutlib/misc.h"

struct Layout
{
  Layout() {}
  vec4 color_sea_top;
  vec4 color_sea_middle;
  vec4 color_sea_bottom;
  vec4 color_sky_top;
  vec4 color_sky_bottom;
	vec4 color_fire_start;
	bool bottom_pass;
};

typedef map<string,Layout *> layouts_t;

class Layouts
{
  Layout *m_current_layout;
  layouts_t m_layouts;
  str_ary_t m_layout_names;

  void add_layout(Layout *l,const string &name);
  void setup_layouts(void);
public:
  Layouts();
  ~Layouts();

  Layout *cur(void); // returns current layout
  void set_layout(const string &name);
  str_ary_t get_names(void);
};
