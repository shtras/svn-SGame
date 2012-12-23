#pragma once
#include "TexturedWidget.h"

class GlobalWindow: public Widget, public has_slots<>
{
public:
  GlobalWindow(Rect size);
  ~GlobalWindow();
  void onDrop(Widget* w);
private:
};
