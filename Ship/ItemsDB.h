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
  void setFileHash(int hash) {hash_ = hash;}
  int getFileHash() {return hash_;}
private:
  ItemsDB();
  ~ItemsDB();

  map<int, Item*> itemsMap_;
  map<Compartment::Category, list<Compartment*> > roomsMap_;
  int hash_;
};
