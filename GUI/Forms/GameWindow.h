#pragma once
#include "TexturedWidget.h"
#include "ShipViewGame.h"

class GameWindow: public Widget, public has_slots<>
{
public:
  GameWindow(Rect size);
  ~GameWindow();
private:
  ShipViewGame* shipView_;
};
