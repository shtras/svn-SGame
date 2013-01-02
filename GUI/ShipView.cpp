#include "StdAfx.h"
#include "ShipView.h"
#include "Renderer.h"
#include "CompartmentButton.h"

ShipView::ShipView(Rect size):Widget(size),layoutWidth_(50), layoutHeight_(50), zoom_(1.0f), offsetX_(0.0f), offsetY_(0.0f),
  scrolling_(false), lastMouseX_(0), lastMouseY_(0),hoveredLeft_(-1), hoveredTop_(-1), hoverWidth_(1), hoverHeight_(1),tileWidth_(0),
  tileHeight_(0),drawing_(false),drawingStartX_(-1), drawingStartY_(-1),desiredZoom_(1.0f),zoomStep_(0),action_(BuildWalls),
  tilesTexWidth_(0), tilesTexHeight_(0)
{
  wallLayout_ = new TileType[layoutHeight_ * layoutWidth_];
  for (int i=0; i<layoutHeight_ * layoutWidth_; ++i) {
    wallLayout_[i] = Empty;
  }
  tilesTexWidth_ = Renderer::getInstance().getTilesTexWidth();
  tilesTexHeight_ = Renderer::getInstance().getTilesTexHeight();
}

ShipView::~ShipView()
{
  delete[] wallLayout_;
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    delete *itr;
  }
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
  renderer.setTextSize(0.8f);
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
      int wallValue = getWall(i, j);
      int wallCode = 0;
      if (wallValue == Wall || wallValue == Door) {
        wallCode = getWallCode(i, j);
      }
      if (wallValue == Wall && wallCode == 0) {
        wallCode = 10; //horizontal wall;
      }
      if (i >= hoveredLeft_ && j >= hoveredTop_ && i < hoveredLeft_ + hoverWidth_ && j < hoveredTop_ + hoverHeight_) {
        renderer.setColor(Vector4(155,255,55,200));
        CString coord = CString(i) + "," + CString(j);
        if (drawing_) {
          coord += " " + CString(drawEndX - drawStartX) + "," + CString(drawEndY - drawStartY);
        }
        renderer.renderText(tileX + tileWidth_*0.5f - renderer.getCharWidth()*coord.getSize()*0.5f, tileY + tileHeight_*0.5f - renderer.getCharHeight()*0.5f, coord);
      } else if (wallValue == Empty) {
        renderer.setColor(Vector4(255,255,255,50));
      }
      if (drawing_ && i >= drawStartX && i <= drawEndX && j >= drawStartY && j <= drawEndY) {
        renderer.setColor(Vector4(255,155,55,200));
      }
      texPos.top = wallCode / 4 * 64.0f / (float)tilesTexHeight_ + 1/(float)tilesTexHeight_;
      texPos.left = wallCode % 4 * 64.0f / (float)tilesTexWidth_ + 1/(float)tilesTexWidth_;
      if (wallValue == Floor) {
        texPos.left = 257.0f / (float)tilesTexWidth_;
        texPos.top = 1.0f / (float)tilesTexHeight_;
      }
      if (wallValue == Door) {
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
  renderer.setTextSize(1);
  renderer.renderText(0, 0, CString(zoom_, 5) + " " + CString(desiredZoom_, 5) + " " + CString(zoomStep_, 5));
  drawCompartments();
  //glScissor(0, 0, 1600, 900);
}

void ShipView::drawCompartments()
{
  Renderer& renderer = Renderer::getInstance();
  GLuint tilesTex = renderer.getTilesTex();
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    Compartment* comp = *itr;
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
      Rect texPos(item->getTexX() + item->getRotation() * item->getTexWidth(), item->getTexY(), item->getTexWidth(), item->getTexHeight());
      renderer.drawTexRect(pos, tilesTex, texPos);
    }
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
    setDoor(hoveredLeft_, hoveredTop_);
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
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    Compartment* otherComp = *itr;
    int otherX = otherComp->getX();
    int otherY = otherComp->getY();
    int otherWidth = otherComp->getWidth();
    int otherHeight = otherComp->getHeight();
    if (otherX + otherWidth >= x && otherX < x + width && otherY + otherHeight >= y && otherY < y + height) {
      //Overlap
      return;
    }
  }
  for (int i=x; i<x+width; ++i) {
    for (int j=y; j<y+height; ++j) {
      if (getWall(i,j) != Floor && getWall(i,j) != Empty) {
        //Wall or door
        return;
      }
    }
  }
  Compartment* newComp = new Compartment(*comp);
  newComp->setX(hoveredLeft_);
  newComp->setY(hoveredTop_);
  compartments_.push_back(newComp);
}

ShipView::TileType ShipView::getWall( int x, int y )
{
  if (x < 0 || x >= layoutWidth_ || y < 0 || y >= layoutHeight_) {
    return Empty;
  }
  return wallLayout_[y*layoutWidth_ + x];
}

int ShipView::hasWall( int x, int y )
{
  if (x < 0 || x >= layoutWidth_ || y < 0 || y >= layoutHeight_) {
    return 0;
  }
  return (wallLayout_[y*layoutWidth_ + x]==Wall || wallLayout_[y*layoutWidth_ + x]==Door)?1:0;
}

void ShipView::setWall( int x, int y, TileType value )
{
  if (x < 0 || x >= layoutWidth_ || y < 0 || y >= layoutHeight_) {
    assert(0);
  }
  if (value == Wall || value == Floor || value == Door) {
    if (hasDoorsAround(x, y) && value != Floor) {
      //Can't create doors or walls around a door
      return;
    }
    if (getWall(x, y) == Door) {
      //Can only erase door, not replace
      return;
    }
  }
  if ((value == Wall || value == Door) && getCompartment(x,y)) {
    //Can't build over compartment
    return;
  }
  wallLayout_[y*layoutWidth_ + x] = value;
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
    setWall(i, drawingStartY_, Wall);
    setWall(i, hoveredTop_, Wall);
  }
  for (int i=drawingStartY_; i != hoveredTop_ + stepVer; i += stepVer) {
    setWall(drawingStartX_, i, Wall);
    setWall(hoveredLeft_, i, Wall);
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
  TileType value = Empty;
  if (action_ == BuildFloor) {
    value = Floor;
  }
  for (int i=drawingStartX_; i!= hoveredLeft_ + stepHor; i += stepHor) {
    for (int j=drawingStartY_; j != hoveredTop_ + stepVer; j += stepVer) {
      if (value == Floor && getWall(i, j) != Empty) {
        continue;
      }
      if (value == Empty && getWall(i, j) == Wall) {
        eraseDoorsAround(i, j);
      }
      if (value == Empty) {
        Compartment* comp = getCompartment(i,j);
        if (comp) {
          compartments_.remove(comp);
          assert (!getCompartment(i,j));
        }
      }
      setWall(i, j, value);
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

Compartment* ShipView::getCompartment(int x, int y)
{
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    Compartment* comp = *itr;
    if (x >= comp->getX() && x < comp->getX() + comp->getWidth() && y >= comp->getY() && y < comp->getY() + comp->getHeight()) {
      return comp;
    }
  }
  return NULL;
}

int ShipView::getWallCode( int i, int j )
{
  int wallCode = 0;
  wallCode |= hasWall(i, j+1);
  wallCode |= (hasWall(i+1, j) << 1);
  wallCode |= (hasWall(i, j-1) << 2);
  wallCode |= (hasWall(i-1, j) << 3);
  assert (wallCode >= 0 && wallCode < 16);
  return wallCode;
}

void ShipView::setDoor( int x, int y )
{
  TileType type = getWall(x, y);
  if (type != Wall) {
    return;
  }
  int wallCode = getWallCode(x, y);
  if (wallCode != 10 && wallCode != 5) {
    //Not a horizontal or vertical straight wall
    return;
  }
  setWall(x, y, Door);
}

bool ShipView::hasDoorsAround( int x, int y )
{
  return getWall(x-1,y) == Door || getWall(x+1, y) == Door || getWall(x, y-1) == Door || getWall(x, y+1) == Door;
}

void ShipView::eraseDoorsAround( int x, int y )
{
  if (getWall(x-1,y) == Door) {
    setWall(x-1, y, Empty);
  }
  if (getWall(x+1,y) == Door) {
    setWall(x+1, y, Empty);
  }
  if (getWall(x,y-1) == Door) {
    setWall(x, y-1, Empty);
  }
  if (getWall(x,y+1) == Door) {
    setWall(x, y+1, Empty);
  }
}

void ShipView::setHoveredDimensions(int width, int height)
{
  hoverWidth_ = width;
  hoverHeight_ = height;
}
