
#include "game.h"
#include "resources.h"
#include "background.h"
#include "main.h"

#include "fx_ship_expl.h"
#include "fx_moving_text.h"

extern Mouse g_mouse;

Game *g_current_game;
app_modes_t g_mode = MODE_MENU;

extern App *g_app;

Game::Game(int difficulty,int wave) : m_score(0), m_difficulty(difficulty), m_wave(wave), m_hiscore_name(""), m_ended(false)
{
  m_boats[0] = new Boat(vec2(0.37f,0.24f),0);
  m_boats[1] = new Boat(vec2(0.80f,0.24f),1);
  m_boats[2] = new Boat(vec2(1.23f,0.24f),2);

  m_pboat1 = new PBoat(vec2(0.08f,0.25f));
  m_pboat2 = new PBoat(vec2(1.52f,0.25f));

  m_missile_radius = 0.07 - (0.02*m_difficulty);

  change_gamemode(GM_STARTWAVE);
}

Game::~Game()
{
  for (int i=0;i<3;++i)
    delete m_boats[i];

  delete m_pboat1;
  delete m_pboat2;

  for(unsigned int i=0;i<m_missiles.size();++i)
    delete m_missiles[i];
}

void Game::change_gamemode(game_modes_t gamemode)
{
  m_gamemode = gamemode;
  m_last_gamemode_change = g_timer->now();
}

void Game::start_wave(void)
{
  m_torpedoes_to_spawn = clamp<int>(5 + (m_wave-1), 0,20);
  m_torpedo_spawn_time = clamp<float>(4.0 - (m_wave-1)*0.1, 1.0,4.0);
  m_last_torpedo_spawned_at = 0.0;

  m_pboat1->reload_ammo();
  m_pboat2->reload_ammo();

  m_torpedoes.clear();
  m_missiles.clear();
}

void Game::spawn_torpedo(void)
{
  vec2 pos(RAND_0_1 * 1.6,1.1);
//  vec2 target(RAND_0_1 * 1.25 + 0.17,0.25);
  vec2 target;
  
  if (m_boats[0]->is_alive() && !m_boats[1]->is_alive() && !m_boats[2]->is_alive()) // left affinity
    target = vec2(rand_range(0.17,0.80),0.25);
  else if (!m_boats[0]->is_alive() && !m_boats[1]->is_alive() && m_boats[2]->is_alive()) // right affinity
    target = vec2(rand_range(0.80,1.43),0.25);
  else
    target = vec2(rand_range(0.17,1.43),0.25);
  
  vec2 vector = target - pos;
  float velocity = 0.05 + ((float)m_wave/100.0)*0.2;

  m_torpedoes.push_back(Torpedo(pos,vector,velocity));

  m_torpedoes_to_spawn--;
  m_last_torpedo_spawned_at = g_timer->now();
}

void Game::add_effect(FX *effect,int draw_order_id)
{
  m_effects.add_effect(effect,draw_order_id);
}

int Game::torpedo_hit_score(void)
{
  return (20+m_wave-1) * (1.0 + 0.5*m_difficulty);
}

int Game::ship_left_score(void)
{
  return 50 * (1.0 + 0.5*m_difficulty);
}

int Game::missile_left_score(void)
{
  return 2 * (1.0 + 0.5*m_difficulty);
}

void Game::draw_under(void)
{
  glLoadIdentity();
  for (int i=0;i<3;++i)
    m_boats[i]->draw();

	m_pboat1->draw();
	m_pboat2->draw();
}

void Game::draw_HUD(void)
{
  // wave indicator
  {
    string wave = "Wave: " + to_string<int>(m_wave);
		g_resources.font->print_text_with_shadow(wave.c_str(),vec2(0.0, 0.0), 0.7, false, vec4(1,1,1,1));
  }
  
  // score
	{
    char score[7]={0};
    sprintf(score,"%06i",clamp<int>(m_score,0,999999));
		g_resources.font->print_text_with_shadow(score,vec2(0.0, 0.0), 1.0, true, vec4(1,1,1,1));
	}

  // menu button
  {
    const char *text = "MENU ";
    vec4 color(1,1,1,1);
    vec2 mousepos = g_mouse.get_pos();
    if (mouse_over_menu_button(mousepos))
      color = vec4(1,1,0,1);

		g_resources.font->print_text_with_shadow(text,vec2(1.47, 0.0), 0.7, false, color);
  }
}

int Game::torpedoes_left(void)
{
  int n = m_torpedoes_to_spawn;

  FOR_EACH(vector<Torpedo>,m_torpedoes,it) {
    Torpedo &t = *it;

    if (t.alive() == false)
      continue;

    n++;
  }

  return n;
}

int Game::ships_left(void)
{
  int n=0;
  for(int i=0;i<3;++i) {
    if (m_boats[i]->is_alive())
      n++;
  }
  return n;
}

void Game::draw_over(void)
{
  glLoadIdentity();

  // missiles update & draw path indicator
  for(unsigned int i=0;i<m_missiles.size();++i) {
    m_missiles[i]->update();
    m_missiles[i]->draw_path();
  }

  // missile dests
  for(unsigned int i=0;i<m_missiles.size();++i)
    m_missiles[i]->draw_dest();
  
  // missiles
  for(unsigned int i=0;i<m_missiles.size();++i)
    m_missiles[i]->draw();

  // torpedoes
  FOR_EACH(vector<Torpedo>,m_torpedoes,it) {
    Torpedo &t = *it;

    t.update();
  
    if (t.alive() == false)
      continue;

//    t.m_obb.draw();
    
    // torpedo - sea collision
    vec2 t_pos = t.get_pos();
    if (t_pos[1] < 0.30) {
      float sea_level = Sea::sea_func(t_pos[0]) + 0.263;
      if (t_pos[1]<sea_level)
        t.explode();
    }

    // torpedo - boat collisions    
    for(int i=0;i<3;++i) {
      if (!m_boats[i]->is_sinking() && t.m_obb.overlaps(m_boats[i]->m_obb)) {
        t.explode();
        m_boats[i]->hit(t_pos[0]);
      }
    }

    // torpedo - exploding missile collision
    for(unsigned int y=0;y<m_missiles.size();++y) {
      Missile &m = *m_missiles[y];
      if (m.exploding() && t.m_obb.overlaps(m.m_explosion_circle)) {
        t.explode();
        m_score += torpedo_hit_score();

        string tscore = "+" + to_string<int>(torpedo_hit_score());
        FX* textfx = new FX_Moving_Text(tscore,t_pos,vec2(0.0,-1.0),vec2(0.0,0.03));
        add_effect(textfx,1);
//  FX_Moving_Text(const vec2& startpos, const vec2& dir, const vec2& speed);

//        m_effects.add

      }
    }
 
    t.draw();
  } // FOR_EACH

  m_effects.update();
  m_effects.draw_all();

  draw_HUD();

  gamemode_specific_stuff();
}


//enum game_modes_t {GM_INGAME,GM_ENDWAVE,GM_STARTWAVE,GM_GAMEOVER,GM_GAMEOVER_HISCORE};

void Game::gamemode_specific_stuff(void)
{
  float gm_time = g_timer->now() - m_last_gamemode_change;

  float menu_shift_y = 0.0;
  if (g_mode != MODE_GAME)
    menu_shift_y = -0.35;

  string s="";

  switch(m_gamemode) {
  //
  case GM_STARTWAVE:
    m_pboat1->reload_ammo();
    m_pboat2->reload_ammo();

    if (gm_time<=3.0) {
      float alpha = fade_in_out(0.0, 0.5,  2.5, 3.0, gm_time);
      alpha = clamp<float>(alpha,0.0,1.0);
      string wave = "Wave " + to_string<int>(m_wave);
      string wave2 = "Get Ready!";
      g_resources.font->print_text_with_shadow(wave.c_str(),vec2(0.0, 0.4 + menu_shift_y), 1.0, true, vec4(1,1,1,alpha));
      g_resources.font->print_text_with_shadow(wave2.c_str(),vec2(0.0, 0.4 + menu_shift_y + g_resources.font->line_sep()*1.0 ), 1.0, true, vec4(1,1,1,alpha));
    } else {
      start_wave();
      change_gamemode(GM_INGAME);
    }
    break;

  //
  case GM_INGAME:
    // spawn torpedoes if needed
    if ( m_torpedoes_to_spawn > 0 && g_timer->now() > (m_torpedo_spawn_time + m_last_torpedo_spawned_at) ) {
      spawn_torpedo();
    }

    if (ships_left()<1) {
      change_gamemode(GM_GAMEOVER);
      break;
    }
    
    if (torpedoes_left()<1) {
      change_gamemode(GM_ENDWAVE);
      break;
    }

    break;
  //
  case GM_ENDWAVE:
    {
      float y = 0.4 + menu_shift_y;
      float adv_y = g_resources.font->line_sep()*1.0;
      int missilesleft = m_pboat1->missiles_left() + m_pboat2->missiles_left();
      int shipsleft = ships_left();
      int missilesleftscore = missile_left_score() * missilesleft;
      int shipsleftscore = ship_left_score() * shipsleft;

      float alpha = fade_in_out(0.0, 0.5,  7.5, 8.0, gm_time);
      float alpha_ships = fade_in_out(1.0, 1.5,  7.5, 8.0, gm_time);
      float alpha_missiles = fade_in_out(2.0, 2.5,  7.5, 8.0, gm_time);

      s = "Wave " + to_string<int>(m_wave) + " cleared!";
      g_resources.font->print_text_with_shadow(s.c_str(),vec2(0.0, y), 1.0, true, vec4(1,1,1,alpha));

      // ships left
      {
        s = "x" + to_string<int>(shipsleft);
        g_resources.font->print_text_with_shadow(s.c_str(),vec2(0.73, y+adv_y*2), 1.0, false, vec4(1,1,1,alpha_ships));
        s = + " = +" + to_string<int>(shipsleftscore);
        g_resources.font->print_text_with_shadow(s.c_str(),vec2(0.85, y+adv_y*2), 1.0, false, vec4(1,1,1,alpha_ships));
        glPushMatrix();
        glTranslatef(0.63,y+adv_y*2+0.030,0);
        glScalef(0.17,0.17,1.0);
        glColor4f(0,0,0,alpha_ships);
        g_resources.mesh_tanker->draw();
        glPopMatrix();
      }
    
      // missiles left
      {
        s = "x" + to_string<int>(missilesleft);
        g_resources.font->print_text_with_shadow(s.c_str(),vec2(0.73, y+adv_y*3), 1.0, false, vec4(1,1,1,alpha_missiles));
        s = + " = +" + to_string<int>(missilesleftscore);
        g_resources.font->print_text_with_shadow(s.c_str(),vec2(0.85, y+adv_y*3), 1.0, false, vec4(1,1,1,alpha_missiles));
        glPushMatrix();
        glTranslatef(0.68,y+adv_y*3+0.037,0);
        glScalef(0.05,0.05,1.0);
        glRotatef(180,0,0,1);
        glColor4f(0,0,0,alpha_missiles);
        g_resources.mesh_missile->draw();
        glPopMatrix();
      }

      if (gm_time>8.0) {
        m_score += missilesleftscore + shipsleftscore;
        m_wave++;
        change_gamemode(GM_STARTWAVE);
      }
    }
    break;

  case GM_GAMEOVER:
    {
    if (g_app->m_hiscore->is_highscore(m_score)) {
      change_gamemode(GM_GAMEOVER_HISCORE);
      m_hiscore_name = g_app->m_hiscore->first_nonbuiltin_name();
      break;
    }

    string s = "Game Over";
    g_resources.font->print_text_with_shadow(s.c_str(),vec2(0.0, 0.4 + menu_shift_y), 1.0, true, vec4(1,1,1,1));
    break;
    }
  case GM_GAMEOVER_HISCORE:
    {
    float y = 0.4 + menu_shift_y;
    float adv_y = g_resources.font->line_sep()*1.0;
    string s = "Game Over";
    string s2 = "You've made a high score!";
    string s3 = "Enter your name:";

    g_resources.font->print_text_with_shadow(s.c_str(),vec2(0.0, y), 1.0, true, vec4(1,1,1,1));
    g_resources.font->print_text_with_shadow(s2.c_str(),vec2(0.0, y + adv_y), 1.0, true, vec4(1,1,1,1));
    g_resources.font->print_text_with_shadow(s3.c_str(),vec2(0.0, y + adv_y*2), 1.0, true, vec4(1,1,1,1));

    string nametext = "> " + m_hiscore_name;
    if (gm_time-(floor(gm_time))<0.5)
      nametext += "_";
    
    g_resources.font->print_text_with_shadow(nametext.c_str(),vec2(0.55, y + adv_y*3), 1.0, false, vec4(1,1,0,1));

    break;
    }
  }
}

bool Game::mouse_over_menu_button(const vec2 &pos)
{
  //printf("%.2f,%.2f\n",dest[0],dest[1]);
  if (pos[0]>1.46 && pos[1]<0.06) { // menu button active
    return true;
  }

  return false;
}

void Game::mouse_cb(int button,int action)
{
  vec2 dest = g_mouse.get_pos();

  if (mouse_over_menu_button(dest) && g_mode != MODE_MENU) {
    g_mode = MODE_MENU;
    g_app->m_gamemenu->switch_to_item(0);
    g_timer->pause();
    return;
  }

  if (g_timer->is_paused())
    return;

  if (dest[1]<0.25) // limit aiming to below ships
    return;

  if (dest[1]<0.35) // allow some leeway for clicking near water level
    dest[1]=0.35;

  switch(m_gamemode) {
  case GM_STARTWAVE:
  case GM_ENDWAVE:
  case GM_GAMEOVER:
  case GM_GAMEOVER_HISCORE:
    break;

  case GM_INGAME:
    switch(button) {
    case GLFW_MOUSE_BUTTON_LEFT:
  
      if (m_pboat1->missiles_left()) {
        m_missiles.push_back(m_pboat1->fire_missile(dest,m_missile_radius));
      }
      break;
  
    case GLFW_MOUSE_BUTTON_RIGHT:
      if (m_pboat2->missiles_left()) {
        m_missiles.push_back(m_pboat2->fire_missile(dest,m_missile_radius));
      }
      break;
    default:
      return;
    }
    break;

  }
}

void Game::highscore_entry_key(int key)
{
  switch (key) {
  // GLFW_KEY_BACKSPACE
  // GLFW_KEY_ENTER
    case GLFW_KEY_BACKSPACE:
      if (m_hiscore_name.length()>0)
        m_hiscore_name.erase(m_hiscore_name.end()-1);
      break;
    
    case GLFW_KEY_ENTER:
      g_app->m_hiscore->add_highscore(m_hiscore_name,m_score);

      change_gamemode(GM_GAMEOVER);
      m_score = 0;
      m_ended = true;

      g_mode = MODE_SCORES;

      g_app->m_gamemenu->switch_to_menu("main");
      g_app->m_gamemenu->switch_to_item(0);
    

      break;
  }
  // space, enter, backspace, delete?
//  printf("key: '%c' - %i\n",key,key);
}

void Game::highscore_entry_char(int ch)
{
  if (isprint(ch)) // filter out unicode chars
    if (m_hiscore_name.length()<16)
      m_hiscore_name += ch;
}

void Game::character_cb(int ch,int action)
{
  if (m_gamemode==GM_GAMEOVER_HISCORE) {
    highscore_entry_char(ch);
    return;
  }
}

void Game::keyboard_cb(int key,int action)
{
  if (key==GLFW_KEY_ESC) {
    if (m_gamemode == GM_GAMEOVER_HISCORE)
      return;

    if (m_gamemode == GM_GAMEOVER) {
      g_app->m_gamemenu->switch_to_menu("main");
      g_app->m_gamemenu->switch_to_item(0);
      m_ended = true;
    } else {
      g_timer->pause();
    }
    g_mode = MODE_MENU;
    g_app->m_gamemenu->switch_to_item(0);
    return;
  }

  if (m_gamemode==GM_GAMEOVER_HISCORE) {
    highscore_entry_key(key);
    return;
  }

  switch(key) {
    //case 'A':
    //  m_torpedoes_to_spawn = 0;
    //  m_torpedoes.clear();
    //  break;
    //case 'S':
    //  m_score += 1000;
    //  break;
    //case 'D':
    //  m_boats[0]->hit(0.3);
    //  m_boats[1]->hit(0.4);
    //  m_boats[2]->hit(0.5);
    //  break;

//      printf("A\n");
////      m_effects.add_effect(new FX_Ship_Explosion(vec2(0.8,0.5)),0);
//      for(int i=0;i<5;++i) {
//        Torpedo &t = torpedoes[i];
//        if (t.alive() == false)
//          continue;
//
//        t.explode();
//      }
//      break;
//
//    case 'S':
//      printf("S\n");
//      m_boat1->hit(0.1);
//      m_boat2->hit(0.2);
//      m_boat3->hit(0.3);
//      break;
  }

}

bool Game::ended(void)
{
  return m_ended;
}

game_modes_t Game::current_gamemode(void)
{
  return m_gamemode;
}
