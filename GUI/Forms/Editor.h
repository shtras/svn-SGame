#pragma  once
#include "TexturedWidget.h"
#include "Button.h"
#include "ShipView.h"
#include "FileOpenDialog.h"
#include "BuildInfo.h"

class ShipEditor: public Widget, public has_slots<>
{
public:
  ShipEditor(Rect size);
  ~ShipEditor();
private:
  void constructionClick();
  void accessibilityClick();
  void powerclick();
  void oxygenClick();
  void saveClick();
  void loadClick();
  void onDialogClose();

  Button* constructionOverlayButton_;
  Button* accessibilityOverlayButton_;
  Button* powerOverlayButton_;
  Button* oxygenOverlayButton_;
  ShipView* view_;
  BuildInfo* info_;
  FileOpenDialog* openDialog_;
  bool saving_;
};
