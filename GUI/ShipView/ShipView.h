#pragma once
#include "TexturedWidget.h"
#include "Ship.h"
#include "CompHoverInfo.h"

class BuildInfo;

class ShipView: public Widget, public has_slots<>
{
public:
  enum OverviewType {Construction, Accessibility};
  ShipView(Rect size);
  ~ShipView();
  void render();
  void onRMDown();
  void onRMUp();
  void onMouseWheelScroll(int direction);
  void onMouseMove();
  void onHoverExit();
  int getActiveDeck() {return activeDeckIdx_;}
  int getGhostDeck() {return ghostDeckIdx_;}
  Ship* getShip() {return ship_;}
  void constructionOverview();
  void accessibilityOverview();
  void deckUp();
  void deckDown();
  void ghostDeckUp();
  void ghostDeckDown();
protected:
  void drawCompartments();
  void renderFloorSection(Deck* deck, int i, int j, Rect& tilePos, Rect& texPos);
  void structureChanged();
  bool isFloorOrStair(Deck* deck, int x, int y);
  Ship* ship_;
  int layoutWidth_;
  int layoutHeight_;
  float zoom_;
  float desiredZoom_;
  float zoomStep_;
  float offsetX_;
  float offsetY_;
  bool scrolling_;
  float lastMouseX_;
  float lastMouseY_;
  int hoveredLeft_;
  int hoveredTop_;
  int hoverWidth_;
  int hoverHeight_;
  Deck* activeDeck_;
  //vector<DeckView*> decks_;
  int activeDeckIdx_;
  int ghostDeckIdx_;
  float tileWidth_;
  float tileHeight_;
  bool drawing_;
  int drawingStartX_;
  int drawingStartY_;
  int tilesTexWidth_;
  int tilesTexHeight_;
  Compartment* hoveredComp_;
  bool editor_;
  CompHoverInfo* hoveredCompInfo_;
  OverviewType overviewType_;
  Compartment* selectedComp_;
};
