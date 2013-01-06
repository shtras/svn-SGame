#include "StdAfx.h"
#include "ShipView.h"
#include "Renderer.h"
#include "CompartmentButton.h"

ShipView::ShipView(Rect size):Widget(size),layoutWidth_(50), layoutHeight_(50), zoom_(0.5f), offsetX_(0.0f), offsetY_(0.0f),
  scrolling_(false), lastMouseX_(0), lastMouseY_(0),hoveredLeft_(-1), hoveredTop_(-1), hoverWidth_(1), hoverHeight_(1),tileWidth_(0),
  tileHeight_(0),drawing_(false),drawingStartX_(-1), drawingStartY_(-1),desiredZoom_(1.0f),zoomStep_(0),action_(BuildWalls),
  tilesTexWidth_(0), tilesTexHeight_(0), hoveredComp_(NULL),activeDeckIdx_(-1),activeDeck_(NULL),buildInfo_(NULL),hoveredCompInfo_(NULL)
{
  tilesTexWidth_ = Renderer::getInstance().getTilesTexWidth();
  tilesTexHeight_ = Renderer::getInstance().getTilesTexHeight();
  //for (int i=0; i<3; ++i) {
  //  DeckView* deck = new DeckView(layoutWidth_, layoutHeight_);
  //  decks_.push_back(deck);
  //}
  ship_ = new Ship(layoutWidth_, layoutHeight_);
  activeDeck_ = ship_->getDeck(0);
  activeDeckIdx_ = 0;
  desiredZoom_ = zoom_;
  hoveredCompInfo_ = new Window(Rect(0.0, 0.0, 0.2, 0.2));
}

ShipView::~ShipView()
{
  delete hoveredCompInfo_;
}

void ShipView::render()
{
  if (fabs(zoom_ - desiredZoom_) > 0.0001f) {
    zoom_ += zoomStep_;
  }
  Renderer& renderer = Renderer::getInstance();
  renderer.setColor(Vector4(255,255,255,100));
  renderer.drawTexRect(size_, renderer.getBGTex(), Rect(0,0,1,1));
  renderer.resetColor();
  renderer.setTextSize(1.0f * zoom_);
  float aspect = renderer.getWidth() / (float)renderer.getHeight();
  //Renderer::getInstance().flushVerts();
  //glScissor(50, 50, 250, 250);
  tileWidth_ = size_.width * zoom_ * 0.05f;
  tileHeight_ = tileWidth_ * aspect;
  GLuint tilesTex = renderer.getTilesTex();
  int drawStartX = drawingStartX_;
  int drawStartY = drawingStartY_;
  int drawEndX = hoveredLeft_;
  int drawEndY = hoveredTop_;
  if (drawStartX > drawEndX) {
    int tmp = drawEndX;
    drawEndX = drawStartX;
    drawStartX = tmp;
  }
  if (drawStartY > drawEndY) {
    int tmp = drawEndY;
    drawEndY = drawStartY;
    drawStartY = tmp;
  }
  float tileTexWidth = 62 / (float)tilesTexWidth_;
  float tileTexHeight = 62 / (float)tilesTexHeight_;
  for (int i=0; i<layoutWidth_; ++i) {
    float tileX = i*tileWidth_ + offsetX_*zoom_ + size_.left;
    if (tileX < size_.left || tileX+tileWidth_ > size_.left + size_.width) {
      continue;
    }
    for (int j=0; j<layoutHeight_; ++j) {
      float tileY = j*tileHeight_ + offsetY_*zoom_ + size_.top;
      if (tileY < size_.top || tileY+tileHeight_ > size_.top + size_.height) {
        continue;
      }
      //if (lastMouseX_ >= tileX && lastMouseX_ <= tileX + tileWidth_*hoverWidth_ && lastMouseY_ >= tileY && lastMouseY_ <= tileY + tileHeight_*hoverHeight_) {
      //  renderer.setColor(Vector4(100,255,50,200));
      //}
      Rect tilePos(tileX, tileY, tileWidth_, tileHeight_);
      Rect texPos(0.0f, 0.0f, tileTexWidth, tileTexHeight);
      int wallValue = activeDeck_->getTileType(i, j);
      int wallCode = 0;
      bool renderPrevFloorCorners = false;
      if (wallValue == Tile::Wall || wallValue == Tile::Door) {
        wallCode = activeDeck_->getWallCode(i, j);
      }
      if (wallValue == Tile::Wall && wallCode == 0) {
        wallCode = 10; //horizontal wall;
      }
      if (i >= hoveredLeft_ && j >= hoveredTop_ && i < hoveredLeft_ + hoverWidth_ && j < hoveredTop_ + hoverHeight_) {
        renderer.setColor(Vector4(155,255,55,200));
        CString coord = CString(i) + "," + CString(j);
        if (drawing_) {
          coord += " " + CString(drawEndX - drawStartX + 1) + "," + CString(drawEndY - drawStartY + 1);
        }
        renderer.renderText(tileX + tileWidth_*0.5f - renderer.getCharWidth()*coord.getSize()*0.5f, tileY + tileHeight_*0.5f - renderer.getCharHeight()*0.5f, coord);
      } else if (wallValue == Tile::Empty) {
        if (activeDeckIdx_ > 0) {
          int prevWallValue = ship_->getDeck(activeDeckIdx_-1)->getTileType(i, j);
          if (prevWallValue != Tile::Empty) {
            wallValue = prevWallValue;
            wallCode = ship_->getDeck(activeDeckIdx_-1)->getWallCode(i, j);
            if (wallValue != Tile::Floor) {
              renderPrevFloorCorners = true;
            }
          }
        }
        renderer.setColor(Vector4(255,255,255,50));
      }
      if (hoveredComp_ && i >= hoveredComp_->getX() && i < hoveredComp_->getX() + hoveredComp_->getWidth() &&
          j >= hoveredComp_->getY() && j < hoveredComp_->getY() + hoveredComp_->getHeight()) {
        renderer.setColor(Vector4(125, 200, 210, 255));
      }
      if (drawing_ && i >= drawStartX && i <= drawEndX && j >= drawStartY && j <= drawEndY) {
        renderer.setColor(Vector4(255,155,55,200));
      }
      if (wallValue == Tile::Wall || wallValue == Tile::Door) {
        renderFloorSection(activeDeck_, i, j, tilePos, texPos);
      }
      if (renderPrevFloorCorners) {
        //renderFloorSection(ship_->getDeck(activeDeckIdx_-1), i, j, tilePos, texPos);
      }
      texPos.top = wallCode / 4 * 64.0f / (float)tilesTexHeight_ + 1/(float)tilesTexHeight_;
      texPos.left = wallCode % 4 * 64.0f / (float)tilesTexWidth_ + 1/(float)tilesTexWidth_;
      if (wallValue == Tile::Floor) {
        texPos.left = 257.0f / (float)tilesTexWidth_;
        texPos.top = 1.0f / (float)tilesTexHeight_;
      }
      if (wallValue == Tile::Door) {
        assert (wallCode == 10 || wallCode == 5);
        texPos.left = 257.0f / tilesTexWidth_;
        if (wallCode == 10) {
          texPos.top = 65.0f / tilesTexHeight_;
        } else if (wallCode == 5) {
          texPos.top = 129.0f / tilesTexHeight_;
        }
      }
      //texPos.left += 0.002f;
      //texPos.top += 0.004f;
      //texPos.width -= 0.004f;
      //texPos.height -= 0.008f;
      renderer.drawTexRect(tilePos, tilesTex, texPos);
      renderer.setColor(Vector4(255,255,255,255));
    }
  }
  drawCompartments();
  //glScissor(0, 0, 1600, 900);
}

void ShipView::renderFloorSection(Deck* deck, int i, int j, Rect& tilePos, Rect& texPos)
{
  //@TODO: Move this info to Tile class
  Renderer& renderer = Renderer::getInstance();
  GLuint texTile = renderer.getTilesTex();
  float texX = 257.0f / (float)tilesTexWidth_;
  float texY = 1.0f / (float)tilesTexHeight_;
  if (deck->getTileType(i-1, j) == Tile::Floor || deck->getTileType(i, j-1) == Tile::Floor || deck->getTileType(i-1, j-1) == Tile::Floor) {
    Rect upLeft       (tilePos.left, tilePos.top, tilePos.width*0.5f, tilePos.height*0.5f);
    Rect texUpLeft    (texX,         texY,        texPos.width*0.5f,  texPos.height*0.5f);
    renderer.drawTexRect(upLeft, texTile, texUpLeft);
  }
  if (deck->getTileType(i+1, j) == Tile::Floor || deck->getTileType(i, j-1) == Tile::Floor || deck->getTileType(i+1, j-1) == Tile::Floor) {
    Rect upRight      (tilePos.left + tilePos.width*0.5f, tilePos.top, tilePos.width*0.5f, tilePos.height*0.5f);
    Rect texUpRight   (texX + texPos.width*0.5f,          texY,        texPos.width*0.5f,  texPos.height*0.5f);
    renderer.drawTexRect(upRight, texTile, texUpRight);
  }
  if (deck->getTileType(i, j+1) == Tile::Floor || deck->getTileType(i+1, j) == Tile::Floor || deck->getTileType(i+1, j+1) == Tile::Floor) {
    Rect downRight    (tilePos.left + tilePos.width*0.5f, tilePos.top+tilePos.height*0.5f, tilePos.width*0.5f, tilePos.height*0.5f);
    Rect texDownRight (texX + texPos.width*0.5f,          texY+texPos.height*0.5f,         texPos.width*0.5f,  texPos.height*0.5f);
    renderer.drawTexRect(downRight, texTile, texDownRight);
  }
  if (deck->getTileType(i, j+1) == Tile::Floor || deck->getTileType(i-1, j) == Tile::Floor || deck->getTileType(i-1, j+1) == Tile::Floor) {
    Rect downLeft     (tilePos.left, tilePos.top + tilePos.height*0.5f, tilePos.width*0.5f, tilePos.height*0.5f);
    Rect texDownLeft  (texX,         texY + texPos.height*0.5f,         texPos.width*0.5f,  texPos.height*0.5f);
    renderer.drawTexRect(downLeft, texTile, texDownLeft);
  }
}

void ShipView::drawCompartments()
{
  Renderer& renderer = Renderer::getInstance();
  GLuint tilesTex = renderer.getTilesTex();
  for (auto itr = activeDeck_->getCompartments().begin(); itr != activeDeck_->getCompartments().end(); ++itr) {
    Compartment* comp = *itr;
    if (hoveredComp_ == comp) {
      renderer.setColor(Vector4(125, 200, 210, 255));
    } else {
      renderer.setColor(Vector4(255,255,255,255));
    }
    for (auto itemItr = comp->getItems().begin(); itemItr != comp->getItems().end(); ++itemItr) {
      Item* item = *itemItr;
      int x = item->getX() + comp->getX();
      int y = item->getY() + comp->getY();
      float tileX = x*tileWidth_ + offsetX_*zoom_ + size_.left;
      float tileY = y*tileHeight_ + offsetY_*zoom_ + size_.top;
      if (tileX < size_.left || tileX+tileWidth_ > size_.left + size_.width) {
        continue;
      }
      if (tileY < size_.top || tileY+tileHeight_ > size_.top + size_.height) {
        continue;
      }
      Rect pos(tileX, tileY, tileWidth_, tileHeight_);
      Rect texPos(item->getTexX() + item->getRotation() * item->getTexWidth() + item->getTexWidth()*0.01f, item->getTexY() + item->getTexHeight()*0.01f, item->getTexWidth()*0.98f, item->getTexHeight()*0.98f);
      renderer.drawTexRect(pos, tilesTex, texPos);
    }
    renderer.resetColor();
  }
}

void ShipView::onRMDown()
{
  scrolling_ = true;
  float mouseX = Renderer::getInstance().getMouseX();
  float mouseY = Renderer::getInstance().getMouseY();
  lastMouseX_ = mouseX;
  lastMouseY_ = mouseY;
}

void ShipView::onRMUp()
{
  scrolling_ = false;
}

void ShipView::onHoverExit()
{
  scrolling_ = false;
  drawing_ = false;
  hoveredLeft_ = -1;
  hoveredTop_ = -1;
  drawingStartX_ = -1;
  drawingStartY_ = -1;
  Renderer::getInstance().setFloatingWidget(NULL);
}

void ShipView::onMouseMove()
{
  float mouseX = Renderer::getInstance().getMouseX();
  float mouseY = Renderer::getInstance().getMouseY();
  if(scrolling_) {
    offsetX_ += (mouseX - lastMouseX_)/zoom_;
    offsetY_ += (mouseY - lastMouseY_)/zoom_;
  }
  lastMouseX_ = mouseX;
  lastMouseY_ = mouseY;

  //    float tileX = i*tileWidth_ + offsetX_*zoom_ + size_.left;

  hoveredLeft_ = (int)(((lastMouseX_-size_.left) - offsetX_*zoom_)/tileWidth_);
  hoveredTop_ = (int)(((lastMouseY_ - size_.top) - offsetY_*zoom_)/tileHeight_);
  hoveredComp_ = activeDeck_->getCompartment(hoveredLeft_, hoveredTop_);
  if (hoveredComp_) {
    Renderer::getInstance().setFloatingWidget(hoveredCompInfo_);
  } else {
    Renderer::getInstance().setFloatingWidget(NULL);
  }
}

void ShipView::onMouseWheelScroll(int direction)
{
  if (direction > 0) {
    if (desiredZoom_ > 3.0f) {
      return;
    }
    desiredZoom_ *= 1.05f;
  } else {
    if (desiredZoom_ < 0.35f) {
      return;
    }
    desiredZoom_ /= 1.05f;
  }
  zoomStep_ = (desiredZoom_ - zoom_)*0.1f;
}

void ShipView::onLMDown()
{
  drawing_ = true;
  drawingStartX_ = hoveredLeft_;
  drawingStartY_ = hoveredTop_;
}

void ShipView::onLMUp()
{
  if (action_ == BuildWalls) {
    plantWalls();
  } else if (action_ == Erase || action_ == BuildFloor) {
    eraseArea();
  } else if (action_ == BuildDoor) {
    activeDeck_->setDoor(hoveredLeft_, hoveredTop_);
  }
  drawing_ = false;
}

void ShipView::onDrop(Widget* w)
{
  CompartmentButton* cb = dynamic_cast<CompartmentButton*>(w);
  if (!cb) {
    return;
  }
  Compartment* comp = cb->getCompartment();
  //Check overlapping
  int x = hoveredLeft_;
  int y = hoveredTop_;
  int width = comp->getWidth();
  int height = comp->getHeight();
  for (auto itr = activeDeck_->getCompartments().begin(); itr != activeDeck_->getCompartments().end(); ++itr) {
    Compartment* otherComp = *itr;
    int otherX = otherComp->getX();
    int otherY = otherComp->getY();
    int otherWidth = otherComp->getWidth();
    int otherHeight = otherComp->getHeight();
    if (otherX + otherWidth > x && otherX < x + width && otherY + otherHeight > y && otherY < y + height) {
      //Overlap
      return;
    }
    if (x < 0 || y < 0 || x + width >= layoutWidth_ || y + height >= layoutHeight_) {
      return;
    }
  }
  for (int i=x; i<x+width; ++i) {
    for (int j=y; j<y+height; ++j) {
      if (activeDeck_->getTileType(i,j) != Tile::Floor && activeDeck_->getTileType(i,j) != Tile::Empty) {
        //Wall or door
        return;
      }
    }
  }
  Compartment* newComp = new Compartment(*comp);
  newComp->setX(hoveredLeft_);
  newComp->setY(hoveredTop_);
  activeDeck_->addCompartment(newComp);
  buildInfo_->updateValues(ship_);
}

void ShipView::plantWalls()
{
  if (hoveredLeft_ < 0 || hoveredTop_ < 0 || drawingStartX_ < 0 || drawingStartY_ < 0) {
    return;
  }
  if (hoveredLeft_ >= layoutWidth_ || hoveredTop_ >= layoutHeight_ || drawingStartX_ >= layoutWidth_ || drawingStartY_ >= layoutHeight_) {
    return;
  }
  action_ = BuildFloor;
  eraseArea();
  action_ = BuildWalls;
  int stepHor = drawingStartX_ > hoveredLeft_?-1:1;
  int stepVer = drawingStartY_ > hoveredTop_?-1:1;
  for (int i=drawingStartX_; i!= hoveredLeft_ + stepHor; i += stepHor) {
    activeDeck_->setTileType(i, drawingStartY_, Tile::Wall);
    activeDeck_->setTileType(i, hoveredTop_, Tile::Wall);
  }
  for (int i=drawingStartY_; i != hoveredTop_ + stepVer; i += stepVer) {
    activeDeck_->setTileType(drawingStartX_, i, Tile::Wall);
    activeDeck_->setTileType(hoveredLeft_, i, Tile::Wall);
  }
}

void ShipView::eraseArea()
{
  if (hoveredLeft_ < 0 || hoveredTop_ < 0 || drawingStartX_ < 0 || drawingStartY_ < 0) {
    return;
  }
  if (hoveredLeft_ >= layoutWidth_ || hoveredTop_ >= layoutHeight_ || drawingStartX_ >= layoutWidth_ || drawingStartY_ >= layoutHeight_) {
    return;
  }
  int stepHor = drawingStartX_ > hoveredLeft_?-1:1;
  int stepVer = drawingStartY_ > hoveredTop_?-1:1;
  Tile::TileType value = Tile::Empty;
  if (action_ == BuildFloor) {
    value = Tile::Floor;
  }
  for (int i=drawingStartX_; i!= hoveredLeft_ + stepHor; i += stepHor) {
    for (int j=drawingStartY_; j != hoveredTop_ + stepVer; j += stepVer) {
      if (value == Tile::Floor && activeDeck_->getTileType(i, j) != Tile::Empty) {
        continue;
      }
      if (value == Tile::Empty && activeDeck_->getTileType(i, j) == Tile::Wall) {
        activeDeck_->eraseDoorsAround(i, j);
      }
      if (value == Tile::Empty) {
        Compartment* comp = activeDeck_->getCompartment(i,j);
        if (comp) {
          activeDeck_->removeCompartment(comp);
          buildInfo_->updateValues(ship_);
          assert (!activeDeck_->getCompartment(i,j));
        }
      }
      activeDeck_->setTileType(i, j, value);
    }
  }
}

void ShipView::buildWalls()
{
  action_ = BuildWalls;
}

void ShipView::erase()
{
  action_ = Erase;
}

void ShipView::buildFloor()
{
  action_ = BuildFloor;
}

void ShipView::buildDoor()
{
  action_ = BuildDoor;
}

void ShipView::setHoveredDimensions(int width, int height)
{
  hoverWidth_ = width;
  hoverHeight_ = height;
}

void ShipView::deckUp()
{
  ++activeDeckIdx_;
  if (activeDeckIdx_ > 2) {
    activeDeckIdx_ = 2;
  }
  activeDeck_ = ship_->getDeck(activeDeckIdx_);
}

void ShipView::deckDown()
{
  --activeDeckIdx_;
  if (activeDeckIdx_ < 0) {
    activeDeckIdx_ = 0;
  }
  activeDeck_ = ship_->getDeck(activeDeckIdx_);
}

void ShipView::setBuildInfo( BuildInfo* info )
{
  assert(info);
  buildInfo_ = info;
  buildInfo_->updateValues(ship_);
}

//DeckView::DeckView(int width, int height):width_(width), height_(height)
//{
//  wallLayout_ = new TileType[height_ * width_];
//  for (int i=0; i<height_ * width_; ++i) {
//    wallLayout_[i] = Empty;
//  }
//}
//
//DeckView::~DeckView()
//{
//  delete[] wallLayout_;
//  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
//    delete *itr;
//  }
//}
//
//Compartment* DeckView::getCompartment(int x, int y)
//{
//  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
//    Compartment* comp = *itr;
//    if (x >= comp->getX() && x < comp->getX() + comp->getWidth() && y >= comp->getY() && y < comp->getY() + comp->getHeight()) {
//      return comp;
//    }
//  }
//  return NULL;
//}
//
//int DeckView::getWallCode( int i, int j )
//{
//  int wallCode = 0;
//  wallCode |= hasWall(i, j+1);
//  wallCode |= (hasWall(i+1, j) << 1);
//  wallCode |= (hasWall(i, j-1) << 2);
//  wallCode |= (hasWall(i-1, j) << 3);
//  assert (wallCode >= 0 && wallCode < 16);
//  return wallCode;
//}
//
//void DeckView::setDoor( int x, int y )
//{
//  TileType type = getWall(x, y);
//  if (type != Wall) {
//    return;
//  }
//  int wallCode = getWallCode(x, y);
//  if (wallCode != 10 && wallCode != 5) {
//    //Not a horizontal or vertical straight wall
//    return;
//  }
//  setWall(x, y, Door);
//}
//
//DeckView::TileType DeckView::getWall( int x, int y )
//{
//  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
//    return Empty;
//  }
//  return wallLayout_[y*width_ + x];
//}
//
//int DeckView::hasWall( int x, int y )
//{
//  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
//    return 0;
//  }
//  return (wallLayout_[y*width_ + x]==Wall || wallLayout_[y*width_ + x]==Door)?1:0;
//}
//
//void DeckView::setWall( int x, int y, TileType value )
//{
//  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
//    assert(0);
//  }
//  if (value == Wall || value == Floor || value == Door) {
//    if (hasDoorsAround(x, y) && value != Floor) {
//      //Can't create doors or walls around a door
//      return;
//    }
//    if (getWall(x, y) == Door) {
//      //Can only erase door, not replace
//      return;
//    }
//  }
//  if ((value == Wall || value == Door) && getCompartment(x,y)) {
//    //Can't build over compartment
//    return;
//  }
//  wallLayout_[y*width_ + x] = value;
//}
//
//bool DeckView::hasDoorsAround( int x, int y )
//{
//  return getWall(x-1,y) == Door || getWall(x+1, y) == Door || getWall(x, y-1) == Door || getWall(x, y+1) == Door;
//}
//
//void DeckView::eraseDoorsAround( int x, int y )
//{
//  if (getWall(x-1,y) == Door) {
//    setWall(x-1, y, Empty);
//  }
//  if (getWall(x+1,y) == Door) {
//    setWall(x+1, y, Empty);
//  }
//  if (getWall(x,y-1) == Door) {
//    setWall(x, y-1, Empty);
//  }
//  if (getWall(x,y+1) == Door) {
//    setWall(x, y+1, Empty);
//  }
//}
//
//void DeckView::addCompartment( Compartment* comp )
//{
//  compartments_.push_back(comp);
//}
//
//void DeckView::removeCompartment( Compartment* comp )
//{
//  compartments_.remove(comp);
//}

