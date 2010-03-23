#pragma once

#include "snoutlib/shader.h"
#include "snoutlib/staticmesh.h"
#include "snoutlib/mfont.h"

struct Resources
{
	Shader *shader_seawaves;
	Shader *shader_clouds;
	Shader *shader_particles;
	Shader *shader_font;
	Shader *shader_staticmesh;
	Shader *shader_line;
	
	StaticMesh *mesh_tanker;
	StaticMesh *mesh_pboat;
	StaticMesh *mesh_torpedo;
	StaticMesh *mesh_missile;
	StaticMesh *mesh_opengl_logo;

	GLuint texture_smoke;
	GLuint texture_bubble;
	GLuint texture_circle;
	GLuint texture_crosshair;
  GLuint texture_missile_mark;
  GLuint texture_seawaves;
  GLuint texture_seawaves_f;
  GLuint texture_line;
  GLuint texture_cross;
	
	MFont *font;
	
	void load(void);
	void release(void);

  // all sizes will be downsized 2x for in-game textures (this sizes are used for supersampling during pre-comp)
  static const int texture_font_size = 2048;
  static const int texture_clouds_size = 2048;
  static const int texture_mesh_size = 2048;

  static const int texture_particle_size = 512;
};

extern Resources g_resources;
