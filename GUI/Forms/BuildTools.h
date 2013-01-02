#pragma once
#include "Window.h"
#include "ShipView.h"
#include "Button.h"
#include "CompartmentButton.h"

class BuildTools: public Window
{
public:
  BuildTools(Rect size);
  ~BuildTools();
  void init(ShipView* shipView);
  void onClick();
  void render();
  void onMouseWheelScroll(int direction);
private:
  void buildClick();
  void eraseClick();
  void floorClick();
  void doorClick();
  ShipView* shipView_;
  Button* buildButton_;
  Button* floorButton_;
  Button* doorButton_;
  Button* eraseButton_;
  vector<CompartmentButton*> compButtons_;
  int firstButton_;
  int lastButton_;
};
