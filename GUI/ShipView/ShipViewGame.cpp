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
    if (pers->isHighLighted()) {
      renderer.setColor(Vector4(255,200,100,255));
    } else {
      renderer.setColor(Vector4(255,255,255,255));
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
    if (tileX+tileWidth_ < size_.left || tileY+tileHeight_ < size_.top || tileX > size_.left + size_.width || tileY > size_.top + size_.height) {
      continue;
    }
    float texX = ((int)((pers->getPathSteProgress()) * 7.78f) + 1) * 64.0f / (float)headsTexSize.x;
    Rect texSize(/*pers->getHeadTexX()*/texX, 0.0f, 64.0f/headsTexSize.x, 64.0f/headsTexSize.y);
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
    texSize.left = pers->getHeadTexX();
    texSize.top = 128.0f / headsTexSize.y;
    renderer.drawTexRect(Rect(tileX + tileWidth_*0.25f, tileY + tileHeight_*0.25f, tileWidth_*0.5f, tileHeight_*0.5f), headsTex, texSize, rotation);
    renderer.setColor(Vector4(255,255,255,255));
  }
  renderer.setColor(Vector4(255,255,255,255));
}
