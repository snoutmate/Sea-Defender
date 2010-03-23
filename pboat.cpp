
#include "pboat.h"
#include "background.h"
#include "resources.h"

const int pboat_missiles = 10;

PBoat::PBoat(vec2 pos,float scale) :
  m_pos(pos),m_scale(scale),m_shift(0.0),m_missiles(pboat_missiles)
{
  m_model = &g_resources.mesh_pboat;
}

void PBoat::draw(void)
{
	glPushMatrix();

  m_shift = Sea::sea_func(m_pos[0]+0.00) * 0.90; // pos
  float rot = Sea::sea_func(m_pos[0]+0.05) * 0.9 * 333; // rotate

  glTranslatef(m_pos[0],m_pos[1]+m_shift,0.0);
  glScalef(m_scale,m_scale,1.0);
  glRotatef(rot,0,0,1);

  glDisable(GL_TEXTURE_2D);
  glColor4f(0,0,0,1.0);

	(*m_model)->draw();

	glPopMatrix();

  glPushMatrix();

  StaticMesh *missile_model = g_resources.mesh_missile;
  glTranslatef(m_pos[0]-0.05,m_pos[1]-0.06,0.0);
  glScalef(Missile::m_scale,Missile::m_scale,1.0);
  glRotatef(90,0,0,1.0);
  glColor4f(0,0,0,1.0);
  for(int i=0;i<10;++i) {
    if (i==m_missiles)
      glColor4f(1.0,1.0,1.0,0.25);

    missile_model->draw();
    glTranslatef(0.0,-0.3,0.0);
  }

  glPopMatrix();

}

vec2 PBoat::get_pos(void)
{
  return vec2(m_pos[0],m_pos[1]+m_shift);
}

int PBoat::missiles_left(void)
{
  return m_missiles;
}

Missile *PBoat::fire_missile(vec2 dest,float radius)
{
  --m_missiles;
  vec2 src = m_pos + vec2(0.0,m_shift + 0.02);
  return new Missile(src,dest,radius);
}

void PBoat::reload_ammo(void)
{
  m_missiles = pboat_missiles;
}
