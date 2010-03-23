
#include "fx.h"


void FX_stack::add_effect(FX *effect,int draw_order_id)
{
  m_effects.push_back(make_pair(effect,draw_order_id));
}

void FX_stack::update(void)
{
  for(fx_items_ary_t::iterator it=m_effects.begin();it!=m_effects.end();) {
    FX *effect = it->first;

    effect->update();

    if (effect->finished()) {
      delete effect;
      it = m_effects.erase(it);
    } else {
      ++it;
    }
  }
}

void FX_stack::draw(int id)
{
  for(fx_items_ary_t::iterator it=m_effects.begin();it!=m_effects.end();++it) {
    int fx_draw_id = it->second;
    if (fx_draw_id != id)
      continue;
  
    FX *effect = it->first;
    effect->draw();
  }
}

void FX_stack::draw_all(void)
{
  for(fx_items_ary_t::iterator it=m_effects.begin();it!=m_effects.end();++it) {
    FX *effect = it->first;
    effect->draw();
  }
}

void FX_stack::clear(void)
{
  for(fx_items_ary_t::iterator it=m_effects.begin();it!=m_effects.end();++it) {
    FX *effect = it->first;
    delete effect;
  }

  m_effects.clear();
}
