#pragma once

#include "misc.h"
#include "procedural.h"

using namespace std;

typedef pair<char,vec2_ary_t> path_token_t; // command, points
typedef vector<path_token_t> path_t;

typedef vector<vec2_ary_t> pathset_t;

class SVGpath
{
  int m_curve_segments;
  pathset_t m_pathset;
  
  pathset_t path_to_vertices(path_t path);
  void normalize_pathset(void);

public:
  SVGpath(path_t path,bool normalize, int curve_segments = 4);

  pathset_t get_pathset(void);
};
