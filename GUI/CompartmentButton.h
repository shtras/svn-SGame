#pragma once
#include "Button.h"
#include "Ship.h"
#include "Window.h"
#include "ShipViewEditor.h"

class CompartmentButton: public Button
{
public:
  CompartmentButton(Rect size, Compartment* comp, ShipViewEditor* shipView);
  ~CompartmentButton();
  void render();
  void onDrag();
  void onStopDrag();
  void onHoverEnter();
  void onHoverExit();
  Compartment* getCompartment() {return comp_;}
private:
  Compartment* comp_;
  ShipViewEditor* shipView_;
  Window* hoverWindow_;
};
