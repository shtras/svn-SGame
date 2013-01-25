#pragma once
#include "TexturedWidget.h"
#include "ShipViewGame.h"
#include "GameWorld.h"
#include "CrewList.h"

class GameWindow: public Widget, public has_slots<>
{
public:
  GameWindow(Rect size);
  ~GameWindow();
  bool init(GameWorld* world);
private:
  ShipViewGame* shipView_;
  GameWorld* world_;
  CrewList* crewList_;
};
