#pragma once
#include "Ship.h"

class Section;

class RoomParser
{
public:
  RoomParser();
  ~RoomParser();
  bool parse(CString fileName);
  bool parseCSV();
private:
  CString getNextLine();
  void pushBackLine(CString line);

  Section* parseSection();
  bool processData();
  bool processItem(Section* sec);
  bool processCompartment(Section* sec);
  map<int, Item*> itemsMap_;
  map<int, list<Compartment*> > roomsMap_;
  fstream file;
  Section* root_;
  CString pushedLine_;
  bool linePushed_;
  int hash_;
};

struct Property
{
  CString propName;
  CString value;
};

class Section
{
public:
  Section();
  ~Section();
  void addSubSection(Section* section);
  void addProperty(Property& prop);
  void setName(CString name) {name_ = name;}
  CString getName() {return name_;}
  const list<Property>& getProperties() {return properties_;}
  const list<Section*>& getSubSections() {return subSections_;}
private:
  list<Property> properties_;
  list<Section*> subSections_;
  CString name_;
};
