#pragma once
#include "ShipView.h"

class ShipViewGame: public ShipView
{
public:
  ShipViewGame(Rect size);
  ~ShipViewGame();
  void setShip(Ship* ship);
private:
};
