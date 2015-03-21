#pragma once

#include "raymarching.hpp"
#include "particles.hpp"
#include "shadowmap.hpp"
//#include "AA.hpp"


namespace scene
{

namespace detail
{

}

//OpenGLを初期化する前にsceneの準備.
__forceinline void pre_GL_init(HWND hWnd)
{
//	AA::aa(hWnd);
}

void init()
{
	using namespace detail;


#if 0
	gl_AA::init();
#else
//	gl_AA::init_texture();
#endif

	raymarching::init();
	particles::init();
	shadowmap::init();
}

void render()
{
	using namespace detail;

	const float pos = sound::get_position();

//	particles::update(pos);
	particles::multi_update(pos);

#if 0
	gl_AA::bind_texture();
#endif

	shadowmap::begin();
	raymarching::render(pos, true);
	particles::render(pos, true);
	shadowmap::end();
	raymarching::render(pos, false);
	particles::render(pos, false);
}
}

