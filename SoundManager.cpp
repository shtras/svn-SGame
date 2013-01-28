#include "StdAfx.h"
#include "SoundManager.h"

SoundManager& SoundManager::getInstance()
{
  static SoundManager instance;
  return instance;
}

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
  for (auto i = buffers_.begin(); i != buffers_.end(); i++) {
    alDeleteBuffers(1, &i->second.ID);
  }
  alcMakeContextCurrent(NULL);
  alcDestroyContext(context_);
  alcCloseDevice(device_);
}

bool SoundManager::init()
{
  ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
  ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
  ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };
  device_ = alcOpenDevice(NULL);
  if (!device_) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Default sound device not found");
    return false;
  }
  context_ = alcCreateContext(device_, NULL);

  alcMakeContextCurrent(context_);
  alListenerfv(AL_POSITION,    ListenerPos);
  alListenerfv(AL_VELOCITY,    ListenerVel);
  alListenerfv(AL_ORIENTATION, ListenerOri);
  return true;
}

Snd::Snd()
{

}

Snd::~Snd()
{

}

bool Snd::open( CString fileName, bool looped, bool streamed )
{
  return true;
}
