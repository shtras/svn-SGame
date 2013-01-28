#pragma once
#include "al.h"

class WLayout;

struct WAVE_Data {
  char subChunkID[4];
  long subChunk2Size;
};

struct WAVE_Format {
  char subChunkID[4];
  long subChunkSize;
  short audioFormat;
  short numChannels;
  long sampleRate;
  long byteRate;
  short blockAlign;
  short bitsPerSample;
};

struct RIFF_Header {
  char chunkID[4];
  long chunkSize;
  char format[4];
};

double RadToDeg(double rad);
double DegToRad(double deg);

bool isNumberChar(int key);
bool isLetterChar(int key);

void checkErrorDebug(CString errorMsg = "");

void checkReleaseError(CString errorMsg);

vector<CString> getFileNames(CString dir);

bool loadWavFile(CString filename, ALuint* buffer, ALsizei* size, ALsizei* frequency, ALenum* format);

int parseInt(CString str);

void writeStringToFile(FILE* file, CString str);

void writeToFile(void* buffer, size_t size, int count, FILE* file);

bool readFromFile(void* buffer, size_t size, int count, FILE* file);

CString readStringFromFile(FILE* file);

class FilePointer
{
public:
  FilePointer(const char* fileName, std::ios_base::openmode mode);
  ~FilePointer();
  fstream& operator->();
private:
  fstream file_;
};
