#pragma once
#include "Button.h"
#include "Ship.h"
#include "ShipView.h"
#include "Window.h"

class CompartmentButton: public Button
{
public:
  CompartmentButton(Rect size, Compartment* comp, ShipView* shipView);
  ~CompartmentButton();
  void render();
  void onDrag();
  void onStopDrag();
  void onHoverEnter();
  void onHoverExit();
  Compartment* getCompartment() {return comp_;}
private:
  Compartment* comp_;
  ShipView* shipView_;
  Window* hoverWindow_;
};
