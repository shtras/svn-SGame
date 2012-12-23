#include "StdAfx.h"
#include "ShipView.h"
#include "Renderer.h"

ShipView::ShipView(Rect size):Widget(size),layoutWidth_(50), layoutHeight_(50), zoom_(1.0f), offsetX_(0.0f), offsetY_(0.0f),
  scrolling_(false), lastMouseX_(0), lastMouseY_(0),hoveredLeft_(-1), hoveredTop_(-1), hoverWidth_(1), hoverHeight_(1),tileWidth_(0),
  tileHeight_(0),drawing_(false),drawingStartX_(-1), drawingStartY_(-1),desiredZoom_(1.0f),zoomStep_(0),action_(BuildWalls)
{
  wallLayout_ = new int[layoutHeight_ * layoutWidth_];
  for (int i=0; i<layoutHeight_ * layoutWidth_; ++i) {
    wallLayout_[i] = -1;
  }
}

ShipView::~ShipView()
{
  delete[] wallLayout_;
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
      if (i >= hoveredLeft_ && j >= hoveredTop_ && i < hoveredLeft_ + hoverHeight_ && j < hoveredTop_ + hoverHeight_) {
        renderer.setColor(Vector4(100,255,50,200));
        CString coord = CString(i) + "," + CString(j);
        renderer.renderText(tileX + tileWidth_*0.5f - renderer.getCharWidth()*coord.getSize()*0.5f, tileY + tileHeight_*0.5f - renderer.getCharHeight()*0.5f, coord);
      }
      Rect tilePos(tileX, tileY, tileWidth_, tileHeight_);
      Rect texPos(0.0f, 0.0f, 0.125f, 0.25f);
      int wallValue = getWall(i, j);
      int wallCode = 0;
      if (wallValue == 1) {
        wallCode |= hasWall(i, j+1);
        wallCode |= (hasWall(i+1, j) << 1);
        wallCode |= (hasWall(i, j-1) << 2);
        wallCode |= (hasWall(i-1, j) << 3);
        assert (wallValue >= 0 && wallValue < 16);
      }
      if (wallValue == 1 && wallCode == 0) {
        wallCode = 10; //horizontal wall;
      }
      if (wallValue == -1) {
        renderer.setColor(Vector4(255,255,255,50));
      }
      if (drawing_ && i >= drawStartX && i <= drawEndX && j >= drawStartY && j <= drawEndY) {
        renderer.setColor(Vector4(255,155,55,200));
      }
      texPos.top = wallCode / 4 * 0.25f;
      texPos.left = wallCode % 4 * 0.125f;
      if (wallValue == 0) {
        texPos.left = 0.5f;
        texPos.top = 0;
      }
      texPos.left += 0.002f;
      texPos.top += 0.004f;
      texPos.width -= 0.004f;
      texPos.height -= 0.008f;
      renderer.drawTexRect(tilePos, tilesTex, texPos);
      renderer.setColor(Vector4(255,255,255,255));
    }
  }
  renderer.setTextSize(1);
  renderer.renderText(0, 0, CString(zoom_, 5) + " " + CString(desiredZoom_, 5) + " " + CString(zoomStep_, 5));
  //glScissor(0, 0, 1600, 900);
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
  }
  drawing_ = false;
}

int ShipView::getWall( int x, int y )
{
  if (x < 0 || x >= layoutWidth_ || y < 0 || y >= layoutHeight_) {
    return 0;
  }
  return wallLayout_[y*layoutWidth_ + x];
}

int ShipView::hasWall( int x, int y )
{
  if (x < 0 || x >= layoutWidth_ || y < 0 || y >= layoutHeight_) {
    return 0;
  }
  return wallLayout_[y*layoutWidth_ + x]==1?1:0;
}

void ShipView::setWall( int x, int y, int value )
{
  if (x < 0 || x >= layoutWidth_ || y < 0 || y >= layoutHeight_) {
    assert(0);
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
    setWall(i, drawingStartY_, 1);
    setWall(i, hoveredTop_, 1);
  }
  for (int i=drawingStartY_; i != hoveredTop_ + stepVer; i += stepVer) {
    setWall(drawingStartX_, i, 1);
    setWall(hoveredLeft_, i, 1);
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
  int value = -1;
  if (action_ == BuildFloor) {
    value = 0;
  }
  for (int i=drawingStartX_; i!= hoveredLeft_ + stepHor; i += stepHor) {
    for (int j=drawingStartY_; j != hoveredTop_ + stepVer; j += stepVer) {
      if (value == 0 && getWall(i, j) != -1) {
        continue;
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
