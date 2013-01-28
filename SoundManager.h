#pragma once

class Snd
{
public:
  Snd();
  ~Snd();
  bool open(CString fileName, bool looped, bool streamed);
  bool isLooped() {return looped_;}
  bool isStreamed() {return streamed_;}
  void play();
  void stop();
  void close();
  ALuint getID() {return srcID_;}
private:
  bool loadWav(CString fileName);
  ALfloat vel_[3];
  ALfloat pos_[3];
  bool  looped_;
  bool streamed_;
  ALuint srcID_;
  ALuint buferID_;

  OggVorbis_File* mVF_;
  vorbis_comment* comment_;
  vorbis_info* info_;
};

class SoundManager
{
public:
  enum SoundType {
    Ambient = 0,
    Test = 1,
    LastSound = 1
  };
  static SoundManager& getInstance();
  bool init();
  void openSoundFile(SoundType type, CString fileName);
private:
  SoundManager();
  ~SoundManager();

  ALCdevice* device_;
  ALCcontext* context_;
  map<SoundType, Snd*> sounds_;
};
