#ifndef JUPITER_PARTICLES_HPP
#define JUPITER_PARTICLES_HPP

typedef unsigned int index_type;

#include "particle_conf.h"

namespace particles
{

GLuint particle_prog_obj;
GLuint dynamics_prog_obj;

GLuint	vbo[3];
GLuint	tbo[3];
int		vbo_switch;
GLint	particle_prog_sample_count_uniform_loc;
GLint	particle_prog_is_shadowmap_uniform_loc;
GLint	dynamics_prog_sample_count_uniform_loc;

namespace
{

__forceinline void set_vertex_attrib()
{
	glVertexAttribPointer
	(
		0, 3, GL_FLOAT, GL_FALSE,
		sizeof(float[3]),
		(float*)(0)+0*3
	);
	glVertexAttribPointer
	(
		1, 3, GL_FLOAT, GL_FALSE,
		sizeof(float[3]),
		(float*)(0)+1*3
	);
	glVertexAttribPointer
	(
		2, 3, GL_FLOAT, GL_FALSE,
		sizeof(float[3]),
		(float*)(0)+2*3
	);
	glVertexAttribPointer
	(
		3, 3, GL_FLOAT, GL_FALSE,
		sizeof(float[3]),
		(float*)(0)+3*3
	);
	glVertexAttribPointer
	(
		4, 3, GL_FLOAT, GL_FALSE,
		sizeof(float[3]),
		(float*)(0)+4*3
	);
}

#ifdef USE_SMALL_SHADER
#	include "particle_dynamics.vs.hpp"
#endif
__forceinline void init_dynamics_prog_obj()
{
	dynamics_prog_obj = 
#ifndef USE_SMALL_SHADER
		tofu::glsl::load_program_from_file_wo_link
		(
			"particle_dynamics.vs", GL_VERTEX_SHADER
		);
#else
		tofu::glsl::load_program_wo_link
		(
			particle_dynamics_vs, GL_VERTEX_SHADER
		);
#endif
#	ifndef VAR_SAMPLE_COUNT
#		define VAR_SAMPLE_COUNT		"sample_count"
#	endif
	const char* varyings[] = {"out_position"};
	glTransformFeedbackVaryings
	(
		dynamics_prog_obj, 1, varyings, GL_INTERLEAVED_ATTRIBS
	);
	glLinkProgram(dynamics_prog_obj);
	tofu::glsl::check_link_error(dynamics_prog_obj);

	glUseProgram(dynamics_prog_obj);
	GLint loc = glGetUniformLocation(dynamics_prog_obj, VAR_SAMPLE_COUNT);
	assert(loc!=-1);
	dynamics_prog_sample_count_uniform_loc = loc;
}

#ifdef USE_SMALL_SHADER
#	include "particle_render.vs.hpp"
#	include "particle_render.fs.hpp"
#endif

__forceinline void init_render_prog_obj()
{
	particle_prog_obj =
#ifndef USE_SMALL_SHADER
		tofu::glsl::load_program_from_file_wo_link
		(
			"particle_render.vs", "particle_render.fs"
		);
#	define VAR_SAMPLE_COUNT "sample_count"
#else
		tofu::glsl::load_program_wo_link
		(
			particle_render_vs, particle_render_fs
		);
#endif
	glLinkProgram(particle_prog_obj);
	tofu::glsl::check_link_error(particle_prog_obj);

	glUseProgram(particle_prog_obj);
	GLint loc = glGetUniformLocation(particle_prog_obj, VAR_SAMPLE_COUNT);
	assert(loc!=-1);
	particle_prog_sample_count_uniform_loc = loc;

	loc = glGetUniformLocation(particle_prog_obj, "is_shadowmap");
	assert(loc!=-1);
	particle_prog_is_shadowmap_uniform_loc	= loc;
}

}

__forceinline void init()
{
	vbo_switch = 0;

	glGenBuffers(3, vbo);
	const static GLsizeiptr vbo_size
		=
															//ダミーの頂点を追加.
		sizeof(float[3])*(NUM_VERTICES_PER_LINE*NUM_LINES + NUM_ADJACENCY_VERTS2);
//	float zeros[vbo_size/sizeof(float)];	//初期値が必要ならば!
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vbo_size,
		0,
		GL_DYNAMIC_COPY
	);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vbo_size,
		0,
		GL_DYNAMIC_COPY
	);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData
	(
		GL_ARRAY_BUFFER,
		vbo_size,
		0,
		GL_DYNAMIC_COPY
	);

#if 0
	GLint data;
	glGetIntegerv(GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT, &data);
	std::cout << "GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT:" << data << std::endl;
#endif

	glGenTextures(3, tbo);
	glBindTexture(GL_TEXTURE_BUFFER, tbo[0]);
	glTexBufferRange(
		GL_TEXTURE_BUFFER, GL_RGB32F, vbo[0],
		0, sizeof(float[3])*(NUM_VERTICES_PER_LINE*NUM_LINES + NUM_ADJACENCY_VERTS2));
	glBindTexture(GL_TEXTURE_BUFFER, tbo[1]);
	glTexBufferRange(
		GL_TEXTURE_BUFFER, GL_RGB32F, vbo[1],
		0, sizeof(float[3])*(NUM_VERTICES_PER_LINE*NUM_LINES + NUM_ADJACENCY_VERTS2));
	glBindTexture(GL_TEXTURE_BUFFER, tbo[2]);
	glTexBufferRange(
		GL_TEXTURE_BUFFER, GL_RGB32F, vbo[2],
		0, sizeof(float[3])*(NUM_VERTICES_PER_LINE*NUM_LINES + NUM_ADJACENCY_VERTS2));
	glBindTexture(GL_TEXTURE_BUFFER, 0);


	init_dynamics_prog_obj();
	init_render_prog_obj();
}

__forceinline void update(const float pos)
{
	glUseProgram(dynamics_prog_obj);
	glUniform1f(dynamics_prog_sample_count_uniform_loc, pos);

	const size_t crnt_pos_vbo_id	= (vbo_switch+0)%3;
	const size_t new_pos_vbo_id		= (vbo_switch+1)%3;
	const size_t prev_pos_vbo_id	= (vbo_switch+2)%3;

	glEnable(GL_RASTERIZER_DISCARD);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[crnt_pos_vbo_id]);
	set_vertex_attrib();

	glBindBuffer(GL_ARRAY_BUFFER, vbo[prev_pos_vbo_id]);
	glVertexAttribPointer
	(
		5, 3, GL_FLOAT, GL_FALSE,
		sizeof(float[3]),
		(float*)(0)+3*NUM_ADJACENCY_VERTS
	);

	glBindBufferRange
	(
		GL_TRANSFORM_FEEDBACK_BUFFER,
		0,
		vbo[new_pos_vbo_id],
		sizeof(float[3])*NUM_ADJACENCY_VERTS,
		sizeof(float[3])*NUM_VERTICES_PER_LINE*NUM_LINES
	);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, NUM_LINES*NUM_VERTICES_PER_LINE);
	glEndTransformFeedback();

#if 0
	//for debugging.
	float result[NUM_VERTICES_PER_LINE*3*3];
	glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(result), &result);
#endif

	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

	++vbo_switch;
}

__forceinline void multi_update(const float pos)
{
	static float prev_pos = 0;

	update((pos+prev_pos)/2);
	update(pos);

	prev_pos = pos;
}

__forceinline void render(const float pos, const bool is_shadowmap)
{
	const size_t new_pos_vbo_id		= (vbo_switch)%3;

	glUseProgram(particle_prog_obj);
	glUniform1f(particle_prog_sample_count_uniform_loc, pos);
	glUniform1i(particle_prog_is_shadowmap_uniform_loc, is_shadowmap?1:0);

	glBindTexture(GL_TEXTURE_BUFFER, tbo[new_pos_vbo_id]);

//	glDrawArrays(GL_TRIANGLES, 0, num_particles*3);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, NUM_VERTICES_PER_LINE*2, NUM_LINES);
	glBindTexture(GL_TEXTURE_BUFFER, 0);
}
}

#endif
