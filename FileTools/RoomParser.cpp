#include "StdAfx.h"
#include "RoomParser.h"
#include "ItemsDB.h"

RoomParser::RoomParser():linePushed_(false),hash_(0)
{
}

RoomParser::~RoomParser()
{
}

bool RoomParser::parse(CString fileName)
{
  hash_ = 0;
  file.open(fileName, fstream::in);
  if (file.fail()) {
    return false;
  }

  root_ = parseSection();
  file.close();

  if (!root_) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Failed to parse " + fileName);
    return false;
  }
  bool res = processData();
  if (!res) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Failed to process " + fileName);
    return false;
  }
  ItemsDB::getInstance().setFileHash(hash_);
  delete root_;
  return true;
}

Section* RoomParser::parseSection()
{
  CString line;
  Section* res = new Section();
  CString caption = getNextLine();
  assert(caption.contains('{'));
  res->setName(caption.tokenize('{')[0].toLower());
  
  while (1) {
    CString line = getNextLine();
    if (line.isEmpty()) {
      Logger::getInstance().log(ERROR_LOG_NAME, "Failed to parse section");
      delete res;
      return NULL;
    }
    if (line == "}") {
      break;
    }
    if (line.contains('{')) {
      pushBackLine(line);
      Section* subSec = parseSection();
      if (!subSec) {
        delete res;
        return NULL;
      }
      res->addSubSection(subSec);
      continue;
    }
    if (!line.contains('=')) {
      Logger::getInstance().log(ERROR_LOG_NAME, "Line: \"" + line + "\" does not define property/value");
      delete res;
      return NULL;
    }
    vector<CString> parts = line.tokenize('=');
    if (parts.size() != 2) {
      Logger::getInstance().log(ERROR_LOG_NAME, "Line: \"" + line + "\" has more than 2 parts");
      delete res;
      return NULL;
    }
    Property prop;
    prop.propName = parts[0].toLower();
    prop.value = parts[1];
    res->addProperty(prop);
  }

  return res;
}

CString RoomParser::getNextLine()
{
  if (linePushed_) {
    linePushed_= false;
    return pushedLine_;
  }
  string inStr;
  CString res = "";
  while(!file.eof()) {
    getline(file, inStr);
    CString line(inStr.c_str());
    //line = line.toLower();
    line.replace('\t', ' ');
    line = line.remove(' ');
    if (line.contains(';')) {
      line = line.tokenize(';')[0];
    }
    if (line.isEmpty()) {
      continue;
    }
    res = line;
    break;
  }
  hash_ = hash_ ^ ((65599 * res.hashCode()) >> 16);
  return res;
}

void RoomParser::pushBackLine(CString line)
{
  linePushed_ = true;
  pushedLine_ = line;
}

bool RoomParser::processData()
{
  assert(root_);
  if (root_->getName() != "compartmentinfo") {
    return false;
  }
  for (auto itr = root_->getSubSections().begin(); itr != root_->getSubSections().end(); ++itr) {
    Section* sec = *itr;
    assert(sec);
    if (sec->getName() == "item") {
      if (!processItem(sec)) {
        return false;
      }
    } else if (sec->getName() == "compartment") {
      if (!processCompartment(sec)) {
        return false;
      }
    } else {
      Logger::getInstance().log(ERROR_LOG_NAME, "Bad section name: " + sec->getName());
      return false;
    }
  }
  return true;
}

bool RoomParser::processItem( Section* sec )
{
  if (sec->getSubSections().size() != 0) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Item section can not have subsections");
    return false;
  }
  int id = -1;
  CString name = "";
  int texX = -1;
  int texY = -1;
  bool autorotate = false;
  for (auto itr = sec->getProperties().begin(); itr != sec->getProperties().end(); ++itr) {
    const Property& prop = *itr;
    if (prop.propName == "id") {
      id = parseInt(prop.value);
    } else if (prop.propName == "name") {
      name = prop.value;
    } else if (prop.propName == "x") {
      texX = parseInt(prop.value);
    } else if (prop.propName == "y") {
      texY = parseInt(prop.value);
    } else if (prop.propName == "autorotate") {
      if (prop.value == "true") {
        autorotate = true;
      }
    } else {
      Logger::getInstance().log(ERROR_LOG_NAME, "Invalid property: " + prop.propName);
      return false;
    }
  }
  if (id == -1 || name == "" || texX == -1 || texY == -1) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Missing property");
    return false;
  }
  if (itemsMap_.count(id) != 0) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Duplicating ID: " + CString(id));
    return false;
  }
  Item* item = new Item();
  item->id_ = id;
  item->name_ = name;
  item->setTexX(texX);
  item->setTexY(texY);
  item->setTexWidth(64);
  item->setTexHeight(64);
  item->autorotate_ = autorotate;
  itemsMap_[id] = item;
  ItemsDB::getInstance().addItem(item);
  return true;
}

bool RoomParser::processCompartment( Section* sec )
{
  //Assume that all the items are already processed
  int category = -1;
  CString name = "";
  int width = -1;
  int height = -1;
  int minCrew = 0;
  int maxCrew = 0;
  int powerRequired = 0;
  int powerProduced = 0;
  int crewCapacity = 0;
  int maxConnections = 0;
  int maxSameConections = 1;
  bool requiresAccess = true;
  set<CString> requiredConnections;
  for (auto itr = sec->getProperties().begin(); itr != sec->getProperties().end(); ++itr) {
    const Property& prop = *itr;
    if (prop.propName == "category") {
      category = parseInt(prop.value);
    } else if (prop.propName == "name") {
      name = prop.value;
    } else if (prop.propName == "width") {
      width = parseInt(prop.value);
    } else if (prop.propName == "height") {
      height = parseInt(prop.value);
    } else if (prop.propName == "mincrew") {
      minCrew = parseInt(prop.value);
    } else if (prop.propName == "maxcrew") {
      maxCrew = parseInt(prop.value);
    } else if (prop.propName == "powerrequired") {
      powerRequired = parseInt(prop.value);
    } else if (prop.propName == "maxconnections") {
      maxConnections = parseInt(prop.value);
    } else if (prop.propName == "maxsameconnections") {
      maxSameConections = parseInt(prop.value);
    } else if (prop.propName == "requiresaccess") {
      if (prop.value == "false") {
        requiresAccess = false;
      }
    } else if (prop.propName == "required") {
      CString value = prop.value;
      vector<CString> values = value.tokenize(',');
      for (int i=0; i<(int)values.size(); ++i) {
        requiredConnections.insert(values[i]);
      }
    } else if (prop.propName == "powerproduced") {
      powerProduced = parseInt(prop.value);
    } else if (prop.propName == "crewcapacity") {
      crewCapacity = parseInt(prop.value);
    } else {
      Logger::getInstance().log(ERROR_LOG_NAME, "Invalid property: " + prop.propName);
      return false;
    }
  }
  if (category == -1 || name == "" || width == -1 || height == -1) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Missing property");
    return false;
  }
  Compartment* comp = new Compartment();
  comp->name_ = name;
  comp->width_ = width;
  comp->height_ = height;
  comp->category_ = (Compartment::Category)category;
  comp->minCrew_ = minCrew;
  comp->maxCrew_ = maxCrew;
  comp->powerRequired_ = powerRequired;
  comp->powerProduced_ = powerProduced;
  comp->crewCapacity_ = crewCapacity;
  comp->requiredConnections_ = requiredConnections;
  comp->maxConnections_ = maxConnections;
  comp->maxSameConnections_ = maxSameConections;
  comp->requiresAccess_ = requiresAccess;
  for (auto itr = sec->getSubSections().begin(); itr != sec->getSubSections().end(); ++itr) {
    Section* subSec = *itr;
    assert(subSec);
    if (subSec->getSubSections().size() != 0) {
      Logger::getInstance().log(ERROR_LOG_NAME, "Item reference in compartment section can not have subsections");
      delete comp;
      return false;
    }
    int itemID = -1;
    int itemX = -1;
    int itemY = -1;
    int itemRot = -1;
    bool requiresVacuum = false;
    for (auto propItr = subSec->getProperties().begin(); propItr != subSec->getProperties().end(); ++propItr) {
      const Property& prop = *propItr;
      if (prop.propName == "id") {
        itemID = parseInt(prop.value);
      } else if (prop.propName == "x") {
        itemX = parseInt(prop.value);
      } else if (prop.propName == "y") {
        itemY = parseInt(prop.value);
      } else if (prop.propName == "rotation") {
        itemRot = parseInt(prop.value);
      } else if (prop.propName == "requiresvacuum") {
        if (prop.value == "true") {
          requiresVacuum = true;
        }
      } else {
        Logger::getInstance().log(ERROR_LOG_NAME, "Invalid property: " + prop.propName);
        delete comp;
        return false;
      }
    }
    if (itemID == -1 || itemX == -1 || itemY == -1 || itemRot == -1) {
      Logger::getInstance().log(ERROR_LOG_NAME, "Missing property");
      delete comp;
      return false;
    }
    if (itemsMap_.count(itemID) == 0) {
      Logger::getInstance().log(ERROR_LOG_NAME, "Item not found: " + CString(itemID));
      delete comp;
      return false;
    }
    Item* item = itemsMap_[itemID];
    Item* newItem = new Item(*item);
    newItem->x_ = itemX;
    newItem->y_ = itemY;
    newItem->rotation_ = itemRot;
    newItem->requiresVacuum_ = requiresVacuum;
    comp->addItem(newItem);
  }
  if (roomsMap_.count(category) == 0) {
    list<Compartment*> newList;
    newList.push_back(comp);
    roomsMap_[category] = newList;
  } else {
    roomsMap_[category].push_back(comp);
  }
  ItemsDB::getInstance().addCompartment(comp);
  return true;
}

Section::Section()
{
}

Section::~Section()
{
  for (auto itr = subSections_.begin(); itr != subSections_.end(); ++itr) {
    delete *itr;
  }
}

void Section::addSubSection(Section* section)
{
  subSections_.push_back(section);
}

void Section::addProperty(Property& prop)
{
  properties_.push_back(prop);
}
