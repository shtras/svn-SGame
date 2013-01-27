#pragma once

class Ship;
class Compartment;
class CrewMemberIcon;

struct Stats
{
  int Health;
  int Stamina;
  int Fatigue;
  int Hunger;
  int Motivation;
};

class Person
{
public:
  enum Rank {
    Private = 0, //Turai
    Corporal = 1, //Rabat
    Sergeant = 2, //Samal
    StaffSergeant = 3, //Samar
    LastSoldierRank = 3,
    SergeantFirstClass = 4, //Rasal
    FirstSergeant = 5, //Rasar
    SergeantMajor = 6, //Rasam
    WarrantOfficer = 7, //Rasab
    SeniorWarrantOfficer = 8, //Ranag
    LastContractorRank = 8,
    SecondLieutenant = 9, //Sagam
    FirstLieutenant = 10, //Segen
    Captain = 11, //Seren
    LastJuniorOfficer = 11,
    Major = 12, //Rasan
    LieutenantColonel = 13, //Saal
    Colonel = 14, //Alam
    BrigadierGeneral = 15, //Taal
    MajorGeneral = 16, //Aluf
    LieutenantGeneral = 17, //Raal
    LastOfficer = 17,
    Other = 18
  };
  enum Type {SeniorOfficer, JuniorOfficer, Contractor, Soldier, Civilian};
  enum Action {Idle, Walking, Working, Sleeping, Eating, Relaxing};
  Person();
  ~Person();
  bool init(Ship* ship, Position pos);
  const Position& getPosition() const {return pos_;}
  void step();
  Direction getDirection() {return dir_;}
  float getPathSteProgress() {return pathStepProgress_;}
  float getHeadTexX() {return headTexX_;}
  //void setWatchCompartment(int watchNum, Compartment* comp);
  //Compartment* getWatchCompartment(int watchNum);
  void setWatchIcon(int watchNum, CrewMemberIcon* icon);
  CrewMemberIcon* getWatchIcon(int watchNum);
  void setHighLighted(bool value) {highlighted_ = value;}
  bool isHighLighted() {return highlighted_;}
  CString getName() {return name_;}
  Compartment* getQuarterComp() {return quarterComp_;}
protected:
  void pathStep();
  void decideNextAction();
  Position movePosition(Position pos, Direction dir);
  CString name_;
  Rank rank_;
  Type type_;
  list<Action> actions_;
  int idleCnt_;
  Stats stats_;
  Position pos_;
  Position quarter_;
  Compartment* quarterComp_;
  Position watchPos_[3];
  //Compartment* watchComp_[3];
  CrewMemberIcon* watchIcon_[3];
  list<Direction> path_;
  float pathStepProgress_;
  Ship* ship_;
  Direction dir_;
  float headTexX_;
  float speed_;
  bool highlighted_;
};

