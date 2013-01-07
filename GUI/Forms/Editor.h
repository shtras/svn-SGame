#pragma  once
#include "TexturedWidget.h"
#include "Button.h"

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

  Button* constructionOverlayButton_;
  Button* accessibilityOverlayButton_;
  Button* powerOverlayButton_;
  Button* oxygenOverlayButton_;
};
