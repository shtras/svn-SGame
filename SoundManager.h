#pragma once

struct SndInfo
{
  unsigned int ID;
  CString  Filename;
  unsigned int Rate;
  unsigned int Format;
};

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
  void update();
private:
  bool loadWav(CString fileName);
  bool loadOGG(CString fileName, bool streamed);
  bool readOggBlock(ALuint BufID, size_t Size);

  ALfloat vel_[3];
  ALfloat pos_[3];
  bool  looped_;
  bool streamed_;
  ALuint srcID_;
  ALuint wavBufferID_;
  OggVorbis_File* mVF_;
  vorbis_comment* comment_;
  vorbis_info* info_;
  ifstream oggFile_;
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
  map<ALuint, SndInfo>& getBuffers() {return buffers_;}
  void update();
  void playSound(SoundType type);
  void stopSound(SoundType type);
private:
  SoundManager();
  ~SoundManager();

  ALCdevice* device_;
  ALCcontext* context_;
  map<SoundType, Snd*> sounds_;
  map<ALuint, SndInfo> buffers_;
};
