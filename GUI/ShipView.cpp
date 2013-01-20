#include "StdAfx.h"
#include "ShipView.h"
#include "Renderer.h"
#include "CompartmentButton.h"
#include "BuildInfo.h"

ShipView::ShipView(Rect size):Widget(size),layoutWidth_(50), layoutHeight_(50), zoom_(0.5f), offsetX_(0.0f), offsetY_(0.0f),
  scrolling_(false), lastMouseX_(0), lastMouseY_(0),hoveredLeft_(-1), hoveredTop_(-1), hoverWidth_(1), hoverHeight_(1),tileWidth_(0),
  tileHeight_(0),drawing_(false),drawingStartX_(-1), drawingStartY_(-1),desiredZoom_(1.0f),zoomStep_(0),action_(BuildWalls),
  tilesTexWidth_(0), tilesTexHeight_(0), hoveredComp_(NULL),activeDeckIdx_(-1),activeDeck_(NULL),buildInfo_(NULL),hoveredCompInfo_(NULL),
  draggedCompartmentRoataion_(0),draggedComp_(NULL),ghostDeckIdx_(-1),overviewType_(Construction),selectedComp_(NULL)
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
      Tile* tile = activeDeck_->getTile(i, j);
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
      } else if (wallValue == Tile::Empty || wallValue == Tile::Floor) {
        if (wallValue == Tile::Empty) {
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
      Compartment* comp = activeDeck_->getCompartment(i, j);
      if (hoveredComp_ && comp == hoveredComp_) {
        renderer.setColor(Vector4(125, 200, 210, 255));
      }
      if (selectedComp_ && comp == selectedComp_) {
        renderer.setColor(Vector4(125, 170, 190, 255));
      }
      if (selectedComp_ && comp && selectedComp_->isConnectedTo(comp)) {
        renderer.setColor(Vector4(125, 125, 125, 255));
      }
      
      if (drawing_ && i >= drawStartX && i <= drawEndX && j >= drawStartY && j <= drawEndY) {
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
        texPos.left = 257.0f / tilesTexWidth_;
        if (wallCode == 10) {
          texPos.top = 65.0f / tilesTexHeight_;
        } else if (wallCode == 5) {
          texPos.top = 129.0f / tilesTexHeight_;
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
      renderer.drawTexRect(tilePos, tilesTex, texPos);
      renderer.setColor(Vector4(255,255,255,255));
    }
  }
  drawCompartments();
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
    if (hoveredComp_ == comp) {
      renderer.setColor(Vector4(125, 200, 210, 255));
    } else {
      renderer.setColor(Vector4(255,255,255,255));
    }
    for (auto itemItr = comp->getItems().begin(); itemItr != comp->getItems().end(); ++itemItr) {
      Item* item = *itemItr;
      int x = item->getX() + comp->getX();
      int y = item->getY() + comp->getY();
      int rotation = item->getRotation();
      int renderRotate = 0;
      if (item->autorotate()) {
        renderRotate = rotation;
        rotation = 0;
      }
      float tileX = x*tileWidth_ + offsetX_*zoom_ + size_.left;
      float tileY = y*tileHeight_ + offsetY_*zoom_ + size_.top;
      if (tileX < size_.left || tileX+tileWidth_ > size_.left + size_.width) {
        continue;
      }
      if (tileY < size_.top || tileY+tileHeight_ > size_.top + size_.height) {
        continue;
      }
      Rect pos(tileX, tileY, tileWidth_, tileHeight_);
      Rect texPos(item->getTexX() + rotation * item->getTexWidth() + item->getTexWidth()*0.01f, item->getTexY() + item->getTexHeight()*0.01f, item->getTexWidth()*0.98f, item->getTexHeight()*0.98f);
      renderer.drawTexRect(pos, tilesTex, texPos, renderRotate);
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
  if (hoveredComp_ && !drawing_) {
    hoveredCompInfo_->setCompartment(hoveredComp_);
    Renderer::getInstance().setFloatingWidget(hoveredCompInfo_);
  } else {
    Renderer::getInstance().setFloatingWidget(NULL);
  }
}

void ShipView::onMouseWheelScroll(int direction)
{
  assert (direction == 1 || direction == -1);
  if (draggedComp_) {
    draggedCompartmentRoataion_ -= direction;
    if (draggedCompartmentRoataion_ > 3) {
      draggedCompartmentRoataion_ = 0;
    }
    if (draggedCompartmentRoataion_ < 0) {
      draggedCompartmentRoataion_ = 3;
    }
    int temp = hoverWidth_;
    hoverWidth_ = hoverHeight_;
    hoverHeight_ = temp;
    return;
  }
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
  if (action_ == BuildFloor || action_ == BuildWalls || action_ == Erase) {
    drawing_ = true;
    drawingStartX_ = hoveredLeft_;
    drawingStartY_ = hoveredTop_;
  }
}

void ShipView::onLMUp()
{
  if (action_ == BuildWalls) {
    plantWalls();
  } else if (action_ == Erase || action_ == BuildFloor) {
    eraseArea();
  } else if (action_ == BuildDoor) {
    activeDeck_->setDoor(hoveredLeft_, hoveredTop_);
  } else if (action_ == ChooseEntrance) {
    setEntrance();
  } else if (action_ == CreateConnection || action_ == RemoveConnection) {
    if (!selectedComp_ && hoveredComp_) {
      selectedComp_ = hoveredComp_;
    }
    if (selectedComp_ && hoveredComp_ && selectedComp_ != hoveredComp_) {
      if (action_ == CreateConnection) {
        createConnection();
      } else {
        removeConnection();
      }
    }
  } else if (action_ == Select) {
    selectedComp_ = hoveredComp_;
  } else if (action_ == BuildStairs) {
    activeDeck_->setStair(hoveredLeft_, hoveredTop_);
  }
  drawing_ = false;
  Tile* lastEntrance = ship_->getEntrance();
  structureChanged();
  if (lastEntrance != ship_->getEntrance()) {
    addLogMessage("Entrance is blocked. Deleted");
  }
}

void ShipView::onDrop(Widget* w)
{
  CompartmentButton* cb = dynamic_cast<CompartmentButton*>(w);
  if (!cb) {
    return;
  }
  Compartment* comp = cb->getCompartment();
  assert (draggedComp_ == comp);
  //Check overlapping
  int x = hoveredLeft_;
  int y = hoveredTop_;
  int width = hoverWidth_;
  int height = hoverHeight_;
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
  newComp->width_ = hoverWidth_;
  newComp->height_ = hoverHeight_;
  //Rotate items in the compartment
  if (draggedCompartmentRoataion_ != 0) {
    for (auto itr = newComp->getItems().begin(); itr != newComp->getItems().end(); ++itr) {
      Item* item = *itr;
      int newRot = item->getRotation() + draggedCompartmentRoataion_;
      if (newRot > 3) {
        newRot -= 4;
      }
      if (newRot < 0) {
        newRot += 4;
      }
      item->setRotation(newRot);
      int x = item->getX();
      int y = item->getY();
      switch (draggedCompartmentRoataion_)
      {
      case 1:
        item->setX(comp->getHeight() - y - 1);
        item->setY(x);
        break;
      case 2:
        item->setX(comp->getWidth() - x - 1);
        item->setY(comp->getHeight() - y - 1);
        break;
      case 3:
        item->setX(y);
        item->setY(comp->getWidth() - x - 1);
        break;
      default:
        assert(0);
        break;
      }
      assert(item->getX() >= 0 && item->getX() < newComp->getWidth() && item->getY() >= 0 && item->getY() < newComp->getHeight());
    }
  }
  activeDeck_->addCompartment(newComp);
  buildInfo_->updateValues();
  draggedCompartmentRoataion_ = 0;
  structureChanged();
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
  int drawWidth = abs(hoveredLeft_ - drawingStartX_);
  int drawHeight = abs(hoveredTop_ - drawingStartY_);
  bool removeArea = true;
  if (drawWidth == 0 && drawHeight == 0) {
    //Removing single tile
    if (value == Tile::Empty && activeDeck_->getTileType(hoveredLeft_, hoveredTop_) == Tile::Stair) {
      //Removing single stair
      activeDeck_->setTileType(hoveredLeft_, hoveredTop_, Tile::Floor);
      removeArea = false;
    }
  }
  if (removeArea) {
    for (int i=drawingStartX_; i != hoveredLeft_ + stepHor; i += stepHor) {
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
            if (comp == selectedComp_) {
              selectedComp_ = NULL;
            }
            if (comp == hoveredComp_) {
              hoveredComp_ = NULL;
            }
            if (comp->getConnections().size() > 0) {
              addLogMessage("Connections removed");
            }
            activeDeck_->removeCompartment(comp);
            buildInfo_->updateValues();
            assert (!activeDeck_->getCompartment(i,j));
            if (drawWidth == 0 && drawHeight == 0) {
              continue;
            }
          }
        }
        activeDeck_->setTileType(i, j, value);
      }
    }
  }
  if (ship_->getEntrance() && ship_->getEntrance()->getType() != Tile::Door) {
    assert (!ship_->getEntrance()->isEntrance());
    ship_->setEntrance(NULL);
  }
}

void ShipView::setAction( Action action )
{
  action_ = action;
}

void ShipView::setHoveredDimensions(int width, int height)
{
  draggedCompartmentRoataion_ = 0;
  hoverWidth_ = width;
  hoverHeight_ = height;
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

void ShipView::setBuildInfo( BuildInfo* info )
{
  assert(info);
  buildInfo_ = info;
  buildInfo_->updateValues();
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

void ShipView::setEntrance()
{
  Tile::TileType type = activeDeck_->getTileType(hoveredLeft_, hoveredTop_);
  if (type != Tile::Door) {
    return;
  }
  if (ship_->getEntrance()) {
    ship_->getEntrance()->setEntrance(false);
  }
  Tile* tile = activeDeck_->getTile(hoveredLeft_, hoveredTop_);
  tile->setEntrance(true);
  ship_->setEntrance(tile);
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

void ShipView::createConnection()
{
  assert(selectedComp_ && hoveredComp_);
  if (hoveredComp_->isConnectedTo(selectedComp_)) {
    assert(selectedComp_->isConnectedTo(hoveredComp_));
    return;
  }
  if (hoveredComp_->isConnectedTo(selectedComp_->getName()) || selectedComp_->isConnectedTo(hoveredComp_->getName())) {
    addLogMessage("Already connected to this type");
    return;
  }
  if (!hoveredComp_->requiredConnection(selectedComp_->getName()) && !selectedComp_->requiredConnection(hoveredComp_->getName())) {
    addLogMessage("Wrong connection");
    return;
  }
  selectedComp_->connectTo(hoveredComp_);
  hoveredComp_->connectTo(selectedComp_);
  addLogMessage(hoveredComp_->getName() + " connected to " + selectedComp_->getName());
  selectedComp_ = NULL;
}

void ShipView::removeConnection()
{
  assert(selectedComp_ && hoveredComp_);
  if (!hoveredComp_->isConnectedTo(selectedComp_)) {
    assert(!selectedComp_->isConnectedTo(hoveredComp_));
    addLogMessage("Connection does not exist");
    return;
  }
  selectedComp_->disconnectFrom(hoveredComp_);
  hoveredComp_->disconnectFrom(selectedComp_);
  addLogMessage("Connection removed");
}

void ShipView::addLogMessage( CString message )
{
  buildInfo_->addLogMessage(message);
}

void ShipView::saveShip(CString fileName)
{
  ship_->save("saves/" + fileName);
  addLogMessage("Successfully saved");
}

void ShipView::loadShip(CString fileName)
{
  bool res = ship_->load("saves/" + fileName);
  if (!res) {
    addLogMessage("Load failed. Refer to error.log for details");
    delete ship_;
    ship_ = new Ship(layoutWidth_, layoutHeight_);
  } else {
    addLogMessage("Successfully loaded");
  }
  activeDeckIdx_ = 0;
  activeDeck_ = ship_->getDeck(activeDeckIdx_);
  hoveredComp_ = NULL;
  selectedComp_ = NULL;
  buildInfo_->updateValues();
}
