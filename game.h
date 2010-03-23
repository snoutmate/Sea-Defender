#pragma once

#include "snoutlib/misc.h"
#include "snoutlib/fx.h"

#include "boat.h"
#include "pboat.h"
#include "torpedo.h"
#include "missile.h"

enum app_modes_t {MODE_MENU,MODE_GAME,MODE_CREDITS,MODE_SCORES};
extern app_modes_t g_mode;

enum game_modes_t {GM_INGAME,GM_ENDWAVE,GM_STARTWAVE,GM_GAMEOVER,GM_GAMEOVER_HISCORE};

class Game
{
  int m_score;
  int m_difficulty; // 0,1,2
  int m_wave;

	Boat *m_boats[3];
	
	PBoat *m_pboat1;
	PBoat *m_pboat2;
	
	vector<Torpedo> m_torpedoes;
	vector<Missile*> m_missiles;

  FX_stack m_effects;

  float m_missile_radius;

  int m_torpedoes_to_spawn;
  float m_last_torpedo_spawned_at;
  float m_torpedo_spawn_time;

  game_modes_t m_gamemode;
  float m_last_gamemode_change;

  bool mouse_over_menu_button(const vec2 &pos);
  int torpedo_hit_score(void);
  void start_wave(void);
  void draw_HUD(void);
  void spawn_torpedo(void);
  void gamemode_specific_stuff(void);
  void change_gamemode(game_modes_t gamemode);
  int torpedoes_left(void);
  int ships_left(void);
  int ship_left_score(void);
  int missile_left_score(void);

  void highscore_entry_key(int key);
  void highscore_entry_char(int ch);

  string m_hiscore_name;
  bool m_ended;
public:
  Game(int difficulty, int wave=1);
  ~Game();

  void draw_under(void);
  void draw_over(void);

  void add_effect(FX *effect,int draw_order_id=0);

	void keyboard_cb(int key,int action);
  void character_cb(int ch,int action);
  void mouse_cb(int button,int action);

  bool ended(void);
  game_modes_t current_gamemode(void);
};

extern Game *g_current_game;

