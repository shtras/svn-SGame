#pragma once
#include "TexturedWidget.h"
#include "ProgressBar.h"

class TextArea: public Widget
{
public:
  TextArea(Rect size);
  ~TextArea();
  virtual void onResize();
  void pushLine(CString line);
  void onMouseWheelScroll(int direction);
  void render();
  void clearLines();
private:
  void resizePB();
  list<CString> lines_;
  int maxSize_;
  int first_;
  int maxLinesDisplayed_;
  ProgressBar* pb_;
};
