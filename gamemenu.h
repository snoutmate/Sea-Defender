#pragma once
#include <algorithm>
#include <functional>
#include <string>

#include "snoutlib/menu.h"
#include "snoutlib/mfont.h"
#include "snoutlib/settings.h"

typedef vector<Menu *> vec_menup;

class GameMenu
{
  MenuSystem m_menusystem;
  vec_menup m_menus;

  void create_menus(void);
  void process_action(str_pair_t action);

public:
  GameMenu();
  ~GameMenu();

  void draw(void);
  void key_action(int key);
  void mouse_action(int button);
  void mouse_moved(void);
  void mouse_wheel(int dir);
  void switch_to_item(const int item);
  void switch_to_menu(const string& menu);
};
