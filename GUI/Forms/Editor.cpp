#include "StdAfx.h"
#include "Editor.h"
#include "Window.h"
#include "Renderer.h"
#include "BuildTools.h"
#include "BuildInfo.h"
#include "SGame.h"

ShipEditor::ShipEditor( Rect size ):Widget(size)
{
  Window* topPanel = new Window(Rect(0.0, 0.0, 1.0, 0.05));
  addWidget(topPanel);

  Button* menuButton = new Button(Rect(0.9, 0.1, 0.098, 0.8));
  menuButton->setCaption("Quit");
  menuButton->setColor(Vector4(255, 0, 0, 255));
  menuButton->setTextSize(1.8f);
  topPanel->addWidget(menuButton);
  menuButton->sigClick.connect(&SGame::getInstance(), &SGame::toggleMenu);

  double buttonWidth = 0.1;
  double buttonLeft = 0.5 - buttonWidth*2.0;
  double buttonTop = 0.1;
  double buttonHeight = 1.0 - buttonTop*2.0;

  constructionOverlayButton_ = new Button(Rect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
  constructionOverlayButton_->setCaption("Construction");
  topPanel->addWidget(constructionOverlayButton_);
  constructionOverlayButton_->sigClick.connect(this, &ShipEditor::constructionClick);
  constructionOverlayButton_->setColor(Vector4(255,0,0,255));
  buttonLeft += buttonWidth;
  constructionOverlayButton_->setHighlighted(true);

  accessibilityOverlayButton_ = new Button(Rect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
  accessibilityOverlayButton_->setCaption("Accessibility");
  topPanel->addWidget(accessibilityOverlayButton_);
  accessibilityOverlayButton_->setColor(Vector4(255,0,0,255));
  accessibilityOverlayButton_->sigClick.connect(this, &ShipEditor::accessibilityClick);
  buttonLeft += buttonWidth;

  powerOverlayButton_ = new Button(Rect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
  powerOverlayButton_->setCaption("Power");
  topPanel->addWidget(powerOverlayButton_);
  powerOverlayButton_->setColor(Vector4(255,0,0,255));
  powerOverlayButton_->sigClick.connect(this, &ShipEditor::powerclick);
  buttonLeft += buttonWidth;

  oxygenOverlayButton_ = new Button(Rect(buttonLeft, buttonTop, buttonWidth, buttonHeight));
  oxygenOverlayButton_->setCaption("Oxygen");
  topPanel->addWidget(oxygenOverlayButton_);
  oxygenOverlayButton_->setColor(Vector4(255,0,0,255));
  oxygenOverlayButton_->sigClick.connect(this, &ShipEditor::oxygenClick);
  buttonLeft += buttonWidth;

  ShipView* view = new ShipView(Rect(0.2, 0.05, 0.6, 0.95));
  addWidget(view);

  BuildTools* tools = new BuildTools(Rect(0.0, 0.05, 0.2, 0.95));
  tools->init(view);
  addWidget(tools);

  BuildInfo* info = new BuildInfo(Rect(0.8, 0.05, 0.2, 0.95));
  info->init(view);
  addWidget(info);
  view->setBuildInfo(info);
}

ShipEditor::~ShipEditor()
{

}

void ShipEditor::constructionClick()
{
  constructionOverlayButton_->setHighlighted(true);
  accessibilityOverlayButton_->setHighlighted(false);
  powerOverlayButton_->setHighlighted(false);
  oxygenOverlayButton_->setHighlighted(false);
}

void ShipEditor::accessibilityClick()
{
  constructionOverlayButton_->setHighlighted(false);
  accessibilityOverlayButton_->setHighlighted(true);
  powerOverlayButton_->setHighlighted(false);
  oxygenOverlayButton_->setHighlighted(false);
}

void ShipEditor::powerclick()
{
  constructionOverlayButton_->setHighlighted(false);
  accessibilityOverlayButton_->setHighlighted(false);
  powerOverlayButton_->setHighlighted(true);
  oxygenOverlayButton_->setHighlighted(false);
}

void ShipEditor::oxygenClick()
{
  constructionOverlayButton_->setHighlighted(false);
  accessibilityOverlayButton_->setHighlighted(false);
  powerOverlayButton_->setHighlighted(false);
  oxygenOverlayButton_->setHighlighted(true);
}

