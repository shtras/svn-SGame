#include "StdAfx.h"
#include "CrewList.h"
#include "CrewMemberIcon.h"
#include "Renderer.h"
#include "CompartmentCrewIcons.h"


CrewList::CrewList(Rect size):Window(size),hoverInfo_(NULL)
{
  hoverInfo_ = new PersonHoverInfo(Rect(0.0, 0.0, 0.15, 0.3));
}

CrewList::~CrewList()
{
  delete hoverInfo_;
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
    CrewMemberIcon* icon = new CrewMemberIcon(iconSize, NULL);
    icon->setHoverInfo(hoverInfo_);
    icon->setPerson(pers);
    addWidget(icon);
    iconSize.left += iconSize.width + 0.02f;
    if (iconSize.left + iconSize.width > 0.98f) {
      iconSize.left = 0.02f;
      iconSize.top += iconSize.height + 0.02f;
    }
  }
  Rect compPos(0.01f, iconSize.top + iconSize.height, 0.98f, 0.2f);
  for (Compartment* comp: ship_->getCompartments()) {
    if (comp->getCategory() == Compartment::Living || comp->getMaxCrew() == 0) {
      continue;
    }
    CompartmentCrewIcons* crewIcons = new CompartmentCrewIcons(compPos);
    crewIcons->init(comp, hoverInfo_);
    addWidget(crewIcons);
    compPos.top += crewIcons->getSize().height + 0.02f;
  }
}
