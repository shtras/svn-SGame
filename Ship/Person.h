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
  enum Rank {SeniorOfficer, JuniorOfficer, Sergeant, Soldier, Civillian};
  Person();
  ~Person();
private:
  CString name_;
  Rank rank_;
  Stats stats_;
};
