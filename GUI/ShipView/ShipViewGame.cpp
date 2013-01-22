#include "StdAfx.h"
#include "ShipViewGame.h"
#include "Ship.h"
#include "Renderer.h"


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

void ShipViewGame::render()
{
  ShipView::render();
  Renderer& renderer = Renderer::getInstance();
  DCoord headsTexSize = renderer.getHeadsTexSize();
  const list<Person*>& crew = ship_->getCrew();
  float dx = offsetX_*zoom_ + size_.left + size_.width*0.5f;
  float dy = offsetY_*zoom_ + size_.top + size_.height*0.5f;
  GLuint headsTex = renderer.getHeadsTex();
  for (Person* pers: crew) {
    if (pers->getPosition().deckIdx != activeDeckIdx_) {
      continue;
    }
    DCoord persCoord = pers->getPosition().coord;
    float tileX = persCoord.x*tileWidth_ + dx;
    float tileY = persCoord.y*tileHeight_ + dy;
    Rect texSize(256.0f/headsTexSize.x, 0.0f, 64.0f/headsTexSize.x, 64.0f/headsTexSize.y);
    int rotation = 0;
    float pathStepProgress = pers->getPathSteProgress();
    switch (pers->getDirection())
    {
    case Direction::Left:
      rotation = 3;
      tileX -= pathStepProgress*tileWidth_;
      break;
    case Direction::Up:
      tileY -= pathStepProgress*tileHeight_;
      rotation = 0;
      break;
    case Direction::Right:
      tileX += pathStepProgress*tileWidth_;
      rotation = 1;
      break;
    case Direction::Down:
      tileY += pathStepProgress*tileHeight_;
      rotation = 2;
      break;
    default:
      assert(0);
      break;
    }
    renderer.drawTexRect(Rect(tileX, tileY, tileWidth_, tileHeight_), headsTex, texSize, rotation);
  }
}
