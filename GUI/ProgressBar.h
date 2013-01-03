#pragma once
#include "TexturedWidget.h"

class ProgressBar: public Widget
{
public:
  ProgressBar(Rect size);
  ~ProgressBar();
  void setParams(int numItems, int firstItem, int lastItem);
  void customFrameRender();
private:
  TexturedWidget* bg_;
  TexturedWidget* selection_;
  int numItems_;
  int firstItem_;
  int lastItem_;
};
