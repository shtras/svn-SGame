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
  void onMouseWheelScroll(int direction);
private:
  void buildClick();
  void eraseClick();
  void floorClick();
  void doorClick();
  void categoryClick(void* param);
  ShipView* shipView_;
  Button* buildButton_;
  Button* floorButton_;
  Button* doorButton_;
  Button* eraseButton_;
  vector<Button*> categoryButtons_;
  map<int, vector<CompartmentButton*> > compButtons_;
  int firstButton_;
  Compartment::Category currentCategory_;
};
