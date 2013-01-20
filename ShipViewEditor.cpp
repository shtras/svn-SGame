#include "StdAfx.h"
#include "ShipViewEditor.h"
#include "Renderer.h"
#include "Ship\Ship.h"
#include "GUI\CompartmentButton.h"
#include "GUI\ShipView\ShipView.h"


ShipViewEditor::ShipViewEditor( Rect size ):ShipView(size),buildInfo_(NULL),action_(BuildWalls),draggedCompartmentRoataion_(0),draggedComp_(NULL)
{
  editor_ = true;
}

ShipViewEditor::~ShipViewEditor()
{

}


void ShipViewEditor::onLMDown()
{
  if (action_ == BuildFloor || action_ == BuildWalls || action_ == Erase) {
    drawing_ = true;
    drawingStartX_ = hoveredLeft_;
    drawingStartY_ = hoveredTop_;
  }
}

void ShipViewEditor::onLMUp()
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
  buildInfo_->updateValues();
}

void ShipViewEditor::onDrop(Widget* w)
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


void ShipViewEditor::onHoverExit()
{
  scrolling_ = false;
  drawing_ = false;
  hoveredLeft_ = -1;
  hoveredTop_ = -1;
  drawingStartX_ = -1;
  drawingStartY_ = -1;
  Renderer::getInstance().setFloatingWidget(NULL);
}

void ShipViewEditor::onMouseMove()
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
    Renderer::getInstance().setFloatingWidgetWidthLimit(size_.left + size_.width);
    Renderer::getInstance().setFloatingWidget(hoveredCompInfo_);
  } else {
    Renderer::getInstance().setFloatingWidget(NULL);
  }
}

void ShipViewEditor::setBuildInfo( BuildInfo* info )
{
  assert(info);
  buildInfo_ = info;
  buildInfo_->updateValues();
}

void ShipViewEditor::plantWalls()
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

void ShipViewEditor::eraseArea()
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

void ShipViewEditor::setEntrance()
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

void ShipViewEditor::setAction( Action action )
{
  action_ = action;
}

void ShipViewEditor::setHoveredDimensions(int width, int height)
{
  draggedCompartmentRoataion_ = 0;
  hoverWidth_ = width;
  hoverHeight_ = height;
}

void ShipViewEditor::addLogMessage( CString message )
{
  buildInfo_->addLogMessage(message);
}

void ShipViewEditor::createConnection()
{
  assert(selectedComp_ && hoveredComp_);
  if (hoveredComp_->isConnectedTo(selectedComp_)) {
    assert(selectedComp_->isConnectedTo(hoveredComp_));
    addLogMessage("Already connected");
    selectedComp_ = NULL;
    return;
  }
  if (hoveredComp_->numConnectionsTo(selectedComp_->getName()) == hoveredComp_->getMaxSameConnections()
      || selectedComp_->numConnectionsTo(hoveredComp_->getName()) == selectedComp_->getMaxSameConnections()) {
    addLogMessage("Has maximum connections to this type");
    selectedComp_ = NULL;
    return;
  }
  if (hoveredComp_->getMaxConnections() == hoveredComp_->getConnections().size() || selectedComp_->getMaxConnections() == selectedComp_->getConnections().size()) {
    addLogMessage("Already has maximum connections");
    selectedComp_ = NULL;
    return;
  }
  if (!hoveredComp_->requiredConnection(selectedComp_->getName()) && !selectedComp_->requiredConnection(hoveredComp_->getName())) {
    addLogMessage("Wrong connection");
    selectedComp_ = NULL;
    return;
  }
  selectedComp_->connectTo(hoveredComp_);
  hoveredComp_->connectTo(selectedComp_);
  addLogMessage(hoveredComp_->getName() + " connected to " + selectedComp_->getName());
  selectedComp_ = NULL;
  ship_->checkConnections();
  onMouseMove();
}

void ShipViewEditor::removeConnection()
{
  assert(selectedComp_ && hoveredComp_);
  if (!hoveredComp_->isConnectedTo(selectedComp_)) {
    assert(!selectedComp_->isConnectedTo(hoveredComp_));
    addLogMessage("Connection does not exist");
    selectedComp_ = NULL;
    return;
  }
  selectedComp_->disconnectFrom(hoveredComp_);
  hoveredComp_->disconnectFrom(selectedComp_);
  addLogMessage("Connection removed");
  selectedComp_ = NULL;
  ship_->checkConnections();
  onMouseMove();
}

void ShipViewEditor::saveShip(CString fileName)
{
  ship_->save("saves/" + fileName);
  addLogMessage("Successfully saved");
}

void ShipViewEditor::loadShip(CString fileName)
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

void ShipViewEditor::onMouseWheelScroll( int direction )
{
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
  } else {
    ShipView::onMouseWheelScroll(direction);
  }
}
