#pragma once

struct SndInfo
{
  unsigned int	ID;
  CString		Filename;
  unsigned int	Rate;
  unsigned int	Format;
};

class Snd
{
public:
  Snd();
  ~Snd();
  bool open(CString fileName, bool looped, bool streamed);
  bool isLooped() {return looped_;}
  bool isStreamed() {return streamed_;}
private:
  ALfloat vel_[3];
  ALfloat pos_[3];
  bool  looped_;
  bool streamed_;
};

class SoundManager
{
public:
  static SoundManager& getInstance();
  bool init();
private:
  SoundManager();
  ~SoundManager();

  ALCdevice* device_;
  ALCcontext* context_;
  map<ALuint, SndInfo> buffers_;
};
