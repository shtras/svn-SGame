#pragma once

//#define USE_MEMORY_MANAGER

#ifdef USE_MEMORY_MANAGER
void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr);
void operator delete[](void* ptr);
#endif

#define SMALL_CHUNK_SIZE 64
#define LARGE_CHUNK_SIZE 1024
#define NUM_SMALL_CHUNKS 100000
#define NUM_LARGE_CHUNKS 100000

/*
chunkID:
used:1
numFree/Used:15
numFree/Used prev: 16
*/

class MemManager
{
public:
  MemManager();
  ~MemManager();
  void* allocate(size_t size);
  void deallocate(void* ptr);
private:
  void* allocateSmall(size_t size);
  void* allocateLarge(size_t size);
  void deallocateSmall(void* ptr);
  void deallocateLarge(void* ptr);
  char* smallPool_;
  char* largePool_;
  bool isUsed(char* addr);
  int getNumChunks(char* addr);
  int numSmallRequired(size_t size);
  int numLargeRequired(size_t size);
  size_t allocatedSize_;
  int smallSize_;
  int largeSize_;
  char* firstSmall_;
  char* firstLarge_;
  int allocatedSmall_;
  int allocatedLarge_;
};

