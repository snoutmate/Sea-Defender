#ifdef WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#include <GL/glfw.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"

#define VERSION "0.9"

bool g_restart = false;
bool g_exit = false;
float g_max_x = 1.6f;
float g_max_y = 1.0f;
LoadingScreen *g_loadingscreen;
Settings *g_settings;
Layouts *g_layouts;

bool g_display_fps = false;

Mouse g_mouse;

App *g_app = NULL;

void App::init_settings(void)
{
	g_settings = new Settings;
	g_settings->read_from_file("sea.cfg");
}

void App::gl_check(void)
{
	const char* extensions[] = {
		"GL_EXT_framebuffer_object",
		"GL_ARB_vertex_buffer_object",
		"GL_ARB_shader_objects",
		"GL_ARB_vertex_shader",
		"GL_ARB_fragment_shader",
		"GL_ARB_shading_language_100",
		0
	};

	bool run = true;
	char str[1024];
	sprintf(str,"OpenGL version:  %s\n",glGetString(GL_VERSION));
	sprintf(str+strlen(str),"OpenGL vendor:   %s\n",glGetString(GL_VENDOR));
	sprintf(str+strlen(str),"OpenGL renderer: %s\n",glGetString(GL_RENDERER));
	sprintf(str+strlen(str),"GLSL version:    %s\n",glGetString(GL_SHADING_LANGUAGE_VERSION));
	sprintf(str+strlen(str),"\n");

	for(int i=0;extensions[i];++i) {
		bool res = Gl::is_available(extensions[i]);
		if (!res) run = false;
		sprintf(str+strlen(str),"%32s: %s\n",extensions[i],res ? "Yes" : "No");
	}

#ifndef NDEBUG
	printf("%s\n",str);
#endif

	if (!run) {
		sprintf(str+strlen(str),"\nThis app requires OpenGL with support for FBO and GLSL shaders\nTry to upgrade your graphic card drivers.\n\n");
#ifdef WIN32
#  ifdef MINGW
		MessageBox(NULL,(LPCSTR)str,(LPCSTR)"GL Error",MB_OK);
#  else
		MessageBox(NULL,(LPCWSTR)str,(LPCWSTR)"GL Error",MB_OK);
#endif
#else
		printf("%s",str);
#endif
		exit(-1);
	}
}

void App::mousebuttoncb_wrapper(int button, int action) { if (g_app) g_app->mousebutton_cb(button,action); }
void App::mousebutton_cb(int button,int action)
{
	if (action!=GLFW_PRESS)
		return;

	if (g_mode == MODE_MENU) {
		m_gamemenu->mouse_action(button);
	} else if (g_mode == MODE_CREDITS || g_mode == MODE_SCORES) {
		g_mode = MODE_MENU;
	} else if (g_mode == MODE_GAME) {
		if (g_current_game)
			g_current_game->mouse_cb(button,action);
	}
}

void App::mouseposcb_wrapper(int x, int y) { if (g_app) g_app->mousepos_cb(x,y); }
void App::mousepos_cb(int x,int y)
{
	if (g_mode == MODE_MENU) {
		m_gamemenu->mouse_moved();
	}
//		printf("%i,%i\n",x,y);
}

void App::mousewheelcb_wrapper(int pos) { if (g_app) g_app->mousewheel_cb(pos); }
void App::mousewheel_cb(int pos)
{
	static int last_pos = 0;
	int dir = 0;
	if (pos>last_pos)
		dir = -1;
	else if (pos<last_pos)
		dir = +1;
	last_pos = pos;

	if (g_mode == MODE_MENU) {
		m_gamemenu->mouse_wheel(dir);
	}
	
}

void App::keycb_wrapper(int key, int action) { if (g_app) g_app->keyboard_cb(key,action); }
void App::keyboard_cb(int key,int action)
{
	if (action!=GLFW_PRESS)
		return;

	if (key == GLFW_KEY_F4 && 
		(glfwGetKey(GLFW_KEY_LALT)==GLFW_PRESS || glfwGetKey(GLFW_KEY_RALT)==GLFW_PRESS)
		)
	{
		g_restart = false;
		g_exit = true;
		return;
	}

	if (!g_current_game || g_current_game->current_gamemode() != GM_GAMEOVER_HISCORE) {
		if (key == 'P') {
			g_timer->pause_toggle();
			return;
		}
	
		if (key == 'F') {
			g_display_fps = !g_display_fps;
			return;
		}
	}

	if (g_mode == MODE_MENU) {
		m_gamemenu->key_action(key);
	} else if (g_mode == MODE_CREDITS || g_mode == MODE_SCORES) {
		g_mode = MODE_MENU;
	} else if (g_mode == MODE_GAME) {
		if (g_current_game)
			g_current_game->keyboard_cb(key,action);
	}
}

void App::charcb_wrapper(int ch, int action) { if (g_app) g_app->character_cb(ch,action); }
void App::character_cb(int ch,int action)
{
	if (action!=GLFW_PRESS)
		return;

	if (g_mode == MODE_GAME) {
		if (g_current_game)
			g_current_game->character_cb(ch,action);
	}
}


App::App(void) :
	m_ctx(),m_fpscounter()
{
	g_exit = false;
	g_restart = false;
	
	init_settings();
	m_ctx = new Glfwapp("Sea Defender", *g_settings);
	m_ctx->reset_vsync(); // disable vsync (if set) as to not slow down the loading screen

	g_mouse.set_accel(g_settings->m_mouse_sensitivity);

	load_exts_entry_points();

	gl_check();
	
	g_loadingscreen = new LoadingScreen(*m_ctx);
	g_loadingscreen->update();

	g_layouts = new Layouts();
	g_loadingscreen->update();

	m_fpscounter = new FPScounter(0.33);

	// load all global resources, done here as most of them need valid GL context
	g_resources.load();

	g_loadingscreen->update();

	m_gamemenu = new GameMenu();
	g_loadingscreen->update();

	m_credits = new Credits();
	g_loadingscreen->update();

	m_hiscore = new HiScore("sea.hsc");
	g_loadingscreen->update();

	// glfwEnable(GLFW_KEY_REPEAT)
	glfwEnable(GLFW_SYSTEM_KEYS);
	glfwSetKeyCallback(App::keycb_wrapper);
	glfwSetCharCallback(App::charcb_wrapper);
	glfwSetMouseButtonCallback(App::mousebuttoncb_wrapper);
	glfwSetMousePosCallback(App::mouseposcb_wrapper);
	glfwSetMouseWheelCallback(App::mousewheelcb_wrapper);

	m_background = new Background();
	g_loadingscreen->update();
	
	g_loadingscreen->update();

	m_ctx->set_vsync(); // re-enable vsync (if set)
}

App::~App() {
	m_hiscore->save_to_file();

	delete m_ctx;
	delete m_fpscounter;
	delete m_gamemenu;
	delete g_settings; g_settings = NULL;
	delete g_layouts; g_layouts = NULL;
	delete m_background;
	delete m_credits;
	delete m_hiscore;
	
	g_resources.release();
}

void App::display_version(void)
{
	float scale = 0.5;
	const char * text = "VER: " VERSION;
	float size = g_resources.font->size_of_text(text,scale);
	g_resources.font->print_text(text,vec2(1.6f-size-0.02f, 1.01f-g_resources.font->line_sep()), scale, false);
}

void App::run_inner_loop(void)
{
  g_timer->update();
	g_mouse.update();

	if (g_current_game && g_current_game->ended()) {
		delete g_current_game;
		g_current_game = NULL;
	}


	if (glfwGetWindowParam(GLFW_ICONIFIED)==GL_TRUE && glfwGetWindowParam(GLFW_ACTIVE)==GL_FALSE)
		g_timer->pause();

	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();

	m_background->draw_sky();

	m_background->draw_sea(true);

	if (g_current_game)
		g_current_game->draw_under();

	m_background->draw_sea(false); // redraw sea again with blending for ship color change, fill-rate hit, but is still faster than enabling depth check and blending only the ship (?)

	if (g_current_game)
		g_current_game->draw_over();
	
	glLoadIdentity();

	switch(g_mode) {
	case MODE_MENU:
		g_resources.font->print_text_with_shadow("Sea Defender",vec2(0.0,0.31), 2.0f, true, vec4(0.4f,0.7,0.9f,1.0));
		m_gamemenu->draw();
		display_version();
		break;
	case MODE_CREDITS:
		m_credits->draw();
		display_version();
		break;
	case MODE_SCORES:
		m_hiscore->draw();
		display_version();
		break;
	case MODE_GAME:
		break;
	default:
		// nop
		break;
	}

	if (g_timer->is_paused()) {
		glLoadIdentity();
		g_resources.font->print_text("**PAUSED**",vec2(0.0,0.9), 1.0f, true, vec4(0.1,1.0f,0.1f,1.0));
	}

	g_mouse.draw(g_resources.texture_crosshair);

//		glTranslatef(0.1,0.5,0.0);
//		draw_textured_quad(0.4,g_resources.texture_bubble,false);

	// display fps
	char fps[16];
	sprintf(fps,"FPS: %6.2f", m_fpscounter->next_frame());

	if (g_display_fps) {
		glLoadIdentity();
		m_background->darken_area(vec4(0,0,0.16f,0.035f),0.4f);
		g_resources.font->print_text(fps,vec2(0.0,0.0), 0.5f, false, vec4(1,0,0.2f,1.0));
	}
//		sleep(0.01) if @fps > 60.0
}

void App::run(void)
{
	while(true) {
		m_ctx->init_frame();

		m_ctx->set_ortho2D();

//      glClearColor(0.35f,0.55f,0.75f, 0);
		glClearColor(1.0,1.0,1.0, 1.0);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		run_inner_loop();

		m_ctx->end_frame();

		if (g_exit || m_ctx->window_closed())
			break;
	}
}

#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else 
int main(void)
#endif
{
	glfwInit();
	srand((unsigned int)time(NULL));

	g_timer = new Timer();

	do {
		g_app = new App;

		g_app->run();

		delete g_app;
		g_app = NULL;
	} while (g_restart);

	delete g_timer;

	return 0;
}
