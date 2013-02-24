#include "StdAfx.h"
#include "Universe.h"


Universe& Universe::getInstance()
{
  static Universe instance;
  return instance;
}

Universe::Universe()
{

}

Universe::~Universe()
{
  for (Sector* sec: sectors_) {
    delete sec;
  }
}

void Universe::reset()
{
  for (Sector* sec: sectors_) {
    delete sec;
  }
  sectors_.clear();
}

Sector::Sector()
{

}

Sector::~Sector()
{
  for (SolarSystem* itr: systems_) {
    delete itr;
  }
}

SolarSystem::SolarSystem()
{

}

SolarSystem::~SolarSystem()
{
  for (Star* itr: stars_) {
    delete itr;
  }
  for (Orbiting* itr: objects_) {
    delete itr;
  }
}

Star::Star()
{

}

Star::~Star()
{

}

Orbiting::Orbiting()
{

}

Orbiting::~Orbiting()
{
  for (Orbiting* itr: satellites_) {
    delete itr;
  }
}

Planet::Planet()
{

}

Planet::~Planet()
{

}

Station::Station()
{

}

Station::~Station()
{

}

JumpGate::JumpGate()
{

}

JumpGate::~JumpGate()
{

}
