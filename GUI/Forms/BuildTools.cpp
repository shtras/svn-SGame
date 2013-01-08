#include "StdAfx.h"
#include "BuildTools.h"
#include "Renderer.h"
#include "ItemsDB.h"
#include "CompartmentButton.h"

BuildTools::BuildTools( Rect size ):Window(size),firstButton_(-1), currentCategory_((Compartment::Category)0)
{
  draggable_ = false;
  clickable_ = true;
}

BuildTools::~BuildTools()
{

}

void BuildTools::init(ShipView* shipView)
{
  shipView_ = shipView;
  Rect toolButtonRect(0.02, 0.005, 0.15, 0.05);
  buildButton_ = new Button(toolButtonRect, false);
  TextureParams regular = {100, 0, 54, 37, 10, 6, 10, 6};
  TextureParams hovered = {155, 0, 54, 37, 10, 6, 10, 6};
  TextureParams pressed = {210, 0, 54, 37, 10, 6, 10, 6};
  buildButton_->init(regular, hovered, pressed);
  buildButton_->sigClick.connect(this, &BuildTools::buildClick);
  buildButton_->setHighlighted(true);
  addWidget(buildButton_);
  buildButton_->setToolTip("Build walls");
  toolButtonRect.left += toolButtonRect.width;

  floorButton_ = new Button(toolButtonRect, false);
  TextureParams regular2 = {100, 76, 54, 37, 10, 6, 10, 6};
  TextureParams hovered2 = {155, 76, 54, 37, 10, 6, 10, 6};
  TextureParams pressed2 = {210, 76, 54, 37, 10, 6, 10, 6};
  floorButton_->init(regular2, hovered2, pressed2);
  floorButton_->sigClick.connect(this, &BuildTools::floorClick);
  addWidget(floorButton_);
  floorButton_->setToolTip("Build floor");
  toolButtonRect.left += toolButtonRect.width;

  doorButton_ = new Button(toolButtonRect, false);
  TextureParams regular3 = {100, 114, 54, 37, 10, 6, 10, 6};
  TextureParams hovered3 = {155, 114, 54, 37, 10, 6, 10, 6};
  TextureParams pressed3 = {210, 114, 54, 37, 10, 6, 10, 6};
  doorButton_->init(regular3, hovered3, pressed3);
  doorButton_->sigClick.connect(this, &BuildTools::doorClick);
  addWidget(doorButton_);
  doorButton_->setToolTip("Build door");
  toolButtonRect.left += toolButtonRect.width;

  eraseButton_ = new Button(toolButtonRect, false);
  TextureParams regular1 = {100, 38, 54, 37, 10, 6, 10, 6};
  TextureParams hovered1 = {155, 38, 54, 37, 10, 6, 10, 6};
  TextureParams pressed1 = {210, 38, 54, 37, 10, 6, 10, 6};
  eraseButton_->init(regular1, hovered1, pressed1);
  eraseButton_->sigClick.connect(this, &BuildTools::eraseClick);
  addWidget(eraseButton_);
  eraseButton_->setToolTip("Demolish");
  toolButtonRect.left += toolButtonRect.width;

  selectButton_ = new Button(toolButtonRect, false);
  TextureParams regular4 = {100, 183, 54, 37, 10, 6, 10, 6};
  TextureParams hovered4 = {155, 183, 54, 37, 10, 6, 10, 6};
  TextureParams pressed4 = {210, 183, 54, 37, 10, 6, 10, 6};
  selectButton_->init(regular4, hovered4, pressed4);
  selectButton_->sigClick.connect(this, &BuildTools::selectClick);
  addWidget(selectButton_);
  selectButton_->setToolTip("Select");
  toolButtonRect.left = 0.02f;
  toolButtonRect.top += toolButtonRect.height;

  entranceButton_ = new Button(toolButtonRect, false);
  TextureParams regular5 = {265, 38, 54, 37, 10, 6, 10, 6};
  TextureParams hovered5 = {320, 38, 54, 37, 10, 6, 10, 6};
  TextureParams pressed5 = {375, 38, 54, 37, 10, 6, 10, 6};
  entranceButton_->init(regular5, hovered5, pressed5);
  entranceButton_->sigClick.connect(this, &BuildTools::entranceClick);
  addWidget(entranceButton_);
  entranceButton_->setToolTip("Choose entrance");
  toolButtonRect.left += toolButtonRect.width;

  connectionButton_ = new Button(toolButtonRect, false);
  TextureParams regular6 = {265, 76, 54, 37, 10, 6, 10, 6};
  TextureParams hovered6 = {320, 76, 54, 37, 10, 6, 10, 6};
  TextureParams pressed6 = {375, 76, 54, 37, 10, 6, 10, 6};
  connectionButton_->init(regular6, hovered6, pressed6);
  connectionButton_->sigClick.connect(this, &BuildTools::createConnectionClick);
  addWidget(connectionButton_);
  connectionButton_->setToolTip("Create connection");
  toolButtonRect.left += toolButtonRect.width;

  Button* upButton = new Button(Rect(0.8, 0.01, 0.08, 0.023));
  upButton->setCaption("+");
  addWidget(upButton);
  upButton->setColor(Vector4(255,0,0,255));
  upButton->sigClick.connect(this, &BuildTools::upClick);
  upButton->setToolTip("Up one deck");
  Button* downButton = new Button(Rect(0.8, 0.05, 0.08, 0.023));
  downButton->setCaption("-");
  addWidget(downButton);
  downButton->setColor(Vector4(255,0,0,255));
  downButton->sigClick.connect(this, &BuildTools::downClick);
  downButton->setToolTip("Down one deck");
  deckText_ = new Text(Rect(0.8, 0.03, 0.08, 0.02));
  deckText_->setCaption("D" + CString(shipView_->getActiveDeck()+1));
  addWidget(deckText_);
  deckText_->setToolTip("Current deck");

  Button* upGhostButton = new Button(Rect(0.88, 0.01, 0.08, 0.023));
  upGhostButton->setCaption("+");
  addWidget(upGhostButton);
  upGhostButton->setColor(Vector4(255,0,0,255));
  upGhostButton->sigClick.connect(this, &BuildTools::ghostUpClick);
  upGhostButton->setToolTip("Ghost deck up");
  Button* downGhostButton = new Button(Rect(0.88, 0.05, 0.08, 0.023));
  downGhostButton->setCaption("-");
  addWidget(downGhostButton);
  downGhostButton->setColor(Vector4(255,0,0,255));
  downGhostButton->sigClick.connect(this, &BuildTools::ghostDownClick);
  downGhostButton->setToolTip("Ghost deck down");
  ghostDeckText_ = new Text(Rect(0.88, 0.03, 0.08, 0.02));
  updateGhostText();
  addWidget(ghostDeckText_);
  ghostDeckText_->setToolTip("Ghost deck");

  float lastY = (float)(toolButtonRect.top + toolButtonRect.height + 0.01);

  progressBar_ = new ProgressBar(Rect(0.95, 0.23, 0.03, 0.75));
  addWidget(progressBar_);
  progressBar_->setParams(10, 2, 7);

  float lastX = 0.02f;
  firstButton_ = 0;
  float width = 0.317f;
  int currentCount = 0;
  for (int i=0; i<Compartment::LastCategory; ++i) {
    Rect size(lastX, lastY, width, 0.05f);
    Button* but = new Button(size);
    but->setCaption(Compartment::categoryName((Compartment::Category)i));
    but->setParam((void*)i);
    but->sig1Click.connect(this, &BuildTools::categoryClick);
    but->setColor(Vector4(255,0,0,255));
    categoryButtons_.push_back(but);
    addWidget(but);
    lastX += width + 0.001f;
    if (lastX >= 0.7f) {
      lastX = 0.02f;
      lastY += 0.05f;
    }

    list<Compartment*> comps = ItemsDB::getInstance().getCompartmentsByCategory((Compartment::Category)i);
    float lastTop = 0.25f;
    float aspect = size_.width / size_.height * Renderer::getInstance().getWidth() / (float)Renderer::getInstance().getHeight();
    int count = 0;
    compButtons_[i] = vector<CompartmentButton*>();
    for (auto itr = comps.begin(); itr != comps.end(); ++itr) {
      Compartment* comp = *itr;
      float width = 0.85f;
      if (comp->getWidth() < 5) {
        width = 0.85f / (5-(float)comp->getWidth());
      }
      float height = width * aspect * comp->getHeight() / (float)comp->getWidth();
      if (height > 0.25f) {
        width *= 0.5f;
        height *= 0.5f;
      }
      CompartmentButton* button = new CompartmentButton(Rect(0.02f, lastTop, width, height), comp, shipView_);
      addWidget(button);
      button->setToolTip(comp->getName());
      compButtons_[i].push_back(button);
      lastTop += height + 0.02f;
      if (lastTop >= 0.95f || i != currentCategory_) {
        button->setVisible(false);
      } else {
        ++count;
      }
    }
    if (i == currentCategory_) {
      currentCount = count;
    }
  }
  progressBar_->setParams(compButtons_[currentCategory_].size(), 0, currentCount);
  categoryButtons_[currentCategory_]->setHighlighted(true);
}

void BuildTools::categoryClick(void* param)
{
  int iParam = (int)param;
  for (int i=0; i<Compartment::LastCategory; ++i) {
    Button* but = categoryButtons_[i];
    but->setHighlighted(i == iParam);
  }
  for (auto itr = compButtons_[currentCategory_].begin(); itr != compButtons_[currentCategory_].end(); ++itr) {
    (*itr)->setVisible(false);
  }
  currentCategory_ = (Compartment::Category)iParam;
  firstButton_ = 0;
  if (compButtons_[currentCategory_].size() == 0) {
    progressBar_->setParams(1,0,1);
    return;
  }
  float lastTop = compButtons_[currentCategory_][0]->getSize().top;
  int last = 0;
  int i = 0;
  int SZ = compButtons_[currentCategory_].size();
  for (auto itr = compButtons_[currentCategory_].begin(); itr != compButtons_[currentCategory_].end(); ++itr) {
    CompartmentButton* but = *itr;
    Rect size = but->getSize();
    size.top = lastTop;
    but->resize(size);
    but->setVisible(true);
    lastTop += size.height + 0.02f;
    if (lastTop >= 0.98f) {
      if (!last) {
        last = i;
      }
      but->setVisible(false);
    }
    ++i;
  }
  if (last == 0) {
    last = SZ;
  }
  progressBar_->setParams(SZ, 0, last);
}

void BuildTools::buildClick()
{
  shipView_->setAction(ShipView::BuildWalls);
  buildButton_->setHighlighted(true);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(false);
  selectButton_->setHighlighted(false);
  entranceButton_->setHighlighted(false);
  connectionButton_->setHighlighted(false);
}

void BuildTools::eraseClick()
{
  shipView_->setAction(ShipView::Erase);
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(true);
  selectButton_->setHighlighted(false);
  entranceButton_->setHighlighted(false);
  connectionButton_->setHighlighted(false);
}

void BuildTools::floorClick()
{
  shipView_->setAction(ShipView::BuildFloor);
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(true);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(false);
  selectButton_->setHighlighted(false);
  entranceButton_->setHighlighted(false);
  connectionButton_->setHighlighted(false);
}

void BuildTools::doorClick()
{
  shipView_->setAction(ShipView::BuildDoor);
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(true);
  eraseButton_->setHighlighted(false);
  selectButton_->setHighlighted(false);
  entranceButton_->setHighlighted(false);
  connectionButton_->setHighlighted(false);
}

void BuildTools::selectClick()
{
  shipView_->setAction(ShipView::Select);
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(false);
  selectButton_->setHighlighted(true);
  entranceButton_->setHighlighted(false);
  connectionButton_->setHighlighted(false);
}

void BuildTools::entranceClick()
{
  shipView_->setAction(ShipView::ChooseEntrance);
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(false);
  selectButton_->setHighlighted(false);
  entranceButton_->setHighlighted(true);
  connectionButton_->setHighlighted(false);
}

void BuildTools::createConnectionClick()
{
  shipView_->setAction(ShipView::CreateConnection);
  buildButton_->setHighlighted(false);
  floorButton_->setHighlighted(false);
  doorButton_->setHighlighted(false);
  eraseButton_->setHighlighted(false);
  selectButton_->setHighlighted(false);
  entranceButton_->setHighlighted(false);
  connectionButton_->setHighlighted(true);
}

void BuildTools::onMouseWheelScroll( int direction )
{
  if (compButtons_[currentCategory_].size() == 0) {
    return;
  }
  if (direction == -1 && compButtons_[currentCategory_][compButtons_[currentCategory_].size()-1]->isVisible()) {
    return;
  }
  firstButton_ -= direction;
  if (firstButton_ < 0) {
    firstButton_ = 0;
    return;
  }
  float lastTop = compButtons_[currentCategory_][0]->getSize().top;
  bool fitsScreen = true;
  int last = 0;
  int SZ = (int)compButtons_[currentCategory_].size();
  for (int i=0; i<SZ; ++i) {
    CompartmentButton* button = compButtons_[currentCategory_][i];
    Rect oldSize = button->getSize();
    if (i < firstButton_ || !fitsScreen) {
      button->setVisible(false);
    } else {
      button->setVisible(true);
      oldSize.top = lastTop;
      button->resize(oldSize);
      lastTop += oldSize.height + 0.02f;
      if (lastTop >= 0.95f) {
        if (last == 0) {
          last = i;
        }
        fitsScreen = false;
        button->setVisible(false);
      }
    }
  }
  if (last == 0) {
    last = SZ;
  }
  progressBar_->setParams(SZ, firstButton_, last);
}

void BuildTools::upClick()
{
  shipView_->deckUp();
  deckText_->setCaption("D" + CString(shipView_->getActiveDeck()+1));
  ghostUpClick();
}

void BuildTools::downClick()
{
  shipView_->deckDown();
  deckText_->setCaption("D" + CString(shipView_->getActiveDeck()+1));
  ghostDownClick();
}

void BuildTools::ghostUpClick()
{
  shipView_->ghostDeckUp();
  updateGhostText();
}

void BuildTools::ghostDownClick()
{
  shipView_->ghostDeckDown();
  updateGhostText();
}

void BuildTools::updateGhostText()
{
  int ghostDeckIdx = shipView_->getGhostDeck();
  if (ghostDeckIdx == -1) {
    ghostDeckText_->setCaption("-");
  } else {
    ghostDeckText_->setCaption("G" + CString(ghostDeckIdx+1));
  }
}
