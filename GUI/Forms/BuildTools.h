#pragma once
#include "Window.h"
#include "ShipView.h"
#include "Button.h"
#include "CompartmentButton.h"
#include "ProgressBar.h"

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
  void selectClick();
  void entranceClick();
  void createConnectionClick();
  void categoryClick(void* param);
  void upClick();
  void downClick();
  void ghostUpClick();
  void ghostDownClick();
  void updateGhostText();
  ShipView* shipView_;
  Button* buildButton_;
  Button* floorButton_;
  Button* doorButton_;
  Button* eraseButton_;
  Button* selectButton_;
  Button* entranceButton_;
  Button* connectionButton_;
  Text* deckText_;
  Text* ghostDeckText_;
  vector<Button*> categoryButtons_;
  map<int, vector<CompartmentButton*> > compButtons_;
  int firstButton_;
  Compartment::Category currentCategory_;
  ProgressBar* progressBar_;
};
