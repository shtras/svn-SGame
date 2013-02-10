#include "StdAfx.h"
#include "ItemsDB.h"
#include "RoomParser.h"

ItemsDB& ItemsDB::getInstance()
{
  static ItemsDB instance;
  return instance;
}

ItemsDB::ItemsDB():hash_(0)
{

}

ItemsDB::~ItemsDB()
{
  for (auto itr = itemsMap_.begin(); itr != itemsMap_.end(); ++itr) {
    delete (*itr).second;
  }
  for (auto itr = compartmentsMap_.begin(); itr != compartmentsMap_.end(); ++itr) {
    delete (*itr).second;
  }
  //for (auto itr = compsCategoryMap_.begin(); itr != compsCategoryMap_.end(); ++itr) {
  //  for (auto itr1 = (*itr).second.begin(); itr1 != (*itr).second.end(); ++itr1) {
  //    delete *itr1;
  //  }
  //}
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
  if (compartmentsMap_.count(comp->getID()) != 0) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Compartment already exists in the DB: " + CString(comp->getID()));
    return;
  }
  compartmentsMap_[comp->getID()] = comp;
  assert (isValidCategory(comp->getCategory()));
  if (compsCategoryMap_.count(comp->getCategory()) == 0) {
    list<Compartment*> compList;
    compList.push_back(comp);
    compsCategoryMap_[comp->getCategory()] = compList;
  } else {
    compsCategoryMap_[comp->getCategory()].push_back(comp);
  }
}

list<Compartment*> ItemsDB::getCompartmentsByCategory( Compartment::Category category )
{
  assert (isValidCategory(category));
  return compsCategoryMap_[category];
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

Compartment* ItemsDB::getCompByID( int id )
{
  assert (compartmentsMap_.count(id) != 0);
  return compartmentsMap_[id];
}

