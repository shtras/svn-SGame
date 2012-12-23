#pragma once
#include "TexturedWidget.h"

class GUIManager
{
public:
  GUIManager();
  ~GUIManager();
  void addWidget(Widget* widget);
  void removeWidget(Widget* widget);
private:
  list<Widget*> widgets_; //1-st level widgets. Each widget deals with its children. Or not.
  //For a while this is unused. Sad but true
};
