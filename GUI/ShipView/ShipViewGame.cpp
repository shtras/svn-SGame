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
  map<int, int> crewInTiles;
  GLuint headsTex = renderer.getHeadsTex();
  for (Person* pers: crew) {
    if (pers->getPosition().deckIdx != activeDeckIdx_) {
      continue;
    }
    DCoord persCoord = pers->getPosition().coord;
    int x = persCoord.x;
    int y = persCoord.y;
    if (crewInTiles.count(x*100+y) == 0) {
      crewInTiles[x*100+y] = 1;
    } else {
      crewInTiles[x*100+y]++;
    }
    int crewInTile = crewInTiles[x*100+y];
    float tileX = x*tileWidth_ + dx + crewInTile * 0.05f * tileWidth_;
    float tileY = y*tileHeight_ + dy + crewInTile * 0.05f * tileHeight_;
    if (tileX < size_.left || tileY < size_.top || tileX + tileWidth_ > size_.left + size_.width || tileY + tileHeight_ > size_.top + size_.height) {
      continue;
    }
    Rect texSize(pers->getHeadTexX(), 0.0f, 64.0f/headsTexSize.x, 64.0f/headsTexSize.y);
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
