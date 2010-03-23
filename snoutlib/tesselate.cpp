
#include "tesselate.h"

vec2_ary_t Tesselate::m_verts;

void APIENTRY cb_begin( GLenum type )
{
  assert(type == GL_TRIANGLES);
}


void APIENTRY cb_end(void) {}


void APIENTRY cb_error( GLenum errno )
{
  printf("GLU tess error: %i\n",errno);
  abort();
}

void APIENTRY cb_edgeflag(GLboolean flag) {}

void APIENTRY cb_combine( GLdouble coords[3], void *vertex_data[4], GLfloat weight[4], void **outData)
{
  double *g_v = new double[3];
  g_v[0] = coords[0];
  g_v[1] = coords[1];
  g_v[2] = coords[2];
  *outData = g_v;
}

void APIENTRY cb_vertex( double *vertex )
{
  vec2 p((float)vertex[0],(float)vertex[1]);
  Tesselate::m_verts.push_back(p);
}

#ifdef WIN32
#  define TESSCBCAST (void (APIENTRY *)(void))
#else
#  define TESSCBCAST (APIENTRY void (*)())
#endif

vec2_ary_t Tesselate::Process(pathset_t contours)
{
  m_verts.clear();
  GLUtesselator* tess = gluNewTess();

  gluTessCallback(tess, GLU_TESS_BEGIN,TESSCBCAST cb_begin);
  gluTessCallback(tess, GLU_TESS_END, TESSCBCAST cb_end);
  gluTessCallback(tess, GLU_TESS_ERROR, TESSCBCAST cb_error);
  gluTessCallback(tess, GLU_TESS_VERTEX, TESSCBCAST cb_vertex);
  gluTessCallback(tess, GLU_TESS_EDGE_FLAG, TESSCBCAST cb_edgeflag);
  gluTessCallback(tess, GLU_TESS_COMBINE, TESSCBCAST cb_combine);

  gluTessBeginPolygon(tess,NULL);
  for (pathset_t::const_iterator c_it=contours.begin();c_it!=contours.end();++c_it) {
    vec2_ary_t contour = *c_it;
    gluTessBeginContour(tess);
    for (vec2_ary_t::iterator p_it=contour.begin();p_it!=contour.end();++p_it) {
      vec2 point = *p_it;
      double *v = new double[3]; // FIXME: memory leak
      v[0] = point[0];
      v[1] = point[1];
      v[2] = 0.0;

//      printf("%.8f,%.8f\n",v[0],v[1]);
      gluTessVertex(tess,v,v);

    }
    gluTessEndContour(tess);
  }

  gluTessEndPolygon(tess);
  gluDeleteTess(tess);

  return m_verts;
}
