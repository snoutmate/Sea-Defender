
#include "svg.h"

SVGpath::SVGpath(path_t path,bool normalize, int curve_segments) :
  m_curve_segments(curve_segments),m_pathset()
{
  m_pathset = path_to_vertices(path);
  if (normalize)
    normalize_pathset();
}

pathset_t SVGpath::path_to_vertices(path_t path)
{
  vec2 cur_point;
  vec2 start;

  vec2_ary_t verts;
  pathset_t curves;

  vec2 last_cp;
  bool last_cp_set = false;

  for(path_t::const_iterator it=path.begin();it<path.end();++it) {
    path_token_t token = *it;
    char command = token.first;
    vec2_ary_t data = token.second;

    // for "T" shorthand Q curves use last control point, otherwise reset it to NULL
    switch(command) {
      case 't':
      case 'T':
      case 'q':
      case 'Q':
        // nop 
        break;
      default:
        last_cp_set = false;
        break;
    }

    // main switch, SVG path ouline => outline vertices list,
    //  curves are broken into [m_curve_segments] parts
    switch(command) {
      case 'M': // moveto - start of curve/contour
      case 'm': // moveto in relative coords (no change from 'M')
        if (!verts.empty())
          curves.push_back(verts);
        verts.clear();
        verts.push_back(data[0]);
        cur_point = data[0];
        start = cur_point;
        break;
      case 'L': // line to
        verts.push_back(data[0]);
        cur_point = data[0];
        break;
      case 'l': // line to, in relative coordinates
        verts.push_back(data[0]);
        cur_point = data[0] + cur_point;
        break;
      case 'V': // vertical line from current point
        {
          vec2 v(cur_point[0],data[0][0]);
          verts.push_back(v);
          cur_point = v;
        }
        break;
      case 'H': // horizontal line from current point
        {
          vec2 v(data[0][0],cur_point[1]);
          verts.push_back(v);
          cur_point = v;
        }
        break;
      case 'C': // cubic curve
        {
          vec2_ary_t curve = Procedural::bezier_cubic_curve(cur_point,data[0],data[1],data[2],m_curve_segments);
          cur_point = curve.back();
          copy(curve.begin(), curve.end(), back_inserter(verts));
        }
        break;
      case 'Q': // quadratic curve
        {
          vec2_ary_t curve = Procedural::bezier_quadratic_curve(cur_point,data[0],data[1],m_curve_segments);
          cur_point = curve.back();
          last_cp = data[0];
          last_cp_set = true;
          copy(curve.begin(), curve.end(), back_inserter(verts));
        }
        break;
      case 'T': // continue quadratic curve, with control point being reflection of the last CP
        {
          vec2 new_cp;
          if (last_cp_set) {
            // reflect the last CP
            new_cp = cur_point + (cur_point - last_cp);
          } else {
            new_cp = cur_point;
          }
          vec2_ary_t curve = Procedural::bezier_quadratic_curve(cur_point,new_cp,data[0],m_curve_segments);
          cur_point = curve.back();
          copy(curve.begin(), curve.end(), back_inserter(verts));
          last_cp = new_cp;
        }
        break;
      case 'Z':
      case 'z':
        cur_point = start;
        break;
      default:
        printf("unhandled SVG path command '%c'",command);
        break;
    }
  }

  // collect the last pass, if any
  if (!verts.empty())
    curves.push_back(verts);

  return curves;
}

// whole path centered to 0,0 and mapped into unit square
void SVGpath::normalize_pathset(void)
{
  vec2 min_p = m_pathset[0][0];
  vec2 max_p = m_pathset[0][0];

  for(pathset_t::const_iterator it=m_pathset.begin();it!=m_pathset.end();++it) {
    vec2_ary_t path = *it;
    for(vec2_ary_t::const_iterator p_it=path.begin();p_it!=path.end();++p_it) {
      vec2 p = *p_it;
      if (p[0] < min_p[0]) min_p[0] = p[0];
      if (p[0] > max_p[0]) max_p[0] = p[0];
      if (p[1] < min_p[1]) min_p[1] = p[1];
      if (p[1] > max_p[1]) max_p[1] = p[1];
    }
  }

  vec2 mid_p = (min_p + max_p) / 2.0;

  float xsize = fabs(max_p[0] - min_p[0]);
  float ysize = fabs(max_p[1] - min_p[1]);

  float divisor = (xsize>ysize) ? xsize : ysize;

  pathset_t pathset_new;
  // normalize points
  for(pathset_t::const_iterator it=m_pathset.begin();it!=m_pathset.end();++it) {
    vec2_ary_t path = *it;
    for(vec2_ary_t::iterator p_it=path.begin();p_it!=path.end();++p_it) {
      vec2 p = *p_it;
      *p_it = (p - mid_p) / divisor;
    }
    pathset_new.push_back(path);
  }
  m_pathset.swap(pathset_new);
}

pathset_t SVGpath::get_pathset(void)
{
  return m_pathset;  
}
