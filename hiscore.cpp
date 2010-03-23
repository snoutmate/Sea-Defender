
#include "snoutlib/mfont.h"
#include "hiscore.h"
#include "resources.h"

HSitem::HSitem(const string& name, int score, bool builtin) :
	m_name(name),m_score(score),m_builtin(builtin)
{}

HSitem::HSitem() {}


HiScore::HiScore(const string& fname) :
  m_fname(fname)
{
  for(int i=0;i<NUMSCORES;++i)
    m_scores[i] = HSitem("",0,true);

  add_highscore("sigflup",1000,true);
  add_highscore("texel",2000,true);
  add_highscore("mfx",3000,true);
  add_highscore("Farbrausch",4000,true);
  add_highscore("BitFellas",5000,true);
  add_highscore("Tigrou",6000,true);
  add_highscore("Navis",7000,true);
  add_highscore("rrrola",8000,true);
  add_highscore("SQNY",9000,true);
  add_highscore("iq/RGBA",10000,true);

  read_from_file();
}

void HiScore::draw(void)
{
	MFont *font = g_resources.font;

  font->setup_drawing();

  float width=0.8;
  float start = (1.6 - width) / 2.0;
  float end = start + width;
  float y_start = 0.38;
  float y_step = 0.05;
  float font_size = 1.0;
  float dotsize = font->size_of_text(".",font_size);
  vec4 color;

	font->print_text_with_shadow("High scores", vec2(0.0+0.002,0.29+0.003),1.5,true,vec4(1.0,1.0,0.0,1.0));

  for(int i=0;i<NUMSCORES;++i) {
    char line[256];
    char score[64];
    strcpy(line,m_scores[i].m_name.c_str());
    sprintf(score,"%i",m_scores[i].m_score);

    float textsize = font->size_of_text(line,font_size) + font->size_of_text(score,font_size);


    int dots = (width - textsize) / dotsize;

    for (int j=0;j<dots;++j)
      strcat(line,".");

    color = vec4(1.0-i/9.0,1.0,i/9.0,1.0);

    float y_pos = y_start + y_step*i;

			font->print_text(line,	vec2(start,y_pos),font_size,false,color);
			font->print_text(score,	vec2(end-font->size_of_text(score,font_size),y_pos),font_size,false,color);
  }


  font->finish_drawing();
}

void xor_buffer(unsigned char *buf,int size,unsigned char xorvalue)
{
  for(int i=0;i<size;++i)
    buf[i] ^= xorvalue;
}

int calc_crc(unsigned char *buf,int size)
{
  int crc=0;
  for(int i=0;i<size;++i)
    crc ^= buf[i];
  
  return crc;
}

void HiScore::read_from_file(void)
{
  FILE *f = fopen(m_fname.c_str(),"rb");

  if (f==NULL)
    return;

  const int bsize = (16+4)*NUMSCORES;
  unsigned char buffer[bsize];
  memset(buffer,0,bsize);

  int readbytes = fread(buffer,1,bsize,f);
  fclose(f);

  xor_buffer(buffer,readbytes,XORVALUE);

  int pos=0;
  for(int i=0;i<NUMSCORES;++i) {
    int score=0;
    char name[17] = {0};
    memcpy(name,buffer+pos,16);
    pos+=16;
    score += buffer[pos++];
    score += (buffer[pos++]) <<8;
    score += (buffer[pos++]) <<16;
    int stored_crc = buffer[pos++];
    int crc = calc_crc(buffer+pos-16,15);

    if (crc!=stored_crc)
      continue;

    add_highscore(name,score,false);
  }
  
}

void HiScore::save_to_file(void)
{
  const int bsize = (16+4)*NUMSCORES;
  unsigned char buffer[bsize];
  memset(buffer,0,bsize);

  int pos = 0;
  for(int i=0;i<NUMSCORES;++i) {
    if (m_scores[i].m_builtin==true)
      continue;

    int score = m_scores[i].m_score;
    sprintf((char *)buffer+pos,"%s",m_scores[i].m_name.c_str()); // %16s
    pos += 16;
    buffer[pos++] = score & 0xFF;
    buffer[pos++] = (score >> 8) & 0xFF;
    buffer[pos++] = (score >> 16) & 0xFF;

    int crc=calc_crc(buffer+pos-15,15);
    buffer[pos++] = crc;
  }

  if (pos<1) // nothing to write
    return;

  xor_buffer(buffer,pos,XORVALUE);

  // write
  FILE *f = fopen(m_fname.c_str(),"wb");
  if (f==NULL) // can't write ? silent fail
    return;

  fwrite(buffer,pos,1,f);
  fclose(f);
}

bool HiScore::is_highscore(int score)
{
  if (m_scores[NUMSCORES-1].m_score>score)
    return false;
  else
    return true;
}

void HiScore::add_highscore(const string& name, int score, bool builtin)
{
  HSitem myscore(name,score,builtin);

  for(int i=0;i<NUMSCORES;++i) {
    if (score > m_scores[i].m_score) {
      for(int j=NUMSCORES-1;j>i;j--)
        m_scores[j] = m_scores[j-1];
      m_scores[i] = myscore;
      break;
    }
  }
}

string HiScore::first_nonbuiltin_name(void)
{
  for(int i=0;i<NUMSCORES;++i) {
    if (m_scores[i].m_builtin==false)
      return m_scores[i].m_name;
  }

  return string("");  
}
