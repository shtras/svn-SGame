#include "StdAfx.h"
#include "ShipView.h"
#include "Renderer.h"
#include "CompartmentButton.h"
#include "BuildInfo.h"

ShipView::ShipView(Rect size):Widget(size),layoutWidth_(50), layoutHeight_(50), zoom_(0.5f), offsetX_(0.0f), offsetY_(0.0f),
  scrolling_(false), lastMouseX_(0), lastMouseY_(0),hoveredLeft_(-1), hoveredTop_(-1), hoverWidth_(1), hoverHeight_(1),tileWidth_(0),
  tileHeight_(0),drawing_(false),drawingStartX_(-1), drawingStartY_(-1),desiredZoom_(1.0f),zoomStep_(0),
  tilesTexWidth_(0), tilesTexHeight_(0), hoveredComp_(NULL),activeDeckIdx_(-1),activeDeck_(NULL),hoveredCompInfo_(NULL),
  ghostDeckIdx_(-1),overviewType_(Construction),selectedComp_(NULL),editor_(false),ship_(NULL)
{
  tilesTexWidth_ = Renderer::getInstance().getTilesTexWidth();
  tilesTexHeight_ = Renderer::getInstance().getTilesTexHeight();
  //for (int i=0; i<3; ++i) {
  //  DeckView* deck = new DeckView(layoutWidth_, layoutHeight_);
  //  decks_.push_back(deck);
  //}
  offsetX_ = -size_.width*0.5f/zoom_;
  offsetY_ = -size_.height*0.5f/zoom_;
  desiredZoom_ = zoom_;
  hoveredCompInfo_ = new CompHoverInfo(Rect(0.0, 0.0, 0.2, 0.2));
}

ShipView::~ShipView()
{
  delete hoveredCompInfo_;
  delete ship_;
}

void ShipView::render()
{
  if (fabs(zoom_ - desiredZoom_) > 0.0001f) {
    zoom_ += zoomStep_;
    onMouseMove();
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
  renderer.setDrawBoundries(size_);
  for (int i=0; i<layoutWidth_; ++i) {
    float tileX = i*tileWidth_ + offsetX_*zoom_ + size_.left + size_.width*0.5f;
    if (tileX < size_.left - tileWidth_ || tileX > size_.left + size_.width) {
      continue;
    }
    for (int j=0; j<layoutHeight_; ++j) {
      float tileY = j*tileHeight_ + offsetY_*zoom_ + size_.top + size_.height*0.5f;
      if (tileY < size_.top-tileHeight_ || tileY > size_.top + size_.height) {
        continue;
      }
      Tile* tile = activeDeck_->getTile(i, j);
      //if (lastMouseX_ >= tileX && lastMouseX_ <= tileX + tileWidth_*hoverWidth_ && lastMouseY_ >= tileY && lastMouseY_ <= tileY + tileHeight_*hoverHeight_) {
      //  renderer.setColor(Vector4(100,255,50,200));
      //}
      Rect tilePos(tileX, tileY, tileWidth_, tileHeight_);
      Rect texPos(0.0f, 0.0f, tileTexWidth, tileTexHeight);
      int wallValue = activeDeck_->getTileType(i, j);
      int wallCode = 0;
      int rotation = 0;
      bool renderPrevFloorCorners = false;
      if (wallValue == Tile::Wall || wallValue == Tile::Door) {
        wallCode = activeDeck_->getWallCode(i, j);
      }
      if (wallValue == Tile::Wall && wallCode == 0) {
        wallCode = 10; //horizontal wall;
      }
      if (editor_ && i >= hoveredLeft_ && j >= hoveredTop_ && i < hoveredLeft_ + hoverWidth_ && j < hoveredTop_ + hoverHeight_) {
        renderer.setColor(Vector4(155,255,55,200));
        CString coord = CString(i) + "," + CString(j);
        if (drawing_) {
          coord += " " + CString(drawEndX - drawStartX + 1) + "," + CString(drawEndY - drawStartY + 1);
        }
        renderer.renderText(tileX + tileWidth_*0.5f - renderer.getCharWidth()*coord.getSize()*0.5f, tileY + tileHeight_*0.5f - renderer.getCharHeight()*0.5f, coord);
      } else if (wallValue == Tile::Empty || wallValue == Tile::Floor) {
        if (wallValue == Tile::Empty) {
          if (!editor_) {
            continue;
          }
          renderer.setColor(Vector4(255,255,255,50));
        }
        if (ghostDeckIdx_ >= 0) {
          int prevWallValue = ship_->getDeck(ghostDeckIdx_)->getTileType(i, j);
          if (wallValue == Tile::Empty && prevWallValue != Tile::Empty) {
            wallValue = prevWallValue;
            wallCode = ship_->getDeck(ghostDeckIdx_)->getWallCode(i, j);
            if (wallValue != Tile::Floor) {
              renderPrevFloorCorners = true;
            }
          }
          if (wallValue == Tile::Floor && prevWallValue == Tile::Stair) {
            wallValue = Tile::Stair;
            renderPrevFloorCorners = true;
            renderer.setColor(Vector4(255,255,50,200));
          }
        }
      }
      if (hoveredComp_ && hoveredComp_->isInside(i, j)) {
        renderer.setColor(Vector4(125, 200, 210, 255));
      }
      if (selectedComp_ && selectedComp_->isInside(i, j)) {
        renderer.setColor(Vector4(125, 170, 190, 255));
      }
      
      if (editor_ && drawing_ && i >= drawStartX && i <= drawEndX && j >= drawStartY && j <= drawEndY) {
        renderer.setColor(Vector4(255,155,55,200));
      }
      if (overviewType_ == Accessibility && tile->getType() != Tile::Empty) {
        if (tile->isAccessible()) {
          renderer.setColor(Vector4(0, 255, 0, 255));
        } else if (tile->isConnected()) {
          renderer.setColor(Vector4(0, 0, 255, 255));
        } else {
          renderer.setColor(Vector4(255, 0, 0, 255));
        }
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
        int openState = tile->getOpenState();
        int coordX = 257 + openState*64;
        texPos.top = 65.0f / tilesTexHeight_;
        if (openState > 3) {
          coordX -= 64*4;
          texPos.top += 64.0f/(float)tilesTexHeight_;
        }
        texPos.left = (coordX) / (float)tilesTexWidth_;
        if (wallCode == 5) {
          rotation = 1;
        }
        if (tile->isEntrance()) {
          assert (tile == ship_->getEntrance());
          renderer.setColor(Vector4(200, 200, 200, 255));
        }
      }
      if (wallValue == Tile::Stair) {
        texPos.left = 321.0f / (float)tilesTexWidth_;
        texPos.top = 1.0f / (float)tilesTexHeight_;
      }
      //texPos.left += 0.002f;
      //texPos.top += 0.004f;
      //texPos.width -= 0.004f;
      //texPos.height -= 0.008f;
      renderer.drawTexRect(tilePos, tilesTex, texPos, rotation);
      renderer.setColor(Vector4(255,255,255,255));
    }
  }
  drawCompartments();
  renderer.resetDrawBoundries();
  //glScissor(0, 0, 1600, 900);
}

bool ShipView::isFloorOrStair( Deck* deck, int x, int y )
{
  if (deck->getTileType(x, y) == Tile::Floor || deck->getTileType(x, y) == Tile::Stair) {
    return true;
  }
  return false;
}

void ShipView::renderFloorSection(Deck* deck, int i, int j, Rect& tilePos, Rect& texPos)
{
  //@TODO: Move this info to Tile class
  Renderer& renderer = Renderer::getInstance();
  GLuint texTile = renderer.getTilesTex();
  float texX = 257.0f / (float)tilesTexWidth_;
  float texY = 1.0f / (float)tilesTexHeight_;
  if (isFloorOrStair(deck, i-1, j) || isFloorOrStair(deck, i, j-1) || isFloorOrStair(deck, i-1, j-1)) {
    Rect upLeft       (tilePos.left, tilePos.top, tilePos.width*0.5f, tilePos.height*0.5f);
    Rect texUpLeft    (texX,         texY,        texPos.width*0.5f,  texPos.height*0.5f);
    renderer.drawTexRect(upLeft, texTile, texUpLeft);
  }
  if (isFloorOrStair(deck, i+1, j) || isFloorOrStair(deck, i, j-1) || isFloorOrStair(deck, i+1, j-1)) {
    Rect upRight      (tilePos.left + tilePos.width*0.5f, tilePos.top, tilePos.width*0.5f, tilePos.height*0.5f);
    Rect texUpRight   (texX + texPos.width*0.5f,          texY,        texPos.width*0.5f,  texPos.height*0.5f);
    renderer.drawTexRect(upRight, texTile, texUpRight);
  }
  if (isFloorOrStair(deck, i, j+1) || isFloorOrStair(deck, i+1, j) || isFloorOrStair(deck, i+1, j+1)) {
    Rect downRight    (tilePos.left + tilePos.width*0.5f, tilePos.top+tilePos.height*0.5f, tilePos.width*0.5f, tilePos.height*0.5f);
    Rect texDownRight (texX + texPos.width*0.5f,          texY+texPos.height*0.5f,         texPos.width*0.5f,  texPos.height*0.5f);
    renderer.drawTexRect(downRight, texTile, texDownRight);
  }
  if (isFloorOrStair(deck, i, j+1) || isFloorOrStair(deck, i-1, j) || isFloorOrStair(deck, i-1, j+1)) {
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
    renderer.setColor(Vector4(255,255,255,255));
    if (selectedComp_ && selectedComp_->isConnectedTo(comp)) {
      renderer.setColor(Vector4(125, 125, 125, 255));
    }
    if (hoveredComp_ == comp || comp->isHighLighted()) {
      renderer.setColor(Vector4(125, 200, 210, 255));
    }
    for (auto itemItr = comp->getItems().begin(); itemItr != comp->getItems().end(); ++itemItr) {
      Item* item = *itemItr;
      int x = item->getX() + comp->getX();
      int y = item->getY() + comp->getY();
      Tile* tile = activeDeck_->getTile(x, y);
      assert(tile);
      int rotation = item->getRotation();
      int renderRotate = 0;
      if (item->autorotate()) {
        renderRotate = rotation;
        rotation = 0;
      }
      float tileX = x*tileWidth_ + offsetX_*zoom_ + size_.left + size_.width*0.5f;
      float tileY = y*tileHeight_ + offsetY_*zoom_ + size_.top + size_.height*0.5f;
      if (tileX < size_.left-tileWidth_ || tileX > size_.left + size_.width) {
        continue;
      }
      if (tileY < size_.top - tileHeight_ || tileY > size_.top + size_.height) {
        continue;
      }
      if (overviewType_ == Accessibility) {
        renderer.setColor(Vector4(255,255,255,255));
        if (item->requiresVacuum()) {
          if (tile->getType() != Tile::Empty) {
            renderer.setColor(Vector4(255, 0, 0, 255));
          }
        } else if (tile->getType() == Tile::Empty || !tile->isConnected()) {
          renderer.setColor(Vector4(255, 0, 0, 255));
        }
      }
      Rect pos(tileX, tileY, tileWidth_, tileHeight_);
      Rect texPos(item->getTexX() + rotation * item->getTexWidth() + item->getTexWidth()*0.01f, item->getTexY() + item->getTexHeight()*0.01f, item->getTexWidth()*0.98f, item->getTexHeight()*0.98f);
      renderer.drawTexRect(pos, tilesTex, texPos, renderRotate);
    }
    renderer.setColor(Vector4(255,255,255,255));
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

void ShipView::onMouseWheelScroll(int direction)
{
  assert (direction == 1 || direction == -1);
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

  hoveredLeft_ = (int)(((lastMouseX_-size_.left - size_.width*0.5f) - offsetX_*zoom_)/tileWidth_);
  hoveredTop_ = (int)(((lastMouseY_ - size_.top - size_.height*0.5f) - offsetY_*zoom_)/tileHeight_);
  hoveredComp_ = activeDeck_->getCompartment(hoveredLeft_, hoveredTop_);
  if (hoveredComp_ && !drawing_) {
    hoveredCompInfo_->setCompartment(hoveredComp_);
    Renderer::getInstance().setFloatingWidgetWidthLimit(size_.left + size_.width);
    Renderer::getInstance().setFloatingWidget(hoveredCompInfo_);
  } else {
    Renderer::getInstance().setFloatingWidget(NULL);
  }
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

void ShipView::deckUp()
{
  ++activeDeckIdx_;
  if (activeDeckIdx_ > ship_->getNumDecks()-1) {
    activeDeckIdx_ = ship_->getNumDecks()-1;
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

void ShipView::ghostDeckUp()
{
  ++ghostDeckIdx_;
  if (ghostDeckIdx_ > 2) {
    ghostDeckIdx_ = 2;
  }
}

void ShipView::ghostDeckDown()
{
  --ghostDeckIdx_;
  if (ghostDeckIdx_ < -1) {
    ghostDeckIdx_ = -1;
  }
}

void ShipView::structureChanged()
{
  ship_->recalculateTiles();
}

void ShipView::constructionOverview()
{
  overviewType_ = Construction;
}

void ShipView::accessibilityOverview()
{
  overviewType_ = Accessibility;
}

