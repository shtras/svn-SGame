#include "StdAfx.h"
#include "TimeControl.h"
#include "SGame.h"
#include "Time.h"


TimeControl::TimeControl( Rect size ):Widget(size)
{
  timeStr_ = new FramedText(Rect(0.0, 0.0, 0.8, 1.0));
  timeStr_->setCaption(Time::getTime().getTimeStr());
  addWidget(timeStr_);

  pauseButton_ = new Button(Rect(0.8, 0.0, 0.2, 1.0));
  pauseButton_->setCaption("||");
  pauseButton_->setHighlighted(true);
  pauseButton_->setColor(Vector4(255,0,0,255));
  addWidget(pauseButton_);
  pauseButton_->sigClick.connect(this, &TimeControl::pauseClick);
  Time::getTime().setTimeControl(this);
}

TimeControl::~TimeControl()
{
  Time::getTime().setTimeControl(NULL);
}

void TimeControl::pauseClick()
{
  SGame::getInstance().togglePause();
  pauseButton_->setHighlighted(!pauseButton_->isHighlighted());
}

void TimeControl::progressTime()
{
  pauseButton_->setHighlighted(SGame::getInstance().isPaused());
  timeStr_->setCaption(Time::getTime().getTimeStr());
}
