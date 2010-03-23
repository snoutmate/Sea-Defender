
#include "staticmesh.h"
#include "rtt_fbo.h"
#include "../resources.h"


StaticMesh::StaticMesh(const unsigned char* data,unsigned int data_size) :
  m_vertexcount(0),m_vbo(0),m_texture(0),m_vbo_data(),m_data_ptr(0),m_data(data),m_data_size(data_size)
{
  load();
  build_vbo();
	render_to_texture(g_resources.texture_mesh_size);
}

StaticMesh::~StaticMesh()
{
	if (m_texture)
		glDeleteTextures(1,&m_texture);
}

void StaticMesh::render_to_texture(int t_size)
{
  RTT *m_rtt = new RTT_FBO(1,t_size,t_size);

  m_rtt->start_render();

  glClearColor(0.0,0.0,0.0,0.0);

  glViewport(0,0,t_size,t_size);
  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
  //glOrtho(0.0,1.0, 0.0,1.0, -1,1);
  glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
	glScalef(1.95,1.95,1.0);

	draw();
	
  m_rtt->end_render();
  m_texture = m_rtt->get_texid();

	delete m_rtt;

  m_texture = move_texture_red_component(m_texture);

  glMatrixMode(GL_PROJECTION); glPopMatrix();
  glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

void StaticMesh::load(void)
{
  path_t path = read_path();

  // create outline
  SVGpath svgpath = SVGpath(path,true,m_segments);
  pathset_t pathset = svgpath.get_pathset();

  // tesselate into mesh
  vec2_ary_t verts = Tesselate::Process(pathset);
  m_vertexcount = verts.size();

  // copy
  m_vbo_data = vec2_ary_to_float_ary(verts);

	m_aabb = AABB(verts);
}


void StaticMesh::build_vbo(void)
{
  my_glGenBuffers(1,&m_vbo);
  my_glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  my_glBufferData(GL_ARRAY_BUFFER, m_vbo_data.size()*sizeof(float), &m_vbo_data[0], GL_STATIC_DRAW);
  my_glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//public
void StaticMesh::draw(void)
{
  if (m_texture!=0) {
    g_resources.shader_staticmesh->use();

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glScalef(1.02,1.02,1.00); // adjust for texture border
    draw_textured_quad(1.0,m_texture,true);

		glPopMatrix();

    g_resources.shader_staticmesh->unuse();

//		m_aabb.draw();
	} else {
		my_glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(2,GL_FLOAT,0,0);
	
		glDrawArrays(GL_TRIANGLES,0,m_vertexcount);
	
		glDisableClientState(GL_VERTEX_ARRAY);
		my_glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}

float StaticMesh::get_num(void)
{
  const unsigned short *num = reinterpret_cast<const unsigned short*>(&m_data[m_data_ptr]);
  m_data_ptr += 2; // 16-bit
  return ((float)*num / m_divisor);
}

vec2 StaticMesh::get_vec2(void)
{
  float x = get_num();
  float y = get_num();
  return vec2(x,y);
}

int StaticMesh::get_byte(void)
{
  const unsigned char num = m_data[m_data_ptr];
  m_data_ptr++;
  return num;
}

path_t StaticMesh::read_path(void)
{
  path_t path;
  while(true) {
    vec2_ary_t data;

    if (m_data_ptr >= m_data_size)
      break;

    char cmd = get_byte();
    
    switch(cmd) {
    case 'H':
    case 'V':
      data.push_back(vec2(get_num(),0.0));
      break;
    case 'C':
      data.push_back(get_vec2());
			// fallthrough
    case 'Q':
      data.push_back(get_vec2());
			// fallthrough
    case 'M':
    case 'L':
    case 'T':
      data.push_back(get_vec2());
      break;
    case 'Z':
    case 'z':
      // nop
      break;
    default:
      assert(false);
    }
    path_token_t pt(cmd,data);  
    path.push_back(pt);
  }
  return path;
}
