#include "StdAfx.h"
#include "CompHoverInfo.h"

CompHoverInfo::CompHoverInfo( Rect size ):Window(size)
{
  float top = 0.01f;
  name_ = new Text(Rect(0.01f,top,0.98f,0.1f));
  name_->setAlign(Widget::LeftAlign);
  addWidget(name_);
  top += 0.1f;

  requiredConnectionsTA_ = new TextArea(Rect(0.01f, top, 0.98f, 0.3f));
  addWidget(requiredConnectionsTA_);
  top += 0.3f;
}

CompHoverInfo::~CompHoverInfo()
{

}

void CompHoverInfo::setCompartment( Compartment* comp )
{
  requiredConnectionsTA_->clearLines();
  name_->setCaption(comp->getName());
  if (comp->getRequiredConnections().size() == 0) {
    requiredConnectionsTA_->setVisible(false);
  } else {
    requiredConnectionsTA_->setVisible(true);
    for (auto itr = comp->getRequiredConnections().begin(); itr != comp->getRequiredConnections().end(); ++itr ){
      CString requiredConnection = *itr;
      if (comp->isConnectedTo(requiredConnection)) {
        requiredConnection += " - OK";
      } else {
        requiredConnection += " - NOT CONNECTED";
      }
      requiredConnectionsTA_->pushLine(requiredConnection);
    }
  }
}
