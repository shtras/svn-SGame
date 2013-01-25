#pragma once
#include "TexturedWidget.h"
#include "Text.h"
#include "Button.h"

class TimeControl: public Widget, public has_slots<>
{
public:
  TimeControl(Rect size);
  ~TimeControl();
  void progressTime();
private:
  void pauseClick();
  FramedText* timeStr_;
  Button* pauseButton_;
};
