#pragma once
#include "Window.h"
#include "Button.h"
#include "CompartmentButton.h"
#include "ProgressBar.h"
#include "ShipViewEditor.h"

class BuildTools: public Window
{
public:
  BuildTools(Rect size);
  ~BuildTools();
  void init(ShipViewEditor* shipView);
  void onMouseWheelScroll(int direction);
private:
  void buildClick();
  void eraseClick();
  void floorClick();
  void doorClick();
  void stairsClick();
  void selectClick();
  void entranceClick();
  void createConnectionClick();
  void eraseConnectionClick();
  void categoryClick(void* param);
  void upClick();
  void downClick();
  void ghostUpClick();
  void ghostDownClick();
  void updateGhostText();
  ShipViewEditor* shipView_;
  Button* buildButton_;
  Button* floorButton_;
  Button* doorButton_;
  Button* eraseButton_;
  Button* stairsButton_;
  Button* selectButton_;
  Button* entranceButton_;
  Button* connectionButton_;
  Button* eraseConnectionButton_;
  Text* deckText_;
  Text* ghostDeckText_;
  vector<Button*> categoryButtons_;
  map<int, vector<CompartmentButton*> > compButtons_;
  int firstButton_;
  Compartment::Category currentCategory_;
  ProgressBar* progressBar_;
};
