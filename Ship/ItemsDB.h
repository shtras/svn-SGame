#pragma once
#include "Ship.h"

class ItemsDB
{
public:
  static ItemsDB& getInstance();
  bool init();
  bool isValidCategory(int category);
  void addItem(Item* item);
  void addCompartment(Compartment* comp);
  list<Compartment*> getCompartmentsByCategory(Compartment::Category category);
  Item* getItemByID(int id);
private:
  ItemsDB();
  ~ItemsDB();

  map<int, Item*> itemsMap_;
  map<Compartment::Category, list<Compartment*> > roomsMap_;
};
