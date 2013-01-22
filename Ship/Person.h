#pragma once

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
    Civilian = 18
  };
  enum Type {SeniorOfficer, JuniorOfficer, Contractor, Soldier, Civillian};
  Person();
  ~Person();
private:
  CString name_;
  Rank rank_;
  Type type_;
  Stats stats_;
};
