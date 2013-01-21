#pragma once
#include "Window.h"
#include "Button.h"
#include "ShipView.h"

class TopPanel: public Window
{
public:
  TopPanel(Rect size, ShipView* shipView);
  ~TopPanel();

  Button* LoadButton;
  Button* SaveButton;
  Button* MenuButton;
private:
  void constructionClick();
  void accessibilityClick();
  void powerclick();
  void oxygenClick();

  Button* constructionOverlayButton_;
  Button* accessibilityOverlayButton_;
  Button* powerOverlayButton_;
  Button* oxygenOverlayButton_;
  ShipView* shipView_;
};
