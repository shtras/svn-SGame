#include "StdAfx.h"
#include "ShipViewGame.h"
#include "Ship.h"


ShipViewGame::ShipViewGame( Rect size ):ShipView(size)
{
  activeDeckIdx_ = 0;
  zoom_ = 1.0f;
  desiredZoom_ = zoom_;
  offsetX_ = -size_.width*0.5f/zoom_;
  offsetY_ = -size_.height*0.5f/zoom_;
}

ShipViewGame::~ShipViewGame()
{

}

void ShipViewGame::setShip( Ship* ship )
{
  ship_ = ship;
  ship_->normalize();
  activeDeckIdx_ = 0;
  activeDeck_ = ship_->getDeck(activeDeckIdx_);
}
