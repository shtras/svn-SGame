#pragma  once
#include "TexturedWidget.h"
#include "Button.h"
#include "ShipViewEditor.h"
#include "FileOpenDialog.h"
#include "BuildInfo.h"

class ShipEditor: public Widget, public has_slots<>
{
public:
  ShipEditor(Rect size);
  ~ShipEditor();
private:
  void saveClick();
  void loadClick();
  void onDialogClose();


  ShipViewEditor* shipView_;
  BuildInfo* info_;
  FileOpenDialog* openDialog_;
  bool saving_;
};
