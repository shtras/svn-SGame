#include "StdAfx.h"
#include "GameWindow.h"
#include "TopPanel.h"
#include "SGame.h"

GameWindow::GameWindow( Rect size ):Widget(size)
{
  shipView_ = new ShipViewGame(Rect(0.2, 0.05, 0.6, 0.95));
  addWidget(shipView_);

  TopPanel* topPanel = new TopPanel(Rect(0.0, 0.0, 1.0, 0.05), shipView_);
  addWidget(topPanel);

  topPanel->MenuButton->sigClick.connect(&SGame::getInstance(), &SGame::toggleMenu);

  crewList_ = new CrewList(Rect(0.0, 0.05, 0.2, 1.0));
  addWidget(crewList_);
}

GameWindow::~GameWindow()
{
}

bool GameWindow::init( GameWorld* world )
{
  world_ = world;
  shipView_->setShip(world_->getPlayerShip());

  crewList_->init(world_->getPlayerShip());
  return true;
}
