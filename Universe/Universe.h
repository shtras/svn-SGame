#pragma once

class Sector;
class SolarSystem;
class Star;
class Orbiting;

class Universe
{
public:
  static Universe& getInstance();
  void reset();
private:
  Universe();
  ~Universe();

  list<Sector*> sectors_;

  Sector* currSector_;
  SolarSystem* currSystem_;
  Orbiting* currObject_;
};

class Sector
{
public:
  Sector();
  ~Sector();
private:
  list<SolarSystem*> systems_;
};

class SolarSystem
{
public:
  SolarSystem();
  ~SolarSystem();
private:
  list<Star*> stars_;
  list<Orbiting*> objects_;
};

class Star
{
public:
  Star();
  ~Star();
private:
};

class Orbiting
{
public:
  Orbiting();
  virtual ~Orbiting();
protected:
  list <Orbiting*> satellites_;
};

class Planet: public Orbiting
{
public:
  Planet();
  ~Planet();
private:
};

class Station: public Orbiting
{
public:
  Station();
  ~Station();
private:
};

class JumpGate: public Orbiting
{
public:
  JumpGate();
  ~JumpGate();
private:
  Sector* targetSector_;
  SolarSystem* targetSystem_;
  JumpGate* targetJumpGate_;
};
