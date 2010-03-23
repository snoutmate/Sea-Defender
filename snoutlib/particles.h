#pragma once

#include "misc.h"
#include "shader.h"

struct Particle
{
	bool alive;
	float start_time;

	// all values in world coords (not relative)
	vec2 cur_pos; // x,y
	vec2 cur_vector;
	vec2 cur_speed; 
	float cur_size;
	vec4 cur_color; // r,g,b,a

	vec4 user_data;
};

struct pstore_s
{
	vec2 pos;
	float size;
	vec4 color;

	float c_x,c_y;
};

class ParticleEffect
{
protected:
	GLuint *m_texture;
	Shader *m_shader;

	vec2 m_pos; // start position
	float m_emit_rate;
	unsigned int m_num_particles;
	float m_particle_ttl;

	float m_last_update_time;

  float m_start_time;

// keeps track about how much particles we need to emit, if it gets above 1.0f we emit particle and decrease by 1.0f
	float m_particles_to_emit; 
	unsigned int m_mep; // max emitted particles, when reached, stop emitting

  unsigned int m_lifetime_num_emitted;

	Particle *m_particles;
	pstore_s *m_pstore; // particle store for VA/VBO

	GLuint m_list;

	bool m_emitting;

	// delta_t -> time from last call to update, p_time -> elapsed time since particle emission
	virtual void update_single_particle(Particle& p,float delta_t,float p_time) = 0;
	virtual void emit_single_particle(Particle& p) = 0;

	virtual void update_emitter(float delta_t,float pe_time);

public:
	ParticleEffect(vec2 const &pos,float emit_rate, unsigned int num_particles,float particle_ttl,GLuint *texture,unsigned int mep=0);
	virtual ~ParticleEffect();

	void update(void);
	void draw(void);

	void stop_emitting(void);
	void start_emitting(void);
	unsigned int num_live_particles(void);

	void set_pos(vec2 const& pos);
};
