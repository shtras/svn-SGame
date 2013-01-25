#include "StdAfx.h"
#include "CompartmentCrewIcons.h"
#include "CrewMemberIcon.h"
#include "Text.h"
#include "Renderer.h"


CompartmentCrewIcons::CompartmentCrewIcons( Rect size ):Widget(size)
{
  TextureParams params1 = {100, 152, 31, 31, 4, 4, 4, 4};
  TextureParams params2 = {132, 152, 31, 31, 4, 4, 4, 4};
  initRegular(params1);
  initHovered(params2);
  initPressed(params2);
}

CompartmentCrewIcons::~CompartmentCrewIcons()
{

}

void CompartmentCrewIcons::init( Compartment* comp, PersonHoverInfo* hoverInfo )
{
  hoverInfo_ = hoverInfo;
  assert(comp);
  comp_ = comp;
  if (comp->getCategory() == Compartment::Living) {
    initLiving();
  } else {
    initWorking();
  }
}

void CompartmentCrewIcons::initWorking()
{
  Renderer& renderer = Renderer::getInstance();
  float aspect = renderer.getWidth() / (float)renderer.getHeight();
  float lastTop = 0;
  float top = 0.01f;
  FramedText* compName = new FramedText(Rect(0.01f,top, 0.98f, 0.1f));
  compName->setCaption(comp_->getFullName());
  top += compName->getSize().height;
  addWidget(compName);
  for (int i=0; i<3; ++i) {
    //Loop watches
    FramedText* caption = new FramedText(Rect(0.33f*i+0.01f, top, 0.32f, 0.1f));
    caption->setCaption("Watch " + CString(i));
    addWidget(caption);
    Rect iconPos(0.01f + i*0.33f, top + 0.13f, 0.1f, 0.1f*aspect);
    for (int j = 0; j<comp_->getMaxCrew(); ++j) {
      CrewMemberIconWatch* icon = new CrewMemberIconWatch(iconPos, comp_, i);
      icon->setHoverInfo(hoverInfo_);
      addWidget(icon);
      iconPos.left += iconPos.width + 0.01f;
      lastTop = iconPos.top + iconPos.height;
      if (iconPos.left + iconPos.width > (i+1)*0.3) {
        iconPos.left = i*0.3f + 0.01f;
        iconPos.top += iconPos.height + 0.01f;
      }
    }
  }
  size_.height *= lastTop+0.1f;
}

void CompartmentCrewIcons::initLiving()
{

}

void CompartmentCrewIcons::onHoverEnter()
{
  if (comp_) {
    comp_->setHighLighted(true);
  }
}

void CompartmentCrewIcons::onHoverExit()
{
  if (comp_) {
    comp_->setHighLighted(false);
  }
}

