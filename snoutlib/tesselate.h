#pragma once

#include "gldefs.h"

#include <assert.h>
#include <stdlib.h>
#include "svg.h"

class Tesselate
{
public:
  static vec2_ary_t m_verts;
  static vec2_ary_t Process(pathset_t contours);
};

