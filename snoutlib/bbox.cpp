#include "bbox.h"

AABB::AABB() :
	x1(0),x2(0),y1(0),y2(0)
{}

AABB::AABB(float _x1,float _x2,float _y1,float _y2) :
	x1(_x1),x2(_x2),y1(_y1),y2(_y2)
{}

AABB::AABB(vec2_ary_t const& verts)
{
  x1 = verts[0][0];
	x2 = verts[0][0];
  y1 = verts[0][1];
	y2 = verts[0][1];

	for(int i=0,size=verts.size();i<size;++i)
	{
		vec2 const& p = verts[i];
		if (p[0] < x1) x1 = p[0];
		if (p[0] > x2) x2 = p[0];
		if (p[1] < y1) y1 = p[1];
		if (p[1] > y2) y2 = p[1];
  }
}

void AABB::draw(void)
{
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  glPushMatrix();
  glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glColor3f(0,1,0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x1,y1);
	glVertex2f(x2,y1);
	glVertex2f(x2,y2);
	glVertex2f(x1,y2);
  glEnd();

  glPopAttrib();
  glPopMatrix();
}


bool AABB::contains_point(const vec2 p)
{
  return (p[0]>x1 && p[0]<x2 && p[1]>y1 && p[1]<y2);
}

//

bool OBB::overlaps1Way(const OBB& other) const
{
  for (int a = 0; a < 2; ++a) {
    double t = dot(other.corner[0],axis[a]);

    // Find the extent of box 2 on axis a
    double tMin = t;
    double tMax = t;

		for (int c = 1; c < 4; ++c) {
			t = dot(other.corner[c],axis[a]);

			if (t < tMin) {
				tMin = t;
			} else if (t > tMax) {
				tMax = t;
			}
		}

    // We have to subtract off the origin

    // See if [tMin, tMax] intersects [0, 1]
		if ((tMin > 1 + origin[a]) || (tMax < origin[a])) {
			// There was no intersection along this dimension;
			// the boxes cannot possibly overlap.
			return false;
		}
  }

  // There was no dimension along which there is no intersection.
  // Therefore the boxes overlap.
  return true;
}



void OBB::computeAxes(void)
{
  axis[0] = corner[1] - corner[0];
  axis[1] = corner[3] - corner[0];

  // Make the length of each axis 1/edge length so we know any
  // dot product must be less than 1 to fall within the edge.
  for (int a = 0; a < 2; ++a) {
    axis[a] /= length2(axis[a]);
		origin[a] = dot(corner[0],axis[a]);
  }
}

OBB::OBB(const AABB& aabb)
{
	corner[0] = vec2(aabb.x1,aabb.y1);
	corner[1] = vec2(aabb.x2,aabb.y1);
	corner[2] = vec2(aabb.x2,aabb.y2);
	corner[3] = vec2(aabb.x1,aabb.y2);

	// rotate by current matrix
	mat4 mt = glGetCurrentMatrix(GL_MODELVIEW_MATRIX);
  for (int c = 0; c < 4; ++c)
		corner[c] = vec2(transformPoint(mt,vec3(corner[c])));
	
  computeAxes();
}

/** Returns true if the intersection of the boxes is non-empty. */
bool OBB::overlaps(const OBB& other) const
{
  return (overlaps1Way(other) && other.overlaps1Way(*this));
}

bool OBB::overlaps(const Circle& other) const
{
  if (CircleLineIntersect(other,Line(corner[0],corner[1]))) return true;
  if (CircleLineIntersect(other,Line(corner[1],corner[2]))) return true;
  if (CircleLineIntersect(other,Line(corner[2],corner[3]))) return true;
  if (CircleLineIntersect(other,Line(corner[3],corner[0]))) return true;
  return false;
}


void OBB::draw(bool highlight) const
{
	// OBB cords are in worldspace
  glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	if (highlight)
		glColor3f(1,0,0);
	else
		glColor3f(0,1,0);

  glBegin(GL_LINE_LOOP);
  for (int c = 0; c < 4; ++c)
    glVertex(corner[c]);

  glEnd();
	glPopMatrix();
}

bool CircleLineIntersect(const Circle& circle, const Line& line)
{
  vec2 ac = circle.pos - line.p1;
  vec2 ab = line.p2 - line.p1;
  float t = dot(ac,ab) / dot(ab,ab);

  t = clamp_0_1(t);

  vec2 h = line.p1 + t * ab - circle.pos;

  return (dot(h,h) <= (circle.r * circle.r));
}
