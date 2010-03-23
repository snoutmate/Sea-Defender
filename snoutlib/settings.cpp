#include <stdio.h>
#include <assert.h>
#include "settings.h"

Settings::Settings(void) :
  m_res_x(1024), m_res_y(768),
  m_fullscreen(false),
  m_fsaa(0), m_vsync(false),
  m_mouse_sensitivity(6),
  m_fname("")
{
}

Settings::~Settings() // save on exit
{
  if (!m_fname.empty())
    save_to_file();
}

void Settings::read_from_file(const string &filename)
{
  FILE *f = fopen(filename.c_str(),"rb");
  m_fname = filename;

  if (f==NULL)
    return;

  int cnt = 0;
  cnt = fread(&m_res_x,4,1,f); assert (cnt==1);
  cnt = fread(&m_res_y,4,1,f); assert (cnt==1);
  cnt = fread(&m_fsaa,4,1,f); assert (cnt==1);
  cnt = fread(&m_vsync,1,1,f); assert (cnt==1);
  cnt = fread(&m_fullscreen,1,1,f); assert (cnt==1);
  cnt = fread(&m_mouse_sensitivity,4,1,f); assert (cnt==1);
  fclose(f);

}

void Settings::save_to_file(void)
{
  FILE *f = fopen(m_fname.c_str(),"wb");

  if (f==NULL)
    return;

  fwrite(&m_res_x,4,1,f);
  fwrite(&m_res_y,4,1,f);
  fwrite(&m_fsaa,4,1,f);
  fwrite(&m_vsync,1,1,f);
  fwrite(&m_fullscreen,1,1,f);
  fwrite(&m_mouse_sensitivity,4,1,f);
  fclose(f);
}
