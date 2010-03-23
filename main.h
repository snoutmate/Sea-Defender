#pragma once

#include "snoutlib/glfwapp.h"
#include "snoutlib/settings.h"
#include "snoutlib/loadingscreen.h"
#include "snoutlib/timer.h"
#include "snoutlib/misc.h"
#include "snoutlib/mfont.h"
#include "snoutlib/menu.h"
#include "snoutlib/staticmesh.h"
#include "snoutlib/gldefs.h"
#include "snoutlib/particles.h"

#include "pe_smoke.h"
#include "pe_bubbles.h"

#include "resources.h"

#include "gamemenu.h"
#include "layout.h"
#include "background.h"
#include "boat.h"
#include "pboat.h"
#include "credits.h"
#include "hiscore.h"
#include "torpedo.h"
#include "game.h"

class App {
	Glfwapp *m_ctx;
	FPScounter *m_fpscounter;

	Background *m_background;
	Credits *m_credits;

public:
	GameMenu *m_gamemenu;
	HiScore *m_hiscore;

private:
	void init_settings(void);
  void gl_check(void);

	static void mousebuttoncb_wrapper(int button, int action);
	void mousebutton_cb(int button,int action);

	static void mouseposcb_wrapper(int x, int y);
	void mousepos_cb(int x,int y);

	static void mousewheelcb_wrapper(int pos);
	void mousewheel_cb(int pos);

	static void keycb_wrapper(int key, int action);
	void keyboard_cb(int key,int action);

	static void charcb_wrapper(int ch, int action);
	void character_cb(int ch,int action);
public:

	App(void);
	~App();
	void display_version(void);
	void run_inner_loop(void);
	void run(void);
};
