#include "StdAfx.h"
#include "PersonHoverInfo.h"
#include "CrewMemberIcon.h"
#include "Ship.h"

PersonHoverInfo::PersonHoverInfo(Rect size):Window(size),pers_(NULL)
{
  Rect pos(0.02, 0.02, 0.98, 0.1);
  nameText_ = new Text(pos);
  nameText_->setCaption("Vacant");
  nameText_->setAlign(Widget::LeftAlign);
  addWidget(nameText_);
  pos.top += pos.height;

  livingText_ = new Text(pos);
  livingText_->setCaption("Quarter: NO");
  livingText_->setAlign(Widget::LeftAlign);
  addWidget(livingText_);
  pos.top += pos.height;

  for (int i=0; i<3; ++i) {
    watchText_[i] = new Text(pos);
    watchText_[i]->setCaption("Shift " + CString(i+1) + ": NO");
    watchText_[i]->setAlign(Widget::LeftAlign);
    addWidget(watchText_[i]);
    pos.top += pos.height;
  }
}

PersonHoverInfo::~PersonHoverInfo()
{

}

void PersonHoverInfo::setPerson( Person* pers )
{
  pers_ = pers;
  if (pers) {
    nameText_->setCaption(pers->getName());
    livingText_->setCaption("Quarter: NO");

    for (int i=0; i<3; ++i) {
      CString watchCompName = "NO";
      if (pers_ && pers_->getWatchIcon(i)) {
        watchCompName = pers_->getWatchIcon(i)->getComp()->getFullName();
      }
      watchText_[i]->setCaption("Shift " + CString(i+1) + ": " + watchCompName);
    }
  } else {
    nameText_->setCaption("Vacant");
    livingText_->setCaption("Quarter: NO");

    for (int i=0; i<3; ++i) {
      watchText_[i]->setCaption("Shift " + CString(i+1) + ": NO");
    }
  }
}

Person* PersonHoverInfo::getPerson()
{
  return pers_;
}
