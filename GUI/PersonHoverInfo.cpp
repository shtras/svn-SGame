#include "StdAfx.h"
#include "PersonHoverInfo.h"

PersonHoverInfo::PersonHoverInfo(Rect size):Window(size)
{
  nameText_ = new Text(Rect(0.01, 0.01, 0.98, 0.1));
  nameText_->setCaption("Vacant");
  nameText_->setAlign(Widget::LeftAlign);
  addWidget(nameText_);
}

PersonHoverInfo::~PersonHoverInfo()
{

}

void PersonHoverInfo::setPerson( Person* pers )
{
  if (pers) {
    nameText_->setCaption(pers->getName());
  } else {
    nameText_->setCaption("Vacant");
  }
}
