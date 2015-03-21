#pragma once

namespace raymarching
{
GLuint	raymarching_prog_obj;

GLint	sample_count_uniform_loc;
GLint	is_shadowmap_uniform_loc;

#ifdef USE_SMALL_SHADER
#	include "raymarching.vs.hpp"
#	include "raymarching.fs.hpp"
#endif

__forceinline void init()
{
	raymarching_prog_obj =
#ifndef USE_SMALL_SHADER
		tofu::glsl::load_program_from_file_wo_link
		(
			"raymarching.vs", "raymarching.fs"
		);
#else
		tofu::glsl::load_program_wo_link
		(
			raymarching_vs, raymarching_fs
		);
#endif
#	ifndef VAR_SAMPLE_COUNT
#		define VAR_SAMPLE_COUNT		"sample_count"
#	endif
	glLinkProgram(raymarching_prog_obj);
	tofu::glsl::check_link_error(raymarching_prog_obj);

	glUseProgram(raymarching_prog_obj);
	GLint loc = glGetUniformLocation(raymarching_prog_obj, VAR_SAMPLE_COUNT);
	assert(loc!=-1);
	sample_count_uniform_loc = loc;

	loc = glGetUniformLocation(raymarching_prog_obj, "is_shadowmap");
	assert(loc!=-1);
	is_shadowmap_uniform_loc = loc;
}

__forceinline void render(const float pos, const bool is_shadowmap)
{
	glUseProgram(raymarching_prog_obj);
	glUniform1f(sample_count_uniform_loc, pos);
	glUniform1i(is_shadowmap_uniform_loc, is_shadowmap?1:0);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

}

