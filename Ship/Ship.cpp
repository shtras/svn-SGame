#include "StdAfx.h"
#include "Ship.h"
#include "Renderer.h"
#include "ItemsDB.h"

Ship::Ship(int width, int height):minCrew_(0),maxCrew_(0),powerRequired_(0),powerProduced_(0),crewCapacity_(0),entrance_(NULL),width_(width), height_(height),numDecks_(3),
  left_(0), top_(0), actualWidth_(0), actualHeight_(0)
{
  for (int i=0; i<numDecks_; ++i) {
    Deck* deck = new Deck(this, width, height, i);
    decks_.push_back(deck);
  }
}

Ship::~Ship()
{
  for (auto itr = decks_.begin(); itr != decks_.end(); ++itr) {
    Deck* deck = *itr;
    delete deck;
  }
}

Deck* Ship::getDeck( int i )
{
  if (i < 0 || i >= (int)decks_.size()) {
    return NULL;
  }
  return decks_[i];
}

void Ship::updateParameters( int dMinCrew, int dMaxCrew, int dPowerProduced, int dPowerRequired, int dCrewCapacity )
{
  minCrew_ += dMinCrew;
  maxCrew_ += dMaxCrew;
  powerRequired_ += dPowerProduced;
  powerProduced_ += dPowerRequired;
  crewCapacity_ += dCrewCapacity;
}

void Ship::recalculateTiles()
{
  left_ = width_;
  top_ = height_;
  actualWidth_ = 0;
  actualHeight_ = 0;
  for (int i=0; i<(int)decks_.size(); ++i) {
    Deck* deck = decks_[i];
    for (int x=0; x<width_; ++x) {
      for (int y=0; y<height_; ++y) {
        Tile* tile = deck->getTile(x, y);
        assert(x == tile->getX() && y == tile->getY());
        tile->setConnected(false);
        tile->setAccessible(false);
        tile->setChecked(false);
        if (tile->getType() != Tile::Empty) {
          if (x < left_) {
            left_ = x;
          }
          if (y < top_) {
            top_ = y;
          }
          if (x+1 > actualWidth_) {
            actualWidth_ = x+1;
          }
          if (y+1 > actualHeight_) {
            actualHeight_ = y+1;
          }
        }
      }
    }
    for (auto itr = deck->getCompartments().begin(); itr != deck->getCompartments().end(); ++itr) {
      Compartment* comp = *itr;
      if (comp->getX() < left_) {
        left_ = comp->getX();
      }
      if (comp->getY() < top_) {
        top_ = comp->getY();
      }
      if (comp->getX() + comp->getWidth() > actualWidth_) {
        actualWidth_ = comp->getX() + comp->getWidth()+1;
      }
      if (comp->getY() + comp->getHeight() > actualHeight_) {
        actualHeight_ = comp->getY() + comp->getHeight()+1;
      }
    }
  }
  if (left_ == width_) {
    left_ = 0;
  }
  if (top_ == height_) {
    top_ = 0;
  }
  actualWidth_ -= left_;
  actualHeight_ -= top_;
  assert(actualWidth_ >= 0 && actualHeight_ >= 0);
  if (!entrance_) {
    return;
  }
  if (!isEntranceValid()) {
    entrance_->setEntrance(false);
    entrance_ = NULL;
    return;
  }
  recalculateTilesRec(entrance_->getDeckIdx(), entrance_->getX(), entrance_->getY(), true, true);
}

void Ship::recalculateTilesRec( int deckIdx, int x, int y, bool accessible, bool connected )
{
  if (!connected) {
    assert(!accessible);
    return;
  }
  Deck* deck = decks_[deckIdx];
  Tile* tile = deck->getTile(x, y);
  if (tile->isAccessible()) {
    assert (tile->isConnected());
    return;
  }
  if (tile->getType() == Tile::Empty) {
    return;
  }
  if (!accessible && tile->isConnected()) {
    return;
  }
  if (tile->getType() != Tile::Wall) {
    tile->setAccessible(accessible);
  } else {
    accessible = false;
  }
  tile->setConnected(true);
  Tile* left = deck->getTile(x-1, y);
  if (left) {
    recalculateTilesRec(deckIdx, x-1, y, accessible, true);
  }
  Tile* up = deck->getTile(x, y-1);
  if (up) {
    recalculateTilesRec(deckIdx, x, y-1, accessible, true);
  }
  Tile* right = deck->getTile(x+1, y);
  if (right) {
    recalculateTilesRec(deckIdx, x+1, y, accessible, true);
  }
  Tile* down = deck->getTile(x, y+1);
  if (down) {
    recalculateTilesRec(deckIdx, x, y+1, accessible, true);
  }
  if (deckIdx > 0 && tile->getType() == Tile::Stair) {
    Tile* levelDown = decks_[deckIdx-1]->getTile(x, y);
    assert (levelDown);
    if (levelDown->getType() == Tile::Stair) {
      recalculateTilesRec(deckIdx-1, x, y, accessible, true);
    }
  }
  if (deckIdx < numDecks_-1 && tile->getType() == Tile::Stair) {
    Tile* levelUp = decks_[deckIdx+1]->getTile(x, y);
    assert (levelUp);
    if (levelUp->getType() == Tile::Stair) {
      recalculateTilesRec(deckIdx+1, x, y, accessible, true);
    }
  }
}

bool Ship::isEntranceValid()
{
  assert(entrance_);
  return outAccessible(entrance_);
}

bool Ship::outAccessible(Tile* tile)
{
  if (tile->isChecked()) {
    return false;
  }
  if (tile != entrance_ && tile->getType() != Tile::Empty) {
    return false;
  }
  tile->setChecked(true);
  int x = tile->getX();
  int y = tile->getY();
  Deck* deck = decks_[tile->getDeckIdx()];
  bool res = false;
  Tile* left = deck->getTile(x-1, y);
  if (!left) {
    return true;
  }
  res |= outAccessible(left);
  Tile* up = deck->getTile(x, y-1);
  if (!up) {
    return true;
  }
  res |= outAccessible(up);
  Tile* right = deck->getTile(x+1, y);
  if (!right) {
    return true;
  }
  res |= outAccessible(right);
  Tile* down = deck->getTile(x, y+1);
  if (!down) {
    return true;
  }
  res |= outAccessible(down);
  return res;
}

void Ship::normalize()
{
  recalculateTiles();
  int dx = -left_;
  int dy = -top_;
  shiftContents(dx, dy);
}

void Ship::shiftContents( int dx, int dy )
{
  for (int i=0; i<(int)decks_.size(); ++i) {
    Deck* d = decks_[i];
    d->shiftContents(dx, dy);
  }
  recalculateTiles();
}

bool Ship::load(CString fileName)
{
  for (auto itr = decks_.begin(); itr != decks_.end(); ++itr) {
    Deck* deck = *itr;
    delete deck;
  }
  decks_.clear();
  left_ = 0;
  top_ = 0;
  FILE* file = fopen(fileName, "rb");
  char buffer[1024];
  readFromFile(buffer, 1, 5, file);
  if (buffer[0] != 'S' || buffer[1] != 'G' || buffer[2] != 'S' || buffer[3] != 'F') {
    Logger::getInstance().log(ERROR_LOG_NAME, "Save file corrupted");
    fclose(file);
    return false;
  }

  int fileHash;
  readFromFile(&fileHash, sizeof(int), 1, file);
  if (fileHash != ItemsDB::getInstance().getFileHash()) {
    Logger::getInstance().log(ERROR_LOG_NAME, "rooms.txt is not compatible with this version of save file");
    fclose(file);
    return false;
  }

  readFromFile(buffer, 1, 5, file);
  int width = buffer[0];
  int height = buffer[1];
  int offsetX = buffer[2];
  int offsetY = buffer[3];
  if (width > width_ || height > height_) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Save file corrupted");
    fclose(file);
    return false;
  }
  numDecks_ = buffer[4];
  if (numDecks_ < 1 || numDecks_ > 20) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Save file corrupted");
    fclose(file);
    return false;
  }
  int* deckOffsets = new int[numDecks_];
  readFromFile(deckOffsets, sizeof(int), numDecks_, file);
  map<int, Compartment*> compIDs;
  map<Compartment*, list<int> > compConnections;
  for (int i=0; i<numDecks_; ++i) {
    Deck* deck = new Deck(this, width_, height_, i);
    decks_.push_back(deck);
    fseek(file, deckOffsets[i], SEEK_SET);
    int compartmentsOffset = 0;
    readFromFile(&compartmentsOffset, sizeof(int), 1, file);
    while (ftell(file) < compartmentsOffset) {
      //reading tiles
      readFromFile(buffer, 1, 3, file);
      int x = buffer[0] + offsetX;
      int y = buffer[1] + offsetY;
      assert (x >=0 && x <= width_ && y >= 0 && y <= height_);
      bool entrance = (buffer[2] & 0x80)==0?false:true;
      Tile::TileType type = (Tile::TileType)(buffer[2] & 0x7F);
      Tile* tile = deck->getTile(x, y);
      tile->setType(type);
      tile->setEntrance(entrance);
      if (entrance) {
        entrance_ = tile;
      }
    }
    //Here should be compartments
    if (ftell(file) != compartmentsOffset) {
      delete[] deckOffsets;
      Logger::getInstance().log(ERROR_LOG_NAME, "Save file corrupted");
      fclose(file);
      return false;
    }
    char numCompartments = 0;
    readFromFile(&numCompartments, 1, 1, file);
    for (int i=0; i<numCompartments; ++i) {
      Compartment* comp = new Compartment();
      CString name = readStringFromFile(file);
      comp->setName(name);
      if (!readFromFile(buffer, 1, 13, file)) {
        delete[] deckOffsets;
        Logger::getInstance().log(ERROR_LOG_NAME, "Save file corrupted");
        fclose(file);
        return false;
      }
      comp->setX(buffer[1] + offsetX);
      comp->setY(buffer[2] + offsetY);
      comp->setWidth(buffer[3]);
      comp->setHeight(buffer[4]);
      comp->setRotation(buffer[5]);
      comp->setMinCrew(buffer[6]);
      comp->setMaxCrew(buffer[7]);
      comp->setPowerProduced(buffer[8]);
      comp->setPowerRequired(buffer[9]);
      comp->setCrewCapacity(buffer[10]);
      comp->setCategory((Compartment::Category)buffer[11]);
      int id = buffer[0];
      assert (compIDs.count(id) == 0);
      compIDs[id] = comp;
      list<int> connections;
      for (int connection = 0; connection<buffer[12]; ++connection) {
        char c;
        readFromFile(&c, 1, 1, file);
        connections.push_back(c);
      }
      compConnections[comp] = connections;
      char numRequirements = 0;
      readFromFile(&numRequirements, 1, 1, file);
      set<CString> requirements;
      for (int i=0; i<numRequirements; ++i) {
        CString requirement = readStringFromFile(file);
        requirements.insert(requirement);
      }
      comp->setRequiredConnections(requirements);
      char numItems = 0;
      readFromFile(&numItems, 1, 1, file);
      for (int itemItr = 0; itemItr<numItems; ++itemItr) {
        readFromFile(buffer, 1, 4, file);
        Item* itemOriginal = ItemsDB::getInstance().getItemByID(buffer[0]);
        Item* item = new Item(*itemOriginal);
        item->setX(buffer[1]);
        item->setY(buffer[2]);
        item->setRotation(buffer[3]);
        comp->addItem(item);
      }
      deck->addCompartment(comp);
    }
  }
  delete[] deckOffsets;
  fclose(file);

  //Finalize connections
  for (int i=0; i<numDecks_; ++i) {
    Deck* d = decks_[i];
    for (auto itr = d->getCompartments().begin(); itr != d->getCompartments().end(); ++itr) {
      Compartment* comp = *itr;
      list<int>& connections = compConnections[comp];
      for (auto conItr = connections.begin(); conItr != connections.end(); ++conItr) {
        int connection = *conItr;
        if (compIDs.count(connection) != 1) {
          return false;
        }
        Compartment* otherComp = compIDs[connection];
        comp->connectTo(otherComp);
      }
    }
  }

  recalculateTiles();
  return true;
}

void Ship::save( CString fileName )
{
  //normalize();
  FILE* file = fopen(fileName, "wb");
  assert(file);
  char* magic = "SGSF";
  fputs(magic, file);

  char buffer[1024];
  buffer[0] = 1; //version
  writeToFile(buffer, 1, 1, file);
  int res = 0;

  int fileHash = ItemsDB::getInstance().getFileHash();
  writeToFile(&fileHash, sizeof(int), 1, file);

  buffer[0] = actualWidth_;
  buffer[1] = actualHeight_;
  buffer[2] = left_;
  buffer[3] = top_;
  buffer[4] = numDecks_;
  writeToFile(buffer, 1, 5, file); // write width, height, numdecks
  int deckOffsets = ftell(file);
  writeToFile(buffer, 4, numDecks_, file); //reserve space for decks offsets
  
  map<Compartment*, int> compIDs_;
  int lastCompID = 0;
  for (int i=0; i<numDecks_; ++i) {
    Deck* d = decks_[i];
    for (auto itr = d->getCompartments().begin(); itr != d->getCompartments().end(); ++itr) {
      Compartment* comp = *itr;
      assert (compIDs_.count(comp) == 0);
      compIDs_[comp] = ++lastCompID;
    }
  }
  for (int i=0; i<numDecks_; ++i) {
    int deckBeginPosition = ftell(file);
    res = fseek(file, deckOffsets+i*sizeof(int), SEEK_SET);
    assert (res == 0);
    fwrite(&deckBeginPosition, sizeof(int), 1, file);
    res = fseek(file, deckBeginPosition, SEEK_SET);
    assert (res == 0);
    Deck* deck = decks_[i];
    writeToFile(buffer, sizeof(int), 1, file); //reserve space for components offset
    for (int x=0; x<width_; ++x) {
      for (int y=0; y<height_; ++y) {
        Tile* tile = deck->getTile(x, y);
        if (tile->getType() == Tile::Empty) {
          continue;
        }
        buffer[0] = tile->getX() - left_;
        buffer[1] = tile->getY() - top_;
        buffer[2] = (tile->isEntrance())?0x80:0;
        buffer[2] |= tile->getType();
        writeToFile(buffer, 1, 3, file);
      }
    }
    //finished writing tiles. Writing components
    int compartmentsBeginPosition = ftell(file);
    res = fseek(file, deckBeginPosition, SEEK_SET);
    assert (res == 0);
    writeToFile(&compartmentsBeginPosition, sizeof(int), 1, file);
    res = fseek(file, compartmentsBeginPosition, SEEK_SET);
    assert (res == 0);
    buffer[0] = deck->getCompartments().size();
    writeToFile(buffer, 1, 1, file); //write num compartments
    for (auto itr = deck->getCompartments().begin(); itr != deck->getCompartments().end(); ++itr) {
      Compartment* comp = *itr;
      writeStringToFile(file, comp->getName());
      assert (compIDs_.count(comp) == 1);
      int cnt=0;
      buffer[cnt++] = compIDs_[comp];
      buffer[cnt++] = comp->getX() - left_;
      buffer[cnt++] = comp->getY() - top_;
      buffer[cnt++] = comp->getWidth();
      buffer[cnt++] = comp->getHeight();
      buffer[cnt++] = comp->getRotation();
      buffer[cnt++] = comp->getMinCrew();
      buffer[cnt++] = comp->getMaxCrew();
      buffer[cnt++] = comp->getPowerProduced();
      buffer[cnt++] = comp->getPowerRequired();
      buffer[cnt++] = comp->getCrewCapacity();
      buffer[cnt++] = comp->getCategory();
      buffer[cnt++] = comp->getConnections().size();
      writeToFile(buffer, 1, cnt, file);
      cnt=0;
      for (auto itr = comp->getConnections().begin(); itr != comp->getConnections().end(); ++itr) {
        Compartment* otherComp = *itr;
        assert (compIDs_.count(otherComp) == 1);
        buffer[cnt++] = compIDs_[otherComp];
      }
      if (cnt > 0) {
        writeToFile(buffer, 1, cnt, file);
      }
      char numRequirements = comp->getRequiredConnections().size();
      writeToFile(&numRequirements, 1, 1, file);
      for (auto requirementItr = comp->getRequiredConnections().begin(); requirementItr != comp->getRequiredConnections().end(); ++requirementItr) {
        CString requirement = *requirementItr;
        writeStringToFile(file, requirement);
      }
      buffer[0] = comp->getItems().size();
      writeToFile(buffer, 1, 1, file);
      for (auto itr = comp->getItems().begin(); itr != comp->getItems().end(); ++itr) {
        Item* item = *itr;
        int cnt = 0;
        buffer[cnt++] = item->getID();
        buffer[cnt++] = item->getX();
        buffer[cnt++] = item->getY();
        buffer[cnt++] = item->getRotation();
        writeToFile(buffer, 1, cnt, file);
      }
    }
  }
  fclose(file);
}

Deck::Deck(Ship* ship, int width, int height,int idx):
  ship_(ship), width_(width), height_(height), idx_(idx)
{
  tileLayout_ = new Tile*[width_ * height_];
  for (int i=0; i<width_*height_; ++i) {
    tileLayout_[i] = new Tile(i%width_, i/height_, idx_);
  }
}

Deck::~Deck()
{
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    Compartment* comp = *itr;
    delete comp;
  }
  for (int i=0; i<width_*height_; ++i) {
    delete tileLayout_[i];
  }
  delete[] tileLayout_;
}

void Deck::addCompartment(Compartment* comp)
{
  compartments_.push_back(comp);
  ship_->updateParameters(comp->getMinCrew(), comp->getMaxCrew(), comp->getPowerRequired(), comp->getPowerProduced(), comp->getCrewCapacity());
}

void Deck::removeCompartment( Compartment* comp )
{
  compartments_.remove(comp);
  ship_->updateParameters(-comp->getMinCrew(), -comp->getMaxCrew(), -comp->getPowerRequired(), -comp->getPowerProduced(), -comp->getCrewCapacity());
  for (auto itr = comp->getConnections().begin(); itr != comp->getConnections().end(); ++itr) {
    Compartment* other = *itr;
    other->disconnectFrom(comp);
  }
  delete comp;
}

Tile* Deck::getTile(int x, int y)
{
  if (x < 0 || y < 0 || x >= width_ || y >= height_) {
    return NULL;
  }
  return tileLayout_[y*width_ + x];
}

void Deck::setTileType(int x, int y, Tile::TileType value)
{
  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
    assert(0);
  }
  if (value == Tile::Wall || value == Tile::Floor || value == Tile::Door) {
    if (hasDoorsAround(x, y) && value != Tile::Floor) {
      //Can't create doors or walls around a door
      return;
    }
    if (getTileType(x, y) == Tile::Door) {
      //Can only erase door, not replace
      return;
    }
  }
  if ((value == Tile::Wall || value == Tile::Door) && getCompartment(x,y)) {
    //Can't build over compartment
    return;
  }
  Tile* tile = tileLayout_[y*width_ + x];
  if (value != Tile::Door) {
    tile->setEntrance(false);
  }
  if (value == Tile::Empty && tile->getType() == Tile::Door) {
    tile->setType(Tile::Wall);
  } else {
    tile->setType(value);
  }
}

Tile::TileType Deck::getTileType(int x, int y)
{
  if (x < 0 || y < 0 || x >= width_ || y >= height_) {
    return Tile::Empty;
  }
  Tile* tile = tileLayout_[y*width_ + x];
  if (!tile) {
    return Tile::Empty;
  }
  return tile->getType();
}

int Deck::getWallCode( int i, int j )
{
  int wallCode = 0;
  wallCode |= hasWall(i, j+1);
  wallCode |= (hasWall(i+1, j) << 1);
  wallCode |= (hasWall(i, j-1) << 2);
  wallCode |= (hasWall(i-1, j) << 3);
  assert (wallCode >= 0 && wallCode < 16);
  return wallCode;
}

void Deck::setDoor( int x, int y )
{
  Tile::TileType type = getTileType(x, y);
  if (type != Tile::Wall) {
    return;
  }
  int wallCode = getWallCode(x, y);
  if (wallCode != 10 && wallCode != 5) {
    //Not a horizontal or vertical straight wall
    return;
  }
  setTileType(x, y, Tile::Door);
}

void Deck::setStair( int x, int y )
{
  Tile::TileType type = getTileType(x, y);
  if (type != Tile::Floor) {
    return;
  }
  if (getCompartment(x, y)) {
    return;
  }
  setTileType(x, y, Tile::Stair);
}

bool Deck::hasDoorsAround( int x, int y )
{
  return getTileType(x-1,y) == Tile::Door || getTileType(x+1, y) == Tile::Door || getTileType(x, y-1) == Tile::Door || getTileType(x, y+1) == Tile::Door;
}

void Deck::eraseDoorsAround( int x, int y )
{
  if (getTileType(x-1,y) == Tile::Door) {
    setTileType(x-1, y, Tile::Empty);
  }
  if (getTileType(x+1,y) == Tile::Door) {
    setTileType(x+1, y, Tile::Empty);
  }
  if (getTileType(x,y-1) == Tile::Door) {
    setTileType(x, y-1, Tile::Empty);
  }
  if (getTileType(x,y+1) == Tile::Door) {
    setTileType(x, y+1, Tile::Empty);
  }
}

Compartment* Deck::getCompartment( int x, int y )
{
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    Compartment* comp = *itr;
    if (x >= comp->getX() && x < comp->getX() + comp->getWidth() && y >= comp->getY() && y < comp->getY() + comp->getHeight()) {
      return comp;
    }
  }
  return NULL;
}

int Deck::hasWall( int x, int y )
{
  if (x < 0 || x >= width_ || y < 0 || y >= height_) {
    return 0;
  }
  return (getTileType(x, y)==Tile::Wall || getTileType(x, y)==Tile::Door)?1:0;
}

void Deck::shiftContents( int dx, int dy )
{
  int startX = 0;
  int startY = 0;
  int endX = width_;
  int endY = height_;
  int stepX = 1;
  int stepY = 1;
  if (dx > 0) {
    startX = width_-1;
    stepX = -1;
    endX = 0;
  }
  if (dy > 0) {
    startY = height_-1;
    stepY = -1;
    endY = 0;
  }
  for (int i=startX; i!=endX; i+=stepX) {
    for (int j=startY; j!=endY; j+=stepY) {
      Tile* tile = getTile(i, j);
      assert(tile);
      Tile* fromTile = getTile(i-dx, j-dy);
      if (fromTile) {
        tile->setType(fromTile->getType());
        tile->setEntrance(fromTile->isEntrance());
        if (tile->isEntrance()) {
          ship_->setEntrance(tile);
        }
      } else {
        tile->setType(Tile::Empty);
        tile->setEntrance(false);
      }
    }
  }
  for (auto itr = compartments_.begin(); itr != compartments_.end(); ++itr) {
    Compartment* comp = *itr;
    comp->setX(comp->getX() + dx);
    comp->setY(comp->getY() + dy);
  }
}

Compartment::Compartment():rotation_(0)
{
}

Compartment::Compartment(const Compartment& other):left_(other.left_), top_(other.top_), width_(other.width_), height_(other.height_), name_(other.name_),
  category_(other.category_),minCrew_(other.minCrew_),maxCrew_(other.maxCrew_),powerRequired_(other.powerRequired_),powerProduced_(other.powerProduced_),
  crewCapacity_(other.crewCapacity_),/*Maybe this one is not needed -->*/rotation_(other.rotation_)
{
  for (auto itr = other.items_.begin(); itr != other.items_.end(); ++itr) {
    items_.push_back(new Item(**itr));
  }
  for (auto itr = other.requiredConnections_.begin(); itr != other.requiredConnections_.end(); ++itr) {
    requiredConnections_.insert(*itr);
  }
}

Compartment::~Compartment()
{
  for (auto itr = items_.begin(); itr != items_.end(); ++itr) {
    delete *itr;
  }
}

CString Compartment::categoryName(Category cat)
{
  switch (cat) {
  case Common:
    return "Common";
  case Navigation:
    return "Navigation";
  case Power:
    return "Power";
  case LifeSupport:
    return "Life support";
  case Living:
    return "Living";
  case LastCategory:
    assert(0);
    Logger::getInstance().log(ERROR_LOG_NAME, "Attempted to get last category name");
    break;
  default:
    assert(0);
    Logger::getInstance().log(ERROR_LOG_NAME, "Unknown category");
    break;
  }
  return "";
}

void Compartment::connectTo( Compartment* comp )
{
  assert (connections_.count(comp) == 0);
  connections_.insert(comp);
}

void Compartment::disconnectFrom( Compartment* comp )
{
  assert (connections_.count(comp) == 1);
  connections_.erase(comp);
}

bool Compartment::isConnectedTo( Compartment* comp )
{
  return connections_.count(comp) == 1;
}

bool Compartment::isConnectedTo( CString compName )
{
  for (auto itr = connections_.begin(); itr != connections_.end(); ++itr) {
    Compartment* comp = *itr;
    if (comp->getName() == compName) {
      return true;
    }
  }
  return false;
}

bool Compartment::requiredConnection( CString compName )
{
  return requiredConnections_.count(compName) != 0 && !isConnectedTo(compName);
}

Item::Item()
{
}

Item::Item(const Item& other):x_(other.x_), y_(other.y_),name_(other.name_),texX_(other.texX_),texY_(other.texY_), texWidth_(other.texWidth_),texHeight_(other.texHeight_),
  rotation_(other.rotation_),id_(other.id_)
{
}

Item::~Item()
{
}

void Item::setTexX(int x)
{
  texX_ = x / (float)Renderer::getInstance().getTilesTexWidth();
}

void Item::setTexY(int y)
{
  texY_ = y / (float)Renderer::getInstance().getTilesTexHeight();
}

void Item::setTexWidth(int width)
{
  texWidth_ = width / (float)Renderer::getInstance().getTilesTexWidth();
}

void Item::setTexHeight(int height)
{
  texHeight_ = height / (float)Renderer::getInstance().getTilesTexHeight();
}

Tile::Tile(int x, int y, int deck):type_(Empty), x_(x), y_(y), accessible_(false), connected_(false), checked_(false), deckIdx_(deck), entrance_(false)
{
}

Tile::~Tile()
{
}
