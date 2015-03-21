//#pragma comment(linker,"/subsystem:windows")
//#pragma comment(linker,"/NODEFAULTLIB")
#include "stdafx.h"

//#ifdef DEMO_MODE
#ifdef NDEBUG
#define USE_SMALL_SHADER
#endif

#ifndef DEMO_MODE
#	define WIDE_CHAR2(n) L##n
#	define WIDE_CHAR(n) WIDE_CHAR2(n)
#endif

#include "sound.hpp"
#include "scene.hpp"

#if 0
// :%s/"/\\"/g
// :%s/\(^.*$\)/"\1\\r\\n"/g
const static char AA[] = 
#include "aa.h"
;
#endif

#ifndef WIDTH
#define WIDTH 640
#define HEIGHT 480
#endif

#ifdef __cplusplus
extern "C" { 
#endif
#ifdef DEMO_MODE
	int _fltused=1; 
#endif
	void _cdecl _check_commonlanguageruntime_version(){}
#ifdef __cplusplus
}
#endif

static DEVMODE devmode =
{
	{0}, 0, 0, sizeof(DEVMODE), 0,
	DM_PELSWIDTH | DM_PELSHEIGHT,		//画面の何を変更するか.
	{0}, 0, 0, 0, 0, 0, {0}, 0,
	0,									//Bits Per Pixel
	WIDTH,								//画面の幅(pixel)
	HEIGHT,								//画面の高さ(pixel)
	{0},
	0									//画面の周波数(Hz)
};

static PIXELFORMATDESCRIPTOR pfd =
{
	sizeof(PIXELFORMATDESCRIPTOR),		//この構造体のサイズ
	1,									//OpenGLバージョン
	PFD_DRAW_TO_WINDOW |				//Windowスタイル
	PFD_SUPPORT_OPENGL |				//OpenGL
	PFD_DOUBLEBUFFER,					//ダブルバッファ使用可能
	PFD_TYPE_RGBA,						//RGBAカラー
	24,									//色数
	0, 0,								//RGBAのビットとシフト設定        
	0, 0,								//G
	0, 0,								//B
	0, 0,								//A
	0,									//アキュムレーションバッファ
	0, 0, 0, 0,							//RGBAアキュムレーションバッファ
	24,									//Zバッファ    
	0,									//ステンシルバッファ
	0,									//使用しない
	PFD_MAIN_PLANE,						//レイヤータイプ
	0,									//予約
	0, 0, 0								//レイヤーマスクの設定・未使用
};

#define MSAA_SAMPLES 8
#ifdef MSAA_SAMPLES
int iAttribIList[] =
{
	WGL_DRAW_TO_WINDOW_ARB,	GL_TRUE,
	WGL_SUPPORT_OPENGL_ARB,	GL_TRUE,
	WGL_DOUBLE_BUFFER_ARB,	GL_TRUE,
	WGL_PIXEL_TYPE_ARB,		WGL_TYPE_RGBA_ARB,
	WGL_COLOR_BITS_ARB,		32,
	WGL_DEPTH_BITS_ARB,		24,
	WGL_STENCIL_BITS_ARB,	0,
	WGL_SAMPLE_BUFFERS_ARB,	1,				//Number of buffers (must be 1 at time of writing)
	WGL_SAMPLES_ARB,		MSAA_SAMPLES,	//Number of samples
	0
};
#endif

_forceinline void setupGLState()
{
	reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"))(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
//	glClearColor(0.75f, 0.75f, 1.0f, 1.0f);
}

#ifndef DEMO_MODE
void print_last_error()
{
	const DWORD err = GetLastError();
	LPWSTR pmsg;
	const DWORD	size = FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
		0,
		err,
		0,
		reinterpret_cast<LPWSTR>(&pmsg),
		0,
		0);
	if(size == 0)
	{
		std::wcerr << L"Failed to call FormatMessageW." << std::endl;
		return;
	}
	std::wcerr << pmsg << std::endl;
	LocalFree(pmsg);
}

struct gl_call_info
{
	const	char*	funcname;
			int		line;
	const	char*	filename;

	gl_call_info():
		funcname(0), line(0), filename(0)
	{
	}

	void set(const char* funcname, int line, const char* filename)
	{
		this->funcname	= funcname;
		this->line		= line;
		this->filename	= filename;
	}
};

gl_call_info& get_gl_call_info()
{
	static gl_call_info gci;

	return gci;
}

void APIENTRY gl_debug_callback(
	GLenum source, GLenum type, GLuint id,
	GLenum severity,
	GLsizei length, const char *message,
	GLvoid * /*userParam*/)
{
	using namespace std;

	(source);
	(type);
	(id);
	(severity);
	(length);

	assert(get_gl_call_info().funcname);

	cerr << "Message from glDebugMessageCallback:\n";
	cerr <<
		"In " << get_gl_call_info().funcname << ", " <<
		get_gl_call_info().filename << ":" <<
		get_gl_call_info().line <<
		endl;

	cerr << message << endl;

	static unsigned int count = 0;
	if(++count > 10)
	{
		cerr << "continue?(y/n)" << endl;
		char c;
		cin >> c;
		count = 0;
		if(c != 'y')
			exit(0);
	}
}

namespace glew4kb
{
void custom_before_func(const char* name, int line, const char* file)
{
	get_gl_call_info().set(name, line, file);
}
}

HWND init_window()
{
	HWND hWnd =	CreateWindowW(L"STATIC", 0,
		WS_POPUP|WS_VISIBLE, 0, 0,
		WIDTH, HEIGHT, NULL, NULL, NULL,NULL);

	if (!hWnd)
	{
		std::wcerr << L"Failed to call CreateWindowW." << std::endl;
		print_last_error();
		return 0;
	}

	return hWnd;
}

bool init_glew()
{
	GLenum glewError;

	if((glewError = glewInit()) != GLEW_OK)
	{
		std::cerr << "Error: " << glewGetErrorString(glewError) << std::endl;
		return false;
	}

	return true;
}

#ifdef MSAA_SAMPLES
bool init_MSAA(HWND& hWnd, HDC& hdc, HGLRC context)
{
	if(!init_glew())
		return false;

	if(!WGLEW_ARB_multisample)
	{
		std::wcerr << L"WGL_ARB_multisample is not supported." << std::endl;
		return false;
	}

	if(!wglGetProcAddress("wglChoosePixelFormatARB"))
	{
		std::wcerr << L"wglChoosePixelFormatARB is not available." << std::endl;
		return false;
	}

	int iFormat;
	UINT nNumFormats;
	if
	(
		reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>
		(
			wglGetProcAddress("wglChoosePixelFormatARB")
		)
		(hdc, iAttribIList, 0, 1, &iFormat, &nNumFormats)
		==
		FALSE
	)
	{
		std::wcerr << L"Failed to call wglChoosePixelFormatARB." << std::endl;
		return false;
	}

	if(nNumFormats == 0)
	{
		std::wcerr << "MSAA pixel format is not available." << std::endl;
		return false;
	}

	//一度windowを破壊して作り直さないといけないらしい.
	wglMakeCurrent(hdc, 0);
	wglDeleteContext (context);
	ReleaseDC(hWnd, hdc);
	DestroyWindow(hWnd);

	hWnd = init_window();
	if(!hWnd)
		return false;

	hdc = GetDC(hWnd);

	if(!SetPixelFormat(hdc, iFormat, &pfd))
	{
		std::wcerr << L"Failed to call SetPixelFormat" << std::endl;
		return false;
	}

	context = wglCreateContext(hdc);
	if(context == NULL)
	{
		std::wcerr << L"Failed to call wglCreateContext" << std::endl;
		return false;
	}

	if(!wglMakeCurrent(hdc, context))
	{
		std::wcerr << L"Failed to call wglMakeCurrent(hdc, context)" << std::endl;
		return false;
	}

	return true;
}
#else
bool init_MSAA(HWND&, HDC&, HGLRC)
{
	return true;
}
#endif

bool init_debug_context(HWND& hWnd, HDC& hdc)
{
	if(!SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd))
	{
		std::wcerr << L"Failed to call SetPixelFormat" << std::endl;
		return false;
	}

	HGLRC tempContext = wglCreateContext(hdc);
	if(tempContext == NULL)
	{
		std::wcerr << L"Failed to call wglCreateContext" << std::endl;
		return false;
	}

	if(!wglMakeCurrent(hdc, tempContext))
	{
		std::wcerr << L"Failed to call wglMakeCurrent(hdc, tempContext)" << std::endl;
		return false;
	}

	std::cout <<
		"\nGL_VERSION:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_VERSION)) <<
		"\nGL_RENDERER:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_RENDERER)) <<
		"\nGL_VENDOR:\n\t" <<
			reinterpret_cast<const char*>(glGetString(GL_VENDOR)) << std::endl;
	std::wcerr << L"If GL_VERSION is lower than 4.2, this demo will not work." << std::endl;
	std::wcerr << L"If GL_VERSION is 4.1 or 4.0, updating your GPU's driver might bring OpenGL 4.2 or higher." << std::endl;
	std::wcerr << L"If GL_VERSION is lower than 4.0, go to electronics store and ask them give me a 3D accelerator which supports OpenGL version 4.2 or higher." << std::endl;

	if(!init_MSAA(hWnd, hdc, tempContext))
	{
		std::wcerr << L"MSAA is not available." << std::endl;
	}

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB,	4,
		WGL_CONTEXT_MINOR_VERSION_ARB,	2,
		WGL_CONTEXT_FLAGS_ARB,			WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB,	WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,//WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	if(!wglGetProcAddress("wglCreateContextAttribsARB"))
	{
		std::wcerr << L"Failed to get wglCreateContextAttribsARB." << std::endl;
		return false;
	}

	const HGLRC hrc =
		reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>
		(
			wglGetProcAddress("wglCreateContextAttribsARB")
		)(hdc, 0, attribs);
	if(hrc == NULL)
	{
		std::wcerr << L"Failed to call wglCreateContextAttribsARB," << std::endl;
		return false;
	}

	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(tempContext);
	if(wglMakeCurrent(hdc, hrc) == FALSE)
	{
		std::wcerr << L"Failed to call wglMakeCurrent(hdc, hrc)." << std::endl;
		return false;
	}

	using namespace std;

	glewExperimental = GL_TRUE;
	if(!init_glew())
		return false;

	if(!GLEW_VERSION_4_2)
	{
		std::wcerr << L"Error: This demo needs OpenGL 4.2 or higher, but your computer doesn't support it." << std::endl;
		return false;
	}

	tofu::opengl::print_context_info();

	GLint cntxt_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &cntxt_flags);
	if(cntxt_flags & GL_CONTEXT_FLAG_DEBUG_BIT && glIsEnabled(GL_DEBUG_OUTPUT)==GL_TRUE)
	{
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		GLuint ids = 0;
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, &ids, GL_TRUE);
		glDebugMessageCallback(&gl_debug_callback, NULL);
	}

	return true;
}

bool init_screen(HWND& hWnd, HDC& hdc)
{
	//標準出力に日本語を表示するためにロケールを設定.
	std::locale::global(std::locale(""));

	hWnd = init_window();
	if(!hWnd)
		return false;

	scene::pre_GL_init(hWnd);

	hdc = GetDC(hWnd);

	if(!init_debug_context(hWnd, hdc))
	{
		print_last_error();
		return false;
	}

	return true;
}
#else
_forceinline bool init_screen(HWND& hWnd, HDC& hdc)
{
	if(ChangeDisplaySettings(&devmode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		return false;

	hWnd =	CreateWindowW(L"STATIC", 0,
		WS_POPUP|WS_VISIBLE, 0, 0,
		WIDTH, HEIGHT, NULL, NULL, NULL,NULL);

	if(!hWnd)
		return false;

//	SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, width, height, SWP_SHOWWINDOW);
	scene::pre_GL_init(hWnd);

	hdc = GetDC(hWnd);

	if(!SetPixelFormat(hdc, ChoosePixelFormat(hdc, &pfd), &pfd))
		return false;

	wglMakeCurrent(hdc, wglCreateContext(hdc));

	int iFormat;
	UINT nNumFormats;
	reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>
	(
		wglGetProcAddress("wglChoosePixelFormatARB")
	)
	(hdc, iAttribIList, 0, 1, &iFormat, &nNumFormats);

	//MSAAを使うにはwindowを作り直す必要があるらしい.
	//4k introではメモリリークを気にする必要は無い.
	hWnd =	CreateWindowW(L"STATIC", 0,
		WS_POPUP|WS_VISIBLE, 0, 0,
		WIDTH, HEIGHT, NULL, NULL, NULL,NULL);

	hdc = GetDC(hWnd);

	SetPixelFormat(hdc, iFormat, &pfd);

	wglMakeCurrent(hdc, wglCreateContext(hdc));

	ShowCursor(false);

	glew4kb_setup_GL_ext();

	return true;
}
#endif

#ifdef DEMO_MODE
void WinMainCRTStartup()
{
#else
int main()
//int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int)
{
#endif

	HWND hWnd;
	HDC hdc;
	if(!init_screen(hWnd, hdc))
	{
#ifdef DEMO_MODE
		ExitProcess(0);
#else
		system("pause");
		return 1;
#endif
	}

	setupGLState();

	scene::init();
	sound::make_sound(hWnd);

#if 0
	MSG	msg;
	while(TRUE)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if((msg.message==WM_CHAR)&&(msg.wParam==0x1B))
				break;//ESC
			DispatchMessage(&msg);
		}else{
			OnIdle();
			SwapBuffers(hdc);
		}
	}

	ExitProcess(0);
	return	;
#else
	MSG msg;
loop:
	//実行中にキーボードやマウスからの入力を受け取るならPeekMessageを呼ばないと
	//OSからハングしたプロセスとみなされる.
	//DisableProcessWindowsGhostingを呼ぶという方法もあるらしい.
	PeekMessage(&msg, 0, 0, 0, PM_REMOVE);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene::render();
	SwapBuffers(hdc);
	if(GetAsyncKeyState(VK_ESCAPE) || msg.message == MM_WOM_DONE)
#ifdef DEMO_MODE
		ExitProcess(0);
#else
	{
	//	return 0;
		glew4kb::output();
		ExitProcess(0);
	}
#endif
	goto loop;
#endif
}
