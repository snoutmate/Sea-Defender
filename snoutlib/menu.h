#pragma once

#include <string>
#include <vector>
#include <map>
#include <deque>

#include "misc.h"
#include "mfont.h"

struct MenuitemOption;

typedef vector<MenuitemOption> menuoptions_t;

struct MenuitemOption
{
  string m_name;
  string m_value;

  MenuitemOption(const string name, const string value="");

  static int option_id(const menuoptions_t& options,const string &name);
};

struct Menuitem
{
  string m_name;
  string m_text;
  string m_action;
  string m_submenu;
  menuoptions_t m_options;
  int m_selected_option;

  Menuitem(const string &name,const string &text,const string &action,const string &submenu,
           menuoptions_t options=menuoptions_t(),int selected_option=0);
};

typedef deque<Menuitem> menuitems_t;

class Menu
{
protected:
  vec_str_pair_t m_pending_actions;
  menuitems_t m_items;

  void add_action(const string &action_name, const string &action_value);
public:

  int m_selected;
  string m_parent;

  Menu(menuitems_t &items,const string &parent="");
  virtual ~Menu();

  void add_item(Menuitem item);
  void next_item(void);
  void prev_item(void);
  void selected_item_next_option(void);
  void selected_item_prev_option(void);
  string activate_item(void);
  void switch_to_item(int item);

  vec_str_pair_t get_pending_actions(void);

  virtual void draw(vec2 pos, bool centered) = 0;
  virtual void mouse_moved_to(const vec2 p) = 0;

  map_str_str_t collect_options(void);
};

class GraphicMenu : public Menu
{
  MFont *m_font;
  float m_font_size;
  vec4 m_color;
  vec4 m_color_hi;
  vector<AABB> m_bboxes;
public:
  GraphicMenu(menuitems_t &items,MFont *font,float font_size=1.0,vec4 color=vec4(1,1,1,1),vec4 color_hi=vec4(1,1,0,1));
  void draw(vec2 pos, bool centered = true);
  void mouse_moved_to(const vec2 p);
};


typedef map<string,Menu *> menustore_t;

class MenuSystem
{
  Menu *m_current_menu;
  menustore_t m_menus;

  string menup_to_str(Menu *menu);

  void activate(void);

public:
  MenuSystem();

  bool up_level(void);
  void add_menu(const string &menu_name, Menu *menu);
  void draw(vec2 pos, bool centered = true);
  vec_str_pair_t get_pending_actions(void);  
  bool key_action(int key);  
  bool mouse_action(int button);  
  void mouse_moved(void);
  void mouse_wheel(int dir);

  void switch_to_menu(const string &menu_name);
  void switch_to_item(const int item);

  Menu *str_to_menup(const string &str);
};
