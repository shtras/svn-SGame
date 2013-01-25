#include "StdAfx.h"
#include "ShipEditor.h"
#include "Window.h"
#include "Renderer.h"
#include "BuildTools.h"
#include "SGame.h"
#include "TopPanel.h"

ShipEditor::ShipEditor( Rect size ):Widget(size)
{

  shipView_ = new ShipViewEditor(Rect(0.2, 0.05, 0.6, 0.95));
  addWidget(shipView_);

  TopPanel* topPanel = new TopPanel(Rect(0.0, 0.0, 1.0, 0.05), shipView_, true);
  addWidget(topPanel);

  topPanel->MenuButton->sigClick.connect(&SGame::getInstance(), &SGame::toggleMenu);
  topPanel->SaveButton->sigClick.connect(this, &ShipEditor::saveClick);
  topPanel->LoadButton->sigClick.connect(this, &ShipEditor::loadClick);

  BuildTools* tools = new BuildTools(Rect(0.0, 0.05, 0.2, 0.95));
  tools->init(shipView_);
  addWidget(tools);

  info_ = new BuildInfo(Rect(0.8, 0.05, 0.2, 0.95));
  info_->init(shipView_);
  addWidget(info_);
  shipView_->setBuildInfo(info_);

  openDialog_ = new FileOpenDialog(Rect(0.3, 0.2, 0.4, 0.6));
  addWidget(openDialog_);
  openDialog_->Finished.connect(this, &ShipEditor::onDialogClose);
  openDialog_->setExtension("sgs");
}

ShipEditor::~ShipEditor()
{
}

void ShipEditor::saveClick()
{
  saving_ = true;
  shipView_->setVisible(false);
  openDialog_->show();
}

void ShipEditor::loadClick()
{
  saving_ = false;
  shipView_->setVisible(false);
  openDialog_->show();
}

void ShipEditor::onDialogClose()
{
  shipView_->setVisible(true);
  if (!openDialog_->getResult()) {
    return;
  }
  CString fileName = openDialog_->getFileName();
  if (saving_) {
    shipView_->saveShip(fileName);
  } else {
    ifstream testFile("saves/" + fileName);
    if (!testFile) {
      info_->addLogMessage("File not found");
      return;
    }
    testFile.close();
    shipView_->loadShip(fileName);
  }
}

