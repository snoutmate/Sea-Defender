#include "menu.h"

extern bool g_exit;
extern Mouse g_mouse;

MenuitemOption::MenuitemOption(const string name, const string value) :
  m_name(name),m_value(value)
{}

int MenuitemOption::option_id(const menuoptions_t& options,const string &name)
{
  for(unsigned int i=0;i<options.size();++i) {
    if (options[i].m_name == name)
      return i;
  }

  return -1;
}


Menuitem::Menuitem(const string &name,const string &text,const string &action,const string &submenu,
                   menuoptions_t options,int selected_option) :
  m_name(name),m_text(text),m_action(action),m_submenu(submenu),m_options(options),m_selected_option(selected_option)
{}

Menu::Menu(menuitems_t &items,const string &parent) :
  m_items(items),m_selected(0),m_parent(parent)
{} 

Menu::~Menu()
{}

void Menu::add_item(Menuitem item)
{
  m_items.push_back(item);
}

void Menu::next_item(void)
{
  m_selected++;
  if (m_selected>=(int)m_items.size())
    m_selected = 0;
}

void Menu::prev_item(void)
{
  m_selected--;
  if (m_selected<0)
    m_selected = (m_items.size()-1);
}

void Menu::selected_item_next_option(void)
{
  Menuitem &item = m_items[m_selected];
  if (item.m_options.empty()) // item does not have options
    return;

  item.m_selected_option++;
  if (item.m_selected_option>=(int)item.m_options.size())
    item.m_selected_option = 0;
  
  if (!item.m_action.empty()) // if options have action set, set it every time the selected option changes
    add_action(item.m_action, item.m_options[item.m_selected_option].m_value );
}

void Menu::selected_item_prev_option(void)
{
  Menuitem &item = m_items[m_selected];
  if (item.m_options.empty()) // item does not have options
    return;

  item.m_selected_option--;

  if (item.m_selected_option<0)
    item.m_selected_option = item.m_options.size() - 1;

  if (!item.m_action.empty()) // if options have action set, set it every time the selected option changes
    add_action(item.m_action, item.m_options[item.m_selected_option].m_value );
}

void Menu::add_action(const string &action_name, const string &action_value)
{
  m_pending_actions.push_back(make_pair(action_name,action_value));
}

vec_str_pair_t Menu::get_pending_actions(void)
{
  vec_str_pair_t actions = m_pending_actions;
  m_pending_actions.clear();
  return actions;
}


map_str_str_t Menu::collect_options(void)
{
  map_str_str_t opts;

  for (menuitems_t::const_iterator it=m_items.begin(),end=m_items.end();it!=end;++it) {
    Menuitem mi = *it;
    if (mi.m_options.empty()) continue;
    if (mi.m_name.empty()) continue;
    
    opts[mi.m_name] = mi.m_options[mi.m_selected_option].m_value;
  }
  return opts;
}

string Menu::activate_item(void)
{
  Menuitem &item = m_items[m_selected];

  if (!item.m_options.empty()) { // item does have options
    selected_item_next_option();
  } else if (!item.m_action.empty()) { // action
    if (item.m_action == ":disabled")
      return "";

    add_action(item.m_action, "");

  } else if (!item.m_submenu.empty()) {
    if (item.m_submenu==":up_level")
      return m_parent;
    else
      return item.m_submenu;
  }

  return ""; 
}

void Menu::switch_to_item(int item)
{
  item = clamp<int>(item, 0, m_items.size());
  m_selected = item;
}


GraphicMenu::GraphicMenu(menuitems_t &items,MFont *font,float font_size,vec4 color,vec4 color_hi) :
  Menu(items),m_font(font),m_font_size(font_size),m_color(color),m_color_hi(color_hi),m_bboxes()
{
}

void GraphicMenu::draw(vec2 pos, bool centered)
{
  m_font->setup_drawing();

  glPushMatrix();
  glTranslate(vec3(pos));

  m_bboxes.clear();

  for(int i=0;i<(int)m_items.size();++i) {
    Menuitem &item = m_items[i];
    float ypos = m_font->line_sep() * i * m_font_size;
    float xpos = 0.0;
    vec4 color;
    if (m_selected == i)
      color = m_color_hi;
    else
      color = m_color;

    string text;
    if (!item.m_options.empty()) {
      if (m_selected == i) {
        text = item.m_text + ": <" + item.m_options[item.m_selected_option].m_name + ">";
        xpos += (m_font->size_of_text("<>",m_font_size))/2.0f;
      } else {
        text = item.m_text + ": " + item.m_options[item.m_selected_option].m_name;
      }
    } else {
      text = item.m_text;
    }

    if (m_selected == i) {
      m_font->print_text_with_shadow(text.c_str(),vec2(xpos,ypos),m_font_size,centered,color);
    } else {
      m_font->print_text(text.c_str(),vec2(xpos,ypos),m_font_size,centered,color);
    }

    AABB bb = m_font->bbox(text.c_str(),m_font_size,vec2(xpos,ypos)+pos,centered);
    m_bboxes.push_back(bb);
  }
  glPopMatrix();

  m_font->finish_drawing();
}

void GraphicMenu::mouse_moved_to(const vec2 p)
{
  for (unsigned int i=0;i<m_bboxes.size();++i) {
    if (m_bboxes[i].contains_point(p)) { // return on first match
      m_selected = i;
      return;
    }
  }
}

MenuSystem::MenuSystem() :
  m_current_menu(NULL)
{}

void MenuSystem::add_menu(const string &menu_name, Menu *menu)
{
  m_menus[menu_name] = menu;
  if (menu_name == "main")
    m_current_menu = menu;
}

void MenuSystem::switch_to_menu(const string &menu_name)
{
  m_current_menu = m_menus[menu_name];
}

void MenuSystem::switch_to_item(int item)
{
  if (m_current_menu)
    m_current_menu->switch_to_item(item);
}

void MenuSystem::draw(vec2 pos, bool centered)
{
  if (m_current_menu)
    m_current_menu->draw(pos,centered);
}

vec_str_pair_t MenuSystem::get_pending_actions(void)
{
  vec_str_pair_t actions;
  for(menustore_t::const_iterator it=m_menus.begin();it!=m_menus.end();++it) {
    vec_str_pair_t a = (*it).second->get_pending_actions();
    copy(a.begin(),a.end(),back_inserter(actions));
  }
  return actions;
}

bool MenuSystem::up_level(void)
{
  if (!m_current_menu->m_parent.empty()) {
    m_current_menu = m_menus[m_current_menu->m_parent];
    return true;
  } else {
    return false;
  }
}

void MenuSystem::activate(void)
{
  string new_menu = m_current_menu->activate_item();
  if (!new_menu.empty()) {
    if (m_current_menu->m_parent != new_menu) {
      m_menus[new_menu]->m_selected = 0; // reset the index unless returning
      m_menus[new_menu]->m_parent = menup_to_str(m_current_menu);
    }
    m_current_menu = m_menus[new_menu];
  }
}

bool MenuSystem::key_action(int key)
{
  switch(key) {
    case GLFW_KEY_UP: if (m_current_menu) m_current_menu->prev_item(); break;
    case GLFW_KEY_DOWN: if (m_current_menu) m_current_menu->next_item(); break;
    case GLFW_KEY_LEFT: if (m_current_menu) m_current_menu->selected_item_prev_option(); break;
    case GLFW_KEY_RIGHT: if (m_current_menu) m_current_menu->selected_item_next_option(); break;

    case GLFW_KEY_ENTER:
    case GLFW_KEY_KP_ENTER:
      activate();
      break;
    case GLFW_KEY_ESC:
      if (!up_level())
        return true; // top exit
      break;
  }

  return false;
}

bool MenuSystem::mouse_action(int button)
{
  if (button==GLFW_MOUSE_BUTTON_LEFT) {
    activate();
  }

  return false;  
}

void MenuSystem::mouse_moved(void)
{
  if (m_current_menu) {
    vec2 pos = g_mouse.get_pos();
    m_current_menu->mouse_moved_to(pos);
  }
}

void MenuSystem::mouse_wheel(int dir)
{
  if (m_current_menu) {
    if (dir<0)
      m_current_menu->prev_item();
    else if (dir>0)
      m_current_menu->next_item();
  }
}


string MenuSystem::menup_to_str(Menu *menu)
{
  for(menustore_t::const_iterator it=m_menus.begin();it!=m_menus.end();++it) {
    if ((*it).second == menu)
      return ((*it).first);
  }
  return "";
}

Menu *MenuSystem::str_to_menup(const string &str)
{
  return m_menus[str];
}
