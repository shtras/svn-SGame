#include "StdAfx.h"
#include "auxFuncs.h"

double RadToDeg(double rad)
{
  return rad * 180.0 / PI;
}

double DegToRad(double deg)
{
  return deg * PI / 180.0;
}

bool isNumberChar(int key)
{
  if (key >= 'a' && key <= 'z') {
    return true;
  }
  if (key >= 'A' && key <= 'Z') {
    return true;
  }
  return false;
}

bool isLetterChar(int key)
{
  if (key >= 0x30 && key <= 0x39) {
    return true;
  }
  return false;
}

void checkErrorDebug(CString errorMsg/* = ""*/)
{
  GLenum err = glGetError();
  if (err != 0) {
    Logger::getInstance().log(ERROR_LOG_NAME, errorMsg + CString(" OpenGL error: ") + CString((int)err));
  }
  assert(err == 0);
  ALenum alErr = alGetError();
  if (alErr != AL_NO_ERROR) {
    Logger::getInstance().log(ERROR_LOG_NAME, errorMsg + CString(" OpenAL error: ") + CString((int)alErr));
  }
  assert(alErr == AL_NO_ERROR);
}

void checkReleaseError(CString errorMsg)
{
  checkErrorDebug(errorMsg);
}

int parseInt(CString str)
{
  if (str.getSize() == 0) {
    return 0;
  }
  int res = 0;
  bool neg = false;
  int start = 0;
  if (str[0] == '-') {
    neg = true;
    ++start;
  }
  for (int i=start; i<str.getSize(); ++i) {
    char c = str[i];
    if (c < '0' || c > '9') {
      Logger::getInstance().log(ERROR_LOG_NAME, "Error parsing integer: " + str);
      return 0;
    }
    int digit = c - '0';
    res += digit;
    res *= 10;
  }
  res /= 10;
  if (neg) {
    res = -res;
  }
  return res;
}

void writeStringToFile(FILE* file, CString str)
{
  char buffer[1024];
  buffer[0] = str.getSize();
  for (int i=0; i<str.getSize(); ++i) {
    assert (i<1023);
    buffer[i+1] = str[i] - 'a';
  }
  fwrite(buffer, 1, str.getSize()+1, file);
}

void writeToFile(void* buffer, size_t size, int count, FILE* file)
{
  size_t res = fwrite(buffer, size, count, file);
  assert (res == count);
}

bool readFromFile(void* buffer, size_t size, int count, FILE* file)
{
  size_t res = fread(buffer, size, count, file);
  return (res == count);
}

CString readStringFromFile(FILE* file)
{
  char size;
  if(!readFromFile(&size, 1, 1, file)) {
    return "";
  }
  char buffer[256];
  if (size > 255) {
    return "";
  }
  if (!readFromFile(buffer, 1, size, file)) {
    return "";
  }
  buffer[size] = 0;
  for (int i=0; i<size; ++i) {
    buffer[i] += 'a';
  }
  return CString(buffer);
}

FilePointer::FilePointer(const char* fileName, std::ios_base::openmode mode)
{
  file_.open(fileName, mode);
}

FilePointer::~FilePointer()
{
  file_.close();
}

bool loadWavFile(CString filename, ALuint* buffer, ALsizei* size, ALsizei* frequency, ALenum* format)
{
  FILE* soundFile = NULL;
  WAVE_Format wave_format;
  RIFF_Header riff_header;
  WAVE_Data wave_data;
  unsigned char* data;

  soundFile = fopen(filename, "rb");
  if (!soundFile) {
    return false;
  }

  fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);

  if ((riff_header.chunkID[0] != 'R' ||
    riff_header.chunkID[1] != 'I' ||
    riff_header.chunkID[2] != 'F' ||
    riff_header.chunkID[3] != 'F') &&
    (riff_header.format[0] != 'W' ||
    riff_header.format[1] != 'A' ||
    riff_header.format[2] != 'V' ||
    riff_header.format[3] != 'E')) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Invalid RIFF or WAVE Header");
    fclose(soundFile);
    return false;
  }


  fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
  if (wave_format.subChunkID[0] != 'f' ||
    wave_format.subChunkID[1] != 'm' ||
    wave_format.subChunkID[2] != 't' ||
    wave_format.subChunkID[3] != ' ') {
    Logger::getInstance().log(ERROR_LOG_NAME, "Invalid WAV Format");
    fclose(soundFile);
    return false;
  }

  if (wave_format.subChunkSize > 16) {
    fseek(soundFile, sizeof(short), SEEK_CUR);
  }

  fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
  if (wave_data.subChunkID[0] != 'd' ||
    wave_data.subChunkID[1] != 'a' ||
    wave_data.subChunkID[2] != 't' ||
    wave_data.subChunkID[3] != 'a') {
    Logger::getInstance().log(ERROR_LOG_NAME, "Invalid WAV data header");
    fclose(soundFile);
    return false;
  }

  data = new unsigned char[wave_data.subChunk2Size];

  if (!fread(data, wave_data.subChunk2Size, 1, soundFile)) {
    Logger::getInstance().log(ERROR_LOG_NAME, "Error loading WAV data into struct");
  }
  fclose(soundFile);

  *size = wave_data.subChunk2Size;
  *frequency = wave_format.sampleRate;
  if (wave_format.numChannels == 1) {
    if (wave_format.bitsPerSample == 8 ) {
      *format = AL_FORMAT_MONO8;
    } else if (wave_format.bitsPerSample == 16)
      *format = AL_FORMAT_MONO16;
  } else if (wave_format.numChannels == 2) {
    if (wave_format.bitsPerSample == 8 ) {
      *format = AL_FORMAT_STEREO8;
    } else if (wave_format.bitsPerSample == 16) {
      *format = AL_FORMAT_STEREO16;
    }
  }
  alGenBuffers(1, buffer);
  alBufferData(*buffer, *format, (void*)data, *size, *frequency);
  delete[] data;
  return true;
}
