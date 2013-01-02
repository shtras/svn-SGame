#include "StdAfx.h"
#include "CompartmentButton.h"
#include "Renderer.h"

CompartmentButton::CompartmentButton(Rect size, Compartment* comp, ShipView* shipView):Button(size, false),comp_(comp),shipView_(shipView)
{
  TextureParams params1 = {100, 152, 31, 31, 11, 11, 11, 11};
  TextureParams params2 = {132, 152, 31, 31, 11, 11, 11, 11};
  initRegular(params1);
  initHovered(params2);
  initPressed(params2);
  draggable_ = true;
}

CompartmentButton::~CompartmentButton()
{
}

void CompartmentButton::render()
{
  Renderer& renderer = Renderer::getInstance();
  GLuint texID = renderer.getTilesTex();
  float aspect = renderer.getWidth() / (float)renderer.getHeight();
  float tileWidth = size_.width / (float)comp_->getWidth();
  float tileHeight = tileWidth * aspect;

  const list<Item*>& items = comp_->getItems();
  for (auto itemItr = items.begin(); itemItr != items.end(); ++itemItr) {
    Item* item = *itemItr;
    Rect pos(item->getX() * tileWidth + size_.left, item->getY()*tileHeight + size_.top, tileWidth, tileHeight);
    Rect texPos(item->getTexX() + item->getRotation()*item->getTexWidth(), item->getTexY(), item->getTexWidth(), item->getTexHeight());
    renderer.drawTexRect(pos, texID, texPos);
  }
}

void CompartmentButton::onDrag()
{
  shipView_->setHoveredDimensions(comp_->getWidth(), comp_->getHeight());
}

void CompartmentButton::onStopDrag()
{
  shipView_->setHoveredDimensions(1,1);
}
