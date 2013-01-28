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
  for (auto i = sounds_.begin(); i != sounds_.end(); i++) {
    Snd* snd = (*i).second;
    snd->close();
    delete snd;
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

Snd::Snd():mVF_(NULL),srcID_(-1),buferID_(-1)
{
  for (int i = 0; i < 3; i++) {
    pos_[i] = pos_[i] = 0.0f;
  }
}

Snd::~Snd()
{

}

bool Snd::open( CString fileName, bool looped, bool streamed )
{
  // Check file existance
  ifstream a(fileName);
  if (!a.is_open()) {
    return false;
  }
  a.close();

  looped_ = looped;

  alGenSources(1, &srcID_);
  checkReleaseError("");

  alSourcef (srcID_, AL_PITCH,		1.0f);
  alSourcef (srcID_, AL_GAIN,		1.0f);
  alSourcefv(srcID_, AL_POSITION,	pos_);
  alSourcefv(srcID_, AL_VELOCITY,	vel_);
  alSourcei (srcID_, AL_LOOPING,	looped_);

  return loadWav(fileName);
}

void Snd::play()
{
  alSourcePlay(srcID_);
}

void Snd::stop()
{
  alSourceStop(srcID_);
}

void Snd::close()
{
  stop();
  if (alIsSource(srcID_)) {
    alDeleteSources(1, &srcID_);
  }
  if (mVF_) {	
    ov_clear(mVF_);
    delete mVF_;
  }
}

bool Snd::loadWav( CString fileName )
{
  ALenum    format;
  ALsizei    size;
  ALsizei    freq;
  ALuint    BufID = 0;

  loadWavFile(fileName, &buferID_, &size, &freq, &format);
  checkReleaseError("");

  alSourcei (srcID_, AL_BUFFER, buferID_);

  return true;
}
