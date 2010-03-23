#ifdef WIN32
#  include <windows.h>
#endif

#include <assert.h>

#include "gldefs.h"

#include "mfont.h"
#include "bbox.h"

#include "../resources.h"

using namespace std;

Glyph::Glyph(float advance,vec2_ary_t verts) :
  m_advance(advance), m_verts(verts)
{}

Glyph::Glyph(void) :
  m_advance(0.0), m_verts()
{}

MFont::MFont(const unsigned char *data,float prescale) :
  m_texture_chars_per_row(10),m_texture_step(1.0f/m_texture_chars_per_row),
	m_divisor(65535.0f), m_start_char(32), m_end_char(127),
  m_prescale(prescale), m_line_sep(0), m_ascent(0), m_origin_shift(0), m_horiz_advance(0),
  m_em_scale(0), m_vbo(0),m_vbo_data(),m_glyphs(),m_data_ptr(0),m_data(data),
	m_texture(0),m_external_draw(false)
	
{
  read_from_data();

	create_texture();
  build_vbo();

  m_line_sep = m_ascent;
}

MFont::~MFont()
{
	if (m_texture)
		glDeleteTextures(1,&m_texture);	
}

float MFont::line_sep(void) { return m_line_sep * m_prescale; }

float MFont::size_of_text(const char *text,float scale)
{
  float size=0;

  while(*text)
    size += get_advance(*text++);
  
  return size * scale * m_prescale;
}

inline float MFont::get_advance(char ch)
{
  return m_glyphs[ch - m_start_char].m_advance;
}

AABB MFont::bbox(const char* text, float scale,const vec2 &pos,bool centered)
{
  float tsize = size_of_text(text,scale);
  float x1 = pos[0] + m_origin_shift * m_prescale * scale;
  if (centered) {
    x1 += center_text_pos(text,scale,1.6);
  }
  float y1 = pos[1];
  float x2 = x1 + tsize;
  float y2 = y1 + line_sep() * scale;
  
  return AABB(x1,x2,y1,y2);
}

void MFont::setup_drawing(void)
{
	m_external_draw = true;
	start_drawing();
}
void MFont::finish_drawing(void)
{
	stop_drawing();
	m_external_draw = false;
}

float MFont::center_text_pos(const char *text, float scale,float width)
{
  float size = size_of_text(text,scale);
  return (width-size)/2.0f;
}

void MFont::print_text(const char *text, vec2 const &pos, float scale,bool centered,vec4 const &color)
{
  float cx=0.0;
  if (centered)
    cx = center_text_pos(text,scale);

  glPushMatrix();
 
	if (!m_external_draw)
		start_drawing();

//  my_glUniform4fv(m_shader->get_uni_loc("time"),time);
  glColor(color);

  glTranslatef(pos[0]+cx,pos[1],0.0);
  set_scale(scale);

//  glTranslatef(-m_origin_shift,0.0,0.0);

  while(*text) 
    print_char(*text++);

	if (!m_external_draw)
	  stop_drawing();

	glPopMatrix();
}

void MFont::print_text_with_shadow(const char *text, vec2 const &pos, float scale,bool centered,vec4 const &color)
{
  print_text(text,pos+vec2(0.002,0.002),scale,centered,vec4(0.0,0.0,0.0,color[3]));
  print_text(text,pos,scale,centered,color);

  //AABB bb = bbox(text,scale,pos,centered);
  //bb.draw();
}


void MFont::set_scale(float scale)
{
  scale *= m_prescale;
  glScalef(scale,scale,1.0);
}

void MFont::print_char(char ch)
{
  if (ch == '\n') {
    // FIXME:
    //glPopMatrix();
    //glTranslatef(0.0,line_sep(),0.0);
    //glPushMatrix();
    return;
  }

	unsigned int char_n = ch - m_start_char;
	
  glDrawArrays(GL_QUADS,char_n*4,4);

  glTranslatef(m_glyphs[char_n].m_advance,0,0);
}


void MFont::start_drawing(void)
{
  glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_texture);
	
  glEnable(GL_BLEND);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  my_glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glVertexPointer(2,GL_FLOAT,8*2,0);
  glTexCoordPointer(2,GL_FLOAT,8*2,(void *) (4*2));

  g_resources.shader_font->use();
}

void MFont::stop_drawing(void)
{
  g_resources.shader_font->unuse();

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);

  my_glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
}

void MFont::build_vbo(void)
{
	unsigned int datapos = 0;
	float *data = new float [(m_end_char - m_start_char)*(4*2+4*2)]; // 4 verts, 4 texcoords

	for (int ch = 0;ch<(m_end_char-m_start_char);++ch) {
		float border_a = 0.0;
		float border_b = 0.0;
	
		unsigned int x = ch % m_texture_chars_per_row;
		unsigned int y = ch / m_texture_chars_per_row;
		float x1 = x * m_texture_step + m_texture_step*border_a;
		float y1 = y * m_texture_step + m_texture_step*border_a;
		float x2 = (x+1) * m_texture_step - m_texture_step*border_b;
		float y2 = (y+1) * m_texture_step - m_texture_step*border_b;
	
		data[datapos++] = border_a;   data[datapos++] = border_a;
		data[datapos++] = x1; 			  data[datapos++] = y1;

		data[datapos++] = 1-border_b; data[datapos++] = border_a;
		data[datapos++] = x2; 			  data[datapos++] = y1;

		data[datapos++] = 1-border_b; data[datapos++] = 1-border_b;
		data[datapos++] = x2; 			  data[datapos++] = y2;

		data[datapos++] = border_a;   data[datapos++] = 1-border_b;
		data[datapos++] = x1; 			  data[datapos++] = y2;
	}

	my_glGenBuffers(1,&m_vbo);
	my_glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	my_glBufferData(GL_ARRAY_BUFFER, datapos*sizeof(float), data, GL_STATIC_DRAW);
	my_glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] data;
}

// TODO: presunout read() do create_texture


void MFont::create_texture(void)
{
  int t_size = g_resources.texture_font_size;
	float correct_y = m_texture_step * -0.1;

  RTT *m_rtt = new RTT_FBO(1,t_size,t_size);

  m_rtt->start_render();

  glClearColor(0.0,0.0,0.0,0.0);

  glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

  glViewport(0,0,t_size,t_size);

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION); glLoadIdentity();
  glOrtho(0.0,1.0, 0.0,1.0, -1,1);
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glColor4f(1,1,1,1);

	for (unsigned int i=0;i<m_glyphs.size();++i) {
		unsigned int x = i % m_texture_chars_per_row;
		unsigned int y = i / m_texture_chars_per_row;
		float xpos = x * m_texture_step;
		float ypos = (y + 1) * m_texture_step; // shift the row by one, as origin of each letter is inverted 

		vec2_ary_t& verts = m_glyphs[i].m_verts;

		if (verts.empty())
			continue;
	
		glTranslatef(xpos,ypos+correct_y,0.0);
		glScalef(m_texture_step,-m_texture_step,1.0); // inverted, viz above

		glBegin(GL_TRIANGLES);
		for (unsigned int vi = 0; vi<verts.size(); ++vi) {
			glVertex(verts[vi]);
		}
		glEnd();

		glLoadIdentity();
	}

  m_rtt->end_render();

  m_texture = m_rtt->get_texid();

	delete m_rtt;

  g_loadingscreen->update();

  m_texture = move_texture_red_component(m_texture);

  g_loadingscreen->update();
}


// read


void MFont::read_from_data(void) {
  m_horiz_advance = get_num();
  m_em_scale = get_num();
  m_ascent = get_num();
  m_origin_shift = get_num();

  for(int i=m_start_char;i<=m_end_char;i++) {
    float advance = get_num();
    path_t path = read_path();

    if (path.empty()) {
      Glyph glyph(advance);
      m_glyphs.push_back(glyph);
    } else {
      // create outline
      SVGpath svgpath = SVGpath(path,false,32);
      pathset_t pathset = svgpath.get_pathset();

      // tesselate into mesh
      vec2_ary_t verts = Tesselate::Process(pathset);

      Glyph glyph(advance,verts);
      m_glyphs.push_back(glyph);

      g_loadingscreen->update();
    }
  }
}

float MFont::get_num(void)
{
  const unsigned short *num = reinterpret_cast<const unsigned short*>(&m_data[m_data_ptr]);
  m_data_ptr += 2; // 16-bit
  return ((float)*num / m_divisor);
}

vec2 MFont::get_vec2(void)
{
  float x = get_num();
  float y = get_num();
  return vec2(x,y);
}

int MFont::get_byte(void)
{
  const unsigned char num = m_data[m_data_ptr];
  m_data_ptr++;
  return num;
}

path_t MFont::read_path(void)
{
  path_t path;
  while(true) {
    vec2_ary_t data;

    char cmd = get_byte();
    if (cmd=='\n')
      break;
    
    switch(cmd) {
    case 'M':
    case 'T':
    case 'L':
      data.push_back(get_vec2());
      break;
    case 'H':
    case 'V':
      data.push_back(vec2(get_num(),0.0));
      break;
    case 'C':
      data.push_back(get_vec2());
      data.push_back(get_vec2());
      data.push_back(get_vec2());
      break;
    case 'Q':
      data.push_back(get_vec2());
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
