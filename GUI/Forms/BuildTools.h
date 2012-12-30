#pragma once
#include "Window.h"
#include "ShipView.h"
#include "Button.h"

class BuildTools: public Window
{
public:
  BuildTools(Rect size);
  ~BuildTools();
  void init(ShipView* shipView);
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
};
