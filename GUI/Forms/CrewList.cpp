#include "StdAfx.h"
#include "CrewList.h"
#include "CrewMemberIcon.h"
#include "Renderer.h"


CrewList::CrewList(Rect size):Window(size)
{

}

CrewList::~CrewList()
{

}

void CrewList::init( Ship* ship )
{
  Renderer& renderer = Renderer::getInstance();
  float aspect = renderer.getWidth() / (float)renderer.getHeight();

  ship_ = ship;
  float left = 0.02f;
  float width = 0.15f;
  float height = width*aspect * size_.width/size_.height;
  float top = 0.01f;
  Rect iconSize (left, top, width, height);
  for (Person* pers: ship_->getCrew()) {
    CrewMemberIcon* icon = new CrewMemberIcon(iconSize);
    icon->setPerson(pers);
    addWidget(icon);
    iconSize.left += iconSize.width + 0.02f;
    if (iconSize.left + iconSize.width > 0.98f) {
      iconSize.left = 0.02f;
      iconSize.top += iconSize.height + 0.02f;
    }
  }
}
