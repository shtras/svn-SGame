#include "StdAfx.h"
#include "ShipViewGame.h"
#include "Ship.h"


ShipViewGame::ShipViewGame( Rect size ):ShipView(size)
{
  ship_ = new Ship(20, 20);
  ship_->load("saves/test2.sgs");
  ship_->normalize();
  activeDeck_ = ship_->getDeck(0);
  activeDeckIdx_ = 0;
  zoom_ = 1.0f;
  desiredZoom_ = zoom_;
  offsetX_ = -size_.width*0.5f/zoom_;
  offsetY_ = -size_.height*0.5f/zoom_;
}

ShipViewGame::~ShipViewGame()
{

}
