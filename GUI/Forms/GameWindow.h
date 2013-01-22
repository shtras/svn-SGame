#pragma once
#include "TexturedWidget.h"
#include "ShipViewGame.h"
#include "GameWorld.h"

class GameWindow: public Widget, public has_slots<>
{
public:
  GameWindow(Rect size);
  ~GameWindow();
  bool init(GameWorld* world);
private:
  ShipViewGame* shipView_;
  GameWorld* world_;
};
