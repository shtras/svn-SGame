#include "StdAfx.h"
#include "ShipEditor.h"
#include "Window.h"
#include "Renderer.h"
#include "BuildTools.h"
#include "SGame.h"

ShipEditor::ShipEditor( Rect size ):Widget(size)
{
  Window* topPanel = new Window(Rect(0.0, 0.0, 1.0, 0.05));
  addWidget(topPanel);

  double buttonWidth = 0.1;
  double buttonLeft = 0.5 - buttonWidth*2.0;
  double buttonTop = 0.1;
  double buttonHeight = 1.0 - buttonTop*2.0;

  Button* menuButton = new Button(Rect(0.9, buttonTop, 0.098, buttonHeight));
  menuButton->setCaption("Quit");
  menuButton->setColor(Vector4(255, 0, 0, 255));
  topPanel->addWidget(menuButton);
  menuButton->sigClick.connect(&SGame::getInstance(), &SGame::toggleMenu);

  Button* saveButton = new Button(Rect(0.002, buttonTop, 0.1, buttonHeight));
  saveButton->setCaption("Save");
  saveButton->setColor(Vector4(255,0,0,255));
  saveButton->sigClick.connect(this, &ShipEditor::saveClick);
  topPanel->addWidget(saveButton);

  Button* loadButton = new Button(Rect(0.102, buttonTop, 0.1, buttonHeight));
  loadButton->setCaption("Load");
  loadButton->setColor(Vector4(255,0,0,255));
  loadButton->sigClick.connect(this, &ShipEditor::loadClick);
  topPanel->addWidget(loadButton);

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

  view_ = new ShipViewEditor(Rect(0.2, 0.05, 0.6, 0.95));
  addWidget(view_);

  BuildTools* tools = new BuildTools(Rect(0.0, 0.05, 0.2, 0.95));
  tools->init(view_);
  addWidget(tools);

  info_ = new BuildInfo(Rect(0.8, 0.05, 0.2, 0.95));
  info_->init(view_);
  addWidget(info_);
  view_->setBuildInfo(info_);

  openDialog_ = new FileOpenDialog(Rect(0.3, 0.2, 0.4, 0.6));
  addWidget(openDialog_);
  openDialog_->Finished.connect(this, &ShipEditor::onDialogClose);
  openDialog_->setExtension("sgs");
}

ShipEditor::~ShipEditor()
{
}

void ShipEditor::constructionClick()
{
  view_->constructionOverview();
  constructionOverlayButton_->setHighlighted(true);
  accessibilityOverlayButton_->setHighlighted(false);
  powerOverlayButton_->setHighlighted(false);
  oxygenOverlayButton_->setHighlighted(false);
}

void ShipEditor::accessibilityClick()
{
  view_->accessibilityOverview();
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

void ShipEditor::saveClick()
{
  saving_ = true;
  view_->setVisible(false);
  openDialog_->show();
}

void ShipEditor::loadClick()
{
  saving_ = false;
  view_->setVisible(false);
  openDialog_->show();
}

void ShipEditor::onDialogClose()
{
  view_->setVisible(true);
  if (!openDialog_->getResult()) {
    return;
  }
  CString fileName = openDialog_->getFileName();
  if (saving_) {
    view_->saveShip(fileName);
  } else {
    ifstream testFile("saves/" + fileName);
    if (!testFile) {
      info_->addLogMessage("File not found");
      return;
    }
    testFile.close();
    view_->loadShip(fileName);
  }
}

