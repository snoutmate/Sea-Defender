
#include "resources.h"

#include "data/res/kronika.mfont.h"

#include "data/res/torpedo.m.h"
#include "data/res/torpedo2.m.h"
#include "data/res/pboat.m.h"
#include "data/res/tanker.m.h"
#include "data/res/OpenGL_logo_flat.m.h"

#include "data/res/seawaves.vs.h"
#include "data/res/clouds.vs.h"
#include "data/res/clouds.fs.h"
#include "data/res/font.fs.h"
#include "data/res/particles.fs.h"
#include "data/res/particles.vs.h"
#include "data/res/staticmesh.fs.h"
#include "data/res/line.fs.h"

Resources g_resources;

void Resources::load(void)
{
	shader_seawaves = new Shader(data_sha_seawaves_vs,NULL);  g_loadingscreen->update();
	shader_clouds =	new Shader(data_sha_clouds_vs,data_sha_clouds_fs);   g_loadingscreen->update();
	shader_particles = new Shader(data_sha_particles_vs,data_sha_particles_fs);   g_loadingscreen->update();

  my_glBindAttribLocation(shader_particles->get_prog(),0,"pos");
  my_glBindAttribLocation(shader_particles->get_prog(),1,"corner");
  my_glBindAttribLocation(shader_particles->get_prog(),2,"size");
  my_glBindAttribLocation(shader_particles->get_prog(),3,"color");

	shader_particles->relink();

	shader_font = new Shader(NULL,data_sha_font_fs);   g_loadingscreen->update();
	shader_staticmesh = new Shader(NULL,data_sha_staticmesh_fs);   g_loadingscreen->update();
	shader_line = new Shader(NULL,data_sha_line_fs);   g_loadingscreen->update();

  mesh_tanker = new StaticMesh(data_model_tanker_m,sizeof(data_model_tanker_m));   g_loadingscreen->update();
	mesh_pboat = new StaticMesh(data_model_pboat_m,sizeof(data_model_pboat_m));   g_loadingscreen->update();
	mesh_torpedo = new StaticMesh(data_model_torpedo__m,sizeof(data_model_torpedo__m));   g_loadingscreen->update();
  mesh_missile = new StaticMesh(data_model_torpedo_m,sizeof(data_model_torpedo_m));   g_loadingscreen->update();

	mesh_opengl_logo = new StaticMesh(data_model_OpenGL_logo_flat_m,sizeof(data_model_OpenGL_logo_flat_m));   g_loadingscreen->update();

	texture_smoke = Procedural::smoke_particle_texture(texture_particle_size);   g_loadingscreen->update();
	texture_bubble = Procedural::bubble_particle_texture(texture_particle_size);   g_loadingscreen->update();
  texture_circle = Procedural::circle_texture(texture_particle_size);   g_loadingscreen->update();
	texture_crosshair = Procedural::crosshair_texture(texture_particle_size); g_loadingscreen->update();
  texture_missile_mark = Procedural::missile_mark_texture(texture_particle_size); g_loadingscreen->update();
  texture_seawaves = Procedural::seawaves_texture(); g_loadingscreen->update();
  texture_seawaves_f = Procedural::seawaves_texture_f(); g_loadingscreen->update();
  texture_line = Procedural::line_texture(); g_loadingscreen->update();
  texture_cross = Procedural::cross_texture(texture_particle_size); g_loadingscreen->update();

	font = new MFont(data_fonts_kronika_mfont,0.065f);
  g_loadingscreen->update();
}

void Resources::release(void)
{
	delete shader_seawaves;
	delete shader_clouds;
	delete shader_particles;
	delete shader_font;

  delete mesh_tanker;
	delete mesh_pboat;
  delete mesh_torpedo;
	delete mesh_missile;

	glDeleteTextures(1,&texture_smoke);	texture_smoke = 0;
	glDeleteTextures(1,&texture_bubble); texture_bubble = 0;
  glDeleteTextures(1,&texture_circle); texture_circle = 0;
	glDeleteTextures(1,&texture_crosshair); texture_crosshair = 0;
	glDeleteTextures(1,&texture_missile_mark); texture_missile_mark = 0;
	glDeleteTextures(1,&texture_seawaves); texture_seawaves = 0;
	glDeleteTextures(1,&texture_seawaves_f); texture_seawaves_f = 0;
	glDeleteTextures(1,&texture_line); texture_line = 0;
	glDeleteTextures(1,&texture_cross); texture_cross = 0;

	delete font;
}
