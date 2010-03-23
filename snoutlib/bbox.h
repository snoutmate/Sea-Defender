#pragma once

#include "misc.h"

struct Circle
{
  vec2 pos;
  float r;
  Circle(vec2 _pos,float _r) : pos(_pos), r(_r) {}
};

struct Line
{
  vec2 p1,p2;
  Line(vec2 _p1,vec2 _p2) : p1(_p1), p2(_p2) {}
};

// axis aligned bounding box
struct AABB
{
	float x1,x2,y1,y2;
	AABB();
	AABB(float _x1,float _x2,float _y1,float _y2);
	AABB(vec2_ary_t const& verts); // compute from array of vertices

  bool contains_point(const vec2 p);

	void draw(void);
};

// 2d oriented bounding box class for fast collisions, adapted from flipcode code-dump
class OBB {
	vec2 corner[4];
  vec2 axis[2];
  double origin[2];

  bool overlaps1Way(const OBB& other) const;
  void computeAxes(void);

public:
  OBB(const AABB& aabb); // takes AABB and rotates it with current modelview matrix

  bool overlaps(const OBB& other) const;
  bool overlaps(const Circle& other) const;

	void draw(bool highlight=false) const; // for debugging collisions
};

bool CircleLineIntersect(const Circle& circle, const Line& line);
