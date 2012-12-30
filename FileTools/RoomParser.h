#pragma once
#include "Ship.h"

class RoomParser
{
public:
  RoomParser();
  ~RoomParser();
  void parse(CString fileName);
private:
  map<int, Item*> itemsMap_;
  map<int, list<Compartment*> > roomsMap_;
  
}
