#include "StdAfx.h"
#include "TopPanel.h"
#include "TimeControl.h"


TopPanel::TopPanel( Rect size, ShipView* shipView, bool editor ):Window(size),shipView_(shipView), editor_(editor)
{
  double buttonWidth = 0.08;
  double buttonLeft = 0.5 - buttonWidth*2.0;
  double buttonTop = 0.1;
  double buttonHeight = 1.0 - buttonTop*2.0;

  MenuButton = new Button(Rect(0.9, buttonTop, 0.098, buttonHeight));
  MenuButton->setCaption("Quit");
  MenuButton->setColor(Vector4(255, 0, 0, 255));
  addWidget(MenuButton);

  SaveButton = new Button(Rect(0.002, buttonTop, 0.06, buttonHeight));
  SaveButton->setCaption("Save");
  SaveButton->setColor(Vector4(255,0,0,255));
  addWidget(SaveButton);

  LoadButton = new Button(Rect(0.062, buttonTop, 0.06, buttonHeight));
  LoadButton->setCaption("Load");
  LoadButton->setColor(Vector4(255,0,0,255));
  addWidget(LoadButton);

  constructionOverlayButton_ = new Button(Rect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
  if (editor) {
    constructionOverlayButton_->setCaption("Construction");
  } else {
    constructionOverlayButton_->setCaption("Ship");
  }
  addWidget(constructionOverlayButton_);
  constructionOverlayButton_->sigClick.connect(this, &TopPanel::constructionClick);
  constructionOverlayButton_->setColor(Vector4(255,0,0,255));
  buttonLeft += buttonWidth;
  constructionOverlayButton_->setHighlighted(true);

  accessibilityOverlayButton_ = new Button(Rect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
  accessibilityOverlayButton_->setCaption("Accessibility");
  addWidget(accessibilityOverlayButton_);
  accessibilityOverlayButton_->setColor(Vector4(255,0,0,255));
  accessibilityOverlayButton_->sigClick.connect(this, &TopPanel::accessibilityClick);
  buttonLeft += buttonWidth;

  powerOverlayButton_ = new Button(Rect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
  powerOverlayButton_->setCaption("Power");
  addWidget(powerOverlayButton_);
  powerOverlayButton_->setColor(Vector4(255,0,0,255));
  powerOverlayButton_->sigClick.connect(this, &TopPanel::powerclick);
  buttonLeft += buttonWidth;

  oxygenOverlayButton_ = new Button(Rect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
  oxygenOverlayButton_->setCaption("Oxygen");
  addWidget(oxygenOverlayButton_);
  oxygenOverlayButton_->setColor(Vector4(255,0,0,255));
  oxygenOverlayButton_->sigClick.connect(this, &TopPanel::oxygenClick);
  buttonLeft += buttonWidth;

  if (!editor_) {
    TimeControl* control = new TimeControl(Rect(buttonLeft, buttonTop, 0.2, buttonHeight));
    addWidget(control);
  }
}

TopPanel::~TopPanel()
{

}

void TopPanel::constructionClick()
{
  shipView_->constructionOverview();
  constructionOverlayButton_->setHighlighted(true);
  accessibilityOverlayButton_->setHighlighted(false);
  powerOverlayButton_->setHighlighted(false);
  oxygenOverlayButton_->setHighlighted(false);
}

void TopPanel::accessibilityClick()
{
  shipView_->accessibilityOverview();
  constructionOverlayButton_->setHighlighted(false);
  accessibilityOverlayButton_->setHighlighted(true);
  powerOverlayButton_->setHighlighted(false);
  oxygenOverlayButton_->setHighlighted(false);
}

void TopPanel::powerclick()
{
  constructionOverlayButton_->setHighlighted(false);
  accessibilityOverlayButton_->setHighlighted(false);
  powerOverlayButton_->setHighlighted(true);
  oxygenOverlayButton_->setHighlighted(false);
}

void TopPanel::oxygenClick()
{
  constructionOverlayButton_->setHighlighted(false);
  accessibilityOverlayButton_->setHighlighted(false);
  powerOverlayButton_->setHighlighted(false);
  oxygenOverlayButton_->setHighlighted(true);
}
