#include <cmath>
#include "gamemenu.h"
#include "layout.h"
#include "resources.h"
#include "game.h"
#include "background.h"

extern bool g_exit;
extern bool g_restart;
extern Settings *g_settings;
extern Layouts *g_layouts;
extern Mouse g_mouse;

GameMenu::GameMenu() :
  m_menusystem()
{
  create_menus();
  if (g_current_game)
    m_menusystem.switch_to_menu("main_ingame");
}

GameMenu::~GameMenu()
{
  for (unsigned int i=0;i<m_menus.size();++i)
    delete m_menus[i];
}

void GameMenu::create_menus(void)
{
  m_menusystem = MenuSystem();

  int current_w=0,current_h=0;
  glfwGetWindowSize(&current_w,&current_h);

  GLFWvidmode *vidmodes = new GLFWvidmode[256];
  int nummodes = glfwGetVideoModes(vidmodes,256);

  // get resolutions
  menuoptions_t resolutions;
  int selected_resolution=0;
  for(int i=0,x=0;i<nummodes;++i) {
    if ((vidmodes[i].RedBits + vidmodes[i].GreenBits + vidmodes[i].BlueBits) < 24) continue;
    if (vidmodes[i].Width < 1024) continue;
   
    string s = to_string<int>(vidmodes[i].Width) + "x" + to_string<int>(vidmodes[i].Height);
    string s2 = to_string<int>(vidmodes[i].Width) + " " + to_string<int>(vidmodes[i].Height);
    resolutions.push_back(MenuitemOption(s,s2));
    if (vidmodes[i].Width == current_w && vidmodes[i].Height == current_h)
      selected_resolution = x;

    x++;
  }
  delete[] vidmodes;

//  resolutions.push_back(MenuitemOption("1280x720","1280 720"));

  // construct menus

  menuitems_t items;

  // main
  items.push_back(Menuitem("","New Game",  "",          "newgame"));
  items.push_back(Menuitem("","Settings", "",          "settings"));
  items.push_back(Menuitem("","Hiscore",   "scores",   ""));
  items.push_back(Menuitem("","Credits",   "credits",   ""));
  items.push_back(Menuitem("","Exit",         "exit",      ""));
  Menu *m_menu = new GraphicMenu(items,g_resources.font,1.0);
  m_menusystem.add_menu("main",m_menu); m_menus.push_back(m_menu);

  // main in game
  items.push_front(Menuitem("","Resume Game", "resumegame", ""));
  m_menu = new GraphicMenu(items,g_resources.font,1.0);
  m_menusystem.add_menu("main_ingame",m_menu); m_menus.push_back(m_menu);

  // settings
  items.clear();
  items.push_back(Menuitem("","Video Settings","","videosettings"));
  items.push_back(Menuitem("","Input Settings","","inputsettings"));
  items.push_back(Menuitem("","Back","",":up_level"));
  m_menu = new GraphicMenu(items,g_resources.font,1.0);
  m_menusystem.add_menu("settings",m_menu); m_menus.push_back(m_menu);

  // video settings
  menuoptions_t boolean;
  boolean.push_back(MenuitemOption("No","0"));
  boolean.push_back(MenuitemOption("Yes","1"));

  //menuoptions_t fsaa;
  //fsaa.push_back(MenuitemOption("No","0"));
  //fsaa.push_back(MenuitemOption("2x","2"));
  //fsaa.push_back(MenuitemOption("4x","4"));
  ////  fsaa.push_back(MenuitemOption("6x","6"));
  ////  fsaa.push_back(MenuitemOption("8x","8"));

  items.clear();
  items.push_back(Menuitem("resolution","Resolution","","",resolutions,selected_resolution));
  items.push_back(Menuitem("fullscreen","Fullscreen","","",boolean,g_settings->m_fullscreen));
  //items.push_back(Menuitem("fsaa","Multisampling","","",fsaa,g_settings->m_fsaa/2));
  items.push_back(Menuitem("vsync","V-sync","","",boolean,g_settings->m_vsync));
  items.push_back(Menuitem("","Apply settings","apply_video_settings",""));
  items.push_back(Menuitem("","Back","",":up_level"));
  m_menu = new GraphicMenu(items,g_resources.font,1.0);
  m_menusystem.add_menu("videosettings",m_menu); m_menus.push_back(m_menu);

  menuoptions_t sensitivity;
  for(int i=1;i<=10;++i)
    sensitivity.push_back(MenuitemOption(to_string<int>(i),to_string<int>(i)));

  items.clear();
  items.push_back(Menuitem("mouse_sensitivity","Mouse Sensitivity","mouse_sensitivity_change","",sensitivity,(g_settings->m_mouse_sensitivity)-1));
  items.push_back(Menuitem("","Back","",":up_level"));
  m_menu = new GraphicMenu(items,g_resources.font,1.0);
  m_menusystem.add_menu("inputsettings",m_menu); m_menus.push_back(m_menu);


  // start menu

  menuoptions_t layouts;
  vector<string> names = g_layouts->get_names();
  FOR_EACH_CONST(vector<string>,names,it)
    layouts.push_back(MenuitemOption(*it,*it));

  menuoptions_t difficulty;
  difficulty.push_back(MenuitemOption("Easy","0"));
  difficulty.push_back(MenuitemOption("Normal","1"));
  difficulty.push_back(MenuitemOption("Hard","2"));

  menuoptions_t waves;
  waves.push_back(MenuitemOption("1","1"));
  waves.push_back(MenuitemOption("5","5"));
  waves.push_back(MenuitemOption("10","10"));
  waves.push_back(MenuitemOption("25","25"));
  waves.push_back(MenuitemOption("50","50"));

  items.clear();
  items.push_back(Menuitem("","Start","start_game",""));
  items.push_back(Menuitem("difficulty","Difficulty","","",difficulty,1));
  items.push_back(Menuitem("startingwave","Starting wave","","",waves,0));
  items.push_back(Menuitem("layout","Theme","layout_change","",layouts,0));
  items.push_back(Menuitem("","Back","",":up_level"));
  m_menu = new GraphicMenu(items,g_resources.font,1.0);
  m_menusystem.add_menu("newgame",m_menu); m_menus.push_back(m_menu);
}

void GameMenu::process_action(str_pair_t action)
{
  string &name = action.first;
  string &value = action.second;

  if (name=="exit") {
    g_exit = true;
  }
  
  if (name=="apply_video_settings") {
    // collect save settings
    Menu *m = m_menusystem.str_to_menup("videosettings");
    map_str_str_t opts = m->collect_options();

    stringstream s(opts["resolution"]);
    s >> g_settings->m_res_x >> g_settings->m_res_y;
//    g_settings->m_fsaa = from_string<int>(opts["fsaa"]);
    g_settings->m_vsync = (from_string<int>(opts["vsync"]) ? true : false);
    g_settings->m_fullscreen = (from_string<int>(opts["fullscreen"]) ? true : false);

    // set for restart
    g_restart = true;
    g_exit = true;
  }
  
  if (name=="layout_change") {
    g_layouts->set_layout(value);
  }

  if (name=="mouse_sensitivity_change") {
    int sens = from_string<int>(value);
    g_settings->m_mouse_sensitivity = sens;
    g_mouse.set_accel(sens);
  }

  if (name=="credits") {
		g_mode = MODE_CREDITS;
	}

  if (name=="scores") {
		g_mode = MODE_SCORES;
	}

  if (name=="start_game") {
    Menu *m = m_menusystem.str_to_menup("newgame");
    map_str_str_t opts = m->collect_options();

    int difficulty = from_string<int>(opts["difficulty"]);
    int wave = from_string<int>(opts["startingwave"]);

//    m_menusystem.up_level();
    m_menusystem.switch_to_menu("main_ingame");

    delete g_current_game;
    g_current_game = new Game(difficulty,wave);
		g_mode = MODE_GAME;
    g_timer->unpause();
	}

  if (name=="resumegame") {
    g_mode = MODE_GAME;
    g_timer->unpause();
  }
}

void GameMenu::draw(void)
{
  m_menusystem.draw(vec2(0.0,0.45f),true);
}

void GameMenu::mouse_action(int button)
{
  m_menusystem.mouse_action(button);

  vec_str_pair_t actions = m_menusystem.get_pending_actions();
  FOR_EACH_CONST(vec_str_pair_t,actions,it)
    process_action(*it);
}

void GameMenu::mouse_moved(void) { m_menusystem.mouse_moved(); }
void GameMenu::mouse_wheel(int dir) { m_menusystem.mouse_wheel(dir);}

void GameMenu::key_action(int key)
{
  bool topexit = m_menusystem.key_action(key);

  if (topexit) {
    if (g_current_game) {
  		g_mode = MODE_GAME;
      g_timer->unpause();
    } else {
      g_exit = true;
    }
  }

  vec_str_pair_t actions = m_menusystem.get_pending_actions();
  FOR_EACH_CONST(vec_str_pair_t,actions,it)
    process_action(*it);
}

void GameMenu::switch_to_item(const int item)
{
  m_menusystem.switch_to_item(item);
}

void GameMenu::switch_to_menu(const string& menu)
{
  m_menusystem.switch_to_menu(menu);
}
