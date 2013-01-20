#pragma once
#include "Window.h"
#include "Text.h"
#include "TextArea.h"
#include "ShipView.h"

class BuildInfo: public Window
{
public:
  BuildInfo(Rect size);
  ~BuildInfo();
  void init(ShipView* shipView);
  void updateValues();
  void addLogMessage(CString message) {ta_->pushLine(message);}
private:
  FramedText* crewText_;
  FramedText* powerText_;
  FramedText* structureStatusText_;
  FramedText* accessibilityStatusText_;
  FramedText* connectionsStatusText_;
  FramedText* generalStatusText_;
  TextArea* ta_;
  ShipView* shipView_;
};
