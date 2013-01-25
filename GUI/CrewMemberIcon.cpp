#include "StdAfx.h"
#include "CrewMemberIcon.h"
#include "Renderer.h"

CrewMemberIcon::CrewMemberIcon( Rect size ):Widget(size),person_(NULL)
{

}

CrewMemberIcon::~CrewMemberIcon()
{

}

void CrewMemberIcon::render()
{
  Renderer& renderer = Renderer::getInstance();
  Rect texPos(0.0f, 64/(float)renderer.getHeadsTexSize().y, 64/(float)renderer.getHeadsTexSize().x, 64/(float)renderer.getHeadsTexSize().y);
  if (person_) {
    texPos.left = person_->getHeadTexX();
  }
  renderer.drawTexRect(size_, renderer.getHeadsTex(), texPos);
}

void CrewMemberIcon::setPerson( Person* pers )
{
  person_ = pers;
  if (pers) {
    draggable_ = true;
  } else {
    draggable_ = false;
  }
}
