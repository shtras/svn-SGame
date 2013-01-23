#include "StdAfx.h"
#include "Credits.h"
#include "Button.h"
#include "SGame.h"


Credits::Credits( Rect size ):Widget(size)
{
  Button* back = new Button(Rect(0.45, 0.85, 0.1, 0.05));
  back->setCaption("Back");
  back->setColor(Vector4(255,0,0,255));
  back->sigClick.connect(&SGame::getInstance(), &SGame::toggleMenu);
  addWidget(back);

  double left = 0.2;
  double width = 0.6;
  double top = 0.1;
  double height = 0.02;
  Text* t = new Text(Rect(left, top, width, height));
  t->setCaption("Stellar Nomads");
  t->setTextSize(3.0f);
  addWidget(t);
  top += height + 0.1;

  t = new Text(Rect(left, top, width, height));
  t->setCaption("Idea, Code, Art:");
  t->setTextSize(1.5f);
  addWidget(t);
  top += height;

  t = new Text(Rect(left, top, width, height));
  t->setCaption("Gregory \"NetStranger\" Shtrasberg");
  addWidget(t);
  top += height + 0.05;

  t = new Text(Rect(left, top, width, height));
  t->setCaption("Special thanks to:");
  t->setTextSize(1.5f);
  addWidget(t);
  top += height;

  //t = new Text(Rect(left, top, width, height));
  //t->setCaption("Craig Gotsman Ph.D.");
  //addWidget(t);
  //top += height;

  //t = new Text(Rect(left, top, width, height));
  //t->setCaption("Amit Mano");
  //addWidget(t);
  //top += height;

  //t = new Text(Rect(left, top, width, height));
  //t->setCaption("Amir Vaxman");
  //addWidget(t);
  //top += height;

  t = new Text(Rect(left, top, width, height));
  t->setCaption("Nika the cat");
  addWidget(t);
  top += height;

  t = new Text(Rect(left, top, width, height));
  t->setCaption("Gamedev.ru");
  addWidget(t);
  top += height;
}

Credits::~Credits()
{

}
