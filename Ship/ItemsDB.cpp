#include "StdAfx.h"
#include "ItemsDB.h"
#include "RoomParser.h"

ItemsDB& ItemsDB::getInstance()
{
  static ItemsDB instance;
  return instance;
}

ItemsDB::ItemsDB()
{

}

ItemsDB::~ItemsDB()
{

}

bool ItemsDB::isValidCategory( int category )
{
  return category >= 0 && category < Compartment::LastCategory;
}

void ItemsDB::addItem( Item* item )
{
  if (itemsMap_.count(item->getID()) != 0) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Item already exists in the DB: " + CString(item->getID()));
    return;
  }
  itemsMap_[item->getID()] = item;
}

void ItemsDB::addCompartment( Compartment* comp )
{
  assert (isValidCategory(comp->getCategory()));
  if (roomsMap_.count(comp->getCategory()) == 0) {
    list<Compartment*> compList;
    compList.push_back(comp);
    roomsMap_[comp->getCategory()] = compList;
  } else {
    roomsMap_[comp->getCategory()].push_back(comp);
  }
}

list<Compartment*> ItemsDB::getCompartmentsByCategory( Compartment::Category category )
{
  assert (isValidCategory(category));
  return roomsMap_[category];
}

Item* ItemsDB::getItemByID( int id )
{
  assert (itemsMap_.count(id) != 0);
  return itemsMap_[id];
}

bool ItemsDB::init()
{
  RoomParser parser;
  bool res = parser.parse("res/rooms.txt");
  return res;
}

