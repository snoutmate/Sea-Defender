#pragma once

#include "misc.h"

class FX
{
public:
  virtual void update(void) = 0;
  virtual void draw(void) = 0;
  virtual bool finished(void) = 0;
  virtual bool running(void) = 0;

  virtual ~FX() {};
};


typedef pair<FX*,int> fx_item_t;
typedef list<fx_item_t> fx_items_ary_t;

class FX_stack {
  fx_items_ary_t m_effects;
public:
  void add_effect(FX *effect,int draw_order_id);
  void update(void);
  void draw(int id);
  void draw_all(void);

  void clear(void);
};
