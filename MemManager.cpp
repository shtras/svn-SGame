#include "StdAfx.h"
#include "MemManager.h"

int debugDelta = 2244;

#ifdef USE_MEMORY_MANAGER
MemManager manager;

void* operator new(size_t size)
{
  return manager.allocate(size);
}

void* operator new[](size_t size)
{
  return manager.allocate(size);
}

void operator delete(void* ptr)
{
  manager.deallocate(ptr);
}

void operator delete[](void* ptr)
{
  manager.deallocate(ptr);
}
#endif

MemManager::MemManager():allocatedSize_(0),allocatedSmall_(0),allocatedLarge_(0)
{
  assert (sizeof(int) >= 4);
  smallSize_ = SMALL_CHUNK_SIZE + sizeof(int);
  largeSize_ = LARGE_CHUNK_SIZE + sizeof(int);
  smallPool_ = (char*)malloc(smallSize_ * NUM_SMALL_CHUNKS);
  largePool_ = (char*)malloc(largeSize_ * NUM_LARGE_CHUNKS);
  int firstSmallInfo = 0;
  firstSmallInfo |= NUM_SMALL_CHUNKS;
  ((int*)smallPool_)[0] = firstSmallInfo;
  int firstLargeInfo = 0;
  firstLargeInfo |= NUM_LARGE_CHUNKS;
  ((int*)largePool_)[0] = firstLargeInfo;
  firstSmall_ = smallPool_;
  firstLarge_ = largePool_;
}

MemManager::~MemManager()
{
  char* smallItr = smallPool_;
  while (1) {
    if (smallItr >= (char*)((int)smallPool_ + ((int)smallSize_*NUM_SMALL_CHUNKS))) {
      break;
    }
    int numChunks = getNumChunks(smallItr);
    bool used = isUsed(smallItr);
    if (used) {
      int delta = smallItr - smallPool_;
      cout << hex << delta << endl;
    }
    smallItr = (char*)((int)smallItr + smallSize_*numChunks);
  }

  char* largeItr = largePool_;
  while (1) {
    if (largeItr >= (char*)((int)largePool_ + ((int)largeSize_*NUM_LARGE_CHUNKS))) {
      break;
    }
    int numChunks = getNumChunks(largeItr);
    bool used = isUsed(largeItr);
    if (used) {
      int delta = largeItr - largePool_;
      cout << hex << delta << endl;
    }
    largeItr = (char*)((int)largeItr + largeSize_*numChunks);
  }
  free (smallPool_);
  free (largePool_);
}

void* MemManager::allocate(size_t size)
{
  if ((int)size > smallSize_*3) {
    return allocateLarge(size);
  } else {
    return allocateSmall(size);
  }
}

void MemManager::deallocate(void* ptr)
{
  if (ptr >= smallPool_ && ptr <= (char*)((int)smallPool_ + ((int)smallSize_*NUM_SMALL_CHUNKS))) {
    deallocateSmall(ptr);
  } else {
    deallocateLarge(ptr);
  }
}

bool MemManager::isUsed( char* addr )
{
  assert (((int)addr%sizeof(int)) == 0);
  int info = *((int*)addr);
  return (info & 0x80000000) != 0;
}

int MemManager::getNumChunks( char* addr )
{
  assert (((int)addr%sizeof(int)) == 0);
  int info = *((int*)addr);
  return info & 0x7FFFFFFF;
}

int MemManager::numSmallRequired( size_t size )
{
  int res = (size + sizeof(int)) / smallSize_ + 1;
  return res;
}

int MemManager::numLargeRequired( size_t size )
{
  int res = (size + sizeof(int)) / largeSize_ + 1;
  return res;
}

void* MemManager::allocateSmall( size_t size )
{
  int numRequired = numSmallRequired(size);
  int deltaAddr = smallSize_;
  char* addr = firstSmall_;
  char* lastAddr = (char*)((int)smallPool_ + ((int)deltaAddr*NUM_SMALL_CHUNKS));
  char* prevAddr = addr;
  while (1) {
    if (addr > lastAddr) {
      assert(0);
      return NULL;
    }
    bool used = isUsed(addr);
    int numChunks = getNumChunks(addr);
    if (used || numChunks < numRequired) {
      prevAddr = addr;
      addr = (char*)((int)addr + deltaAddr*numChunks);
      continue;
    }
    //Found suitable chunk. Mark next chunk
    char* nextAddr = (char*)((int)addr + numRequired*deltaAddr);
    firstSmall_ = nextAddr;
    if (nextAddr <= lastAddr) {
      if (numChunks != numRequired) {
        //Next chunk is still free
        int newNumChunks = numChunks - numRequired;
        int newInfo = 0;
        newInfo |= newNumChunks;
        *((int*)nextAddr) = newInfo;
      }
    }
    break;
  }
  //mark as used
  bool prevUsed = isUsed(prevAddr);
  if (prevUsed) {
    int prevChunks = getNumChunks(prevAddr);
  }
  int newUsedInfo = 0x80000000;
  newUsedInfo |= numRequired;
  *((int*)addr) = newUsedInfo;
  if (addr - smallPool_ == debugDelta) {
    int a = 0;
  }
  void* res = (void*)(addr + sizeof(int));
  //cout << "Allocated: " << dec << numRequired << " at " << hex << (int)addr << " offset: " << (int)(addr) - (int)smallPool_ << endl;
  allocatedSmall_ += numRequired;
  assert(res);
  return res;
}

void* MemManager::allocateLarge( size_t size )
{
  int numRequired = numLargeRequired(size);
  int deltaAddr = largeSize_;
  char* addr = firstLarge_;
  char* lastAddr = (char*)((int)largePool_ + ((int)deltaAddr*NUM_LARGE_CHUNKS));
  char* prevAddr = addr;
  while (1) {
    if (addr > lastAddr) {
      assert(0);
      return NULL;
    }
    bool used = isUsed(addr);
    int numChunks = getNumChunks(addr);
    if (used || numChunks < numRequired) {
      prevAddr = addr;
      addr = (char*)((int)addr + deltaAddr*numChunks);
      continue;
    }
    //Found suitable chunk. Mark next chunk
    char* nextAddr = (char*)((int)addr + numRequired*deltaAddr);
    firstLarge_ = nextAddr;
    if (nextAddr <= lastAddr) {
      if (numChunks != numRequired) {
        //Next chunk is still free
        int newNumChunks = numChunks - numRequired;
        int newInfo = 0;
        newInfo |= newNumChunks;
        *((int*)nextAddr) = newInfo;
      }
    }
    break;
  }
  //mark as used
  bool prevUsed = isUsed(prevAddr);
  if (prevUsed) {
    int prevChunks = getNumChunks(prevAddr);
  }
  int newUsedInfo = 0x80000000;
  newUsedInfo |= numRequired;
  *((int*)addr) = newUsedInfo;
  if (addr - largePool_ == debugDelta) {
    int a = 0;
  }
  void* res = (void*)(addr + sizeof(int));
  //cout << "Allocated: " << dec << numRequired << " at " << hex << (int)addr << " offset: " << (int)(addr) - (int)smallPool_ << endl;
  allocatedLarge_ += numRequired;
  assert(res);
  return res;
}

void MemManager::deallocateSmall( void* ptr )
{
  if (ptr == NULL) {
    return;
  }
  char* chunkAddr = (char*)((int)ptr - sizeof(int));
  if (chunkAddr - smallPool_ == debugDelta) {
    int a = 0;
  }
  if (firstSmall_ > chunkAddr) {
    firstSmall_ = chunkAddr;
  }
  int chunkSize = smallSize_;
  int lastAddr = (int)smallPool_ + chunkSize * NUM_SMALL_CHUNKS;
  bool used = isUsed(chunkAddr);
  assert(used);
  int numChunks = getNumChunks(chunkAddr);
  assert(numChunks > 0);
  int nextAddr = (int)chunkAddr + numChunks*chunkSize;
  assert (nextAddr <= lastAddr);
  int newNumChunks = numChunks;
  if (nextAddr < lastAddr) {
    bool nextUsed = isUsed((char*)nextAddr);
    if (!nextUsed) {
      int nextChunks = getNumChunks((char*)nextAddr);
      newNumChunks += nextChunks;
    }
  }
  allocatedSmall_ -= numChunks;
  int newInfo = 0;
  newInfo |= newNumChunks;
  *((int*)chunkAddr) = newInfo;
}

void MemManager::deallocateLarge( void* ptr )
{
  if (ptr == NULL) {
    return;
  }
  char* chunkAddr = (char*)((int)ptr - sizeof(int));
  if (chunkAddr - largePool_ == debugDelta) {
    int a = 0;
  }
  if (firstLarge_ > chunkAddr) {
    firstLarge_ = chunkAddr;
  }
  int chunkSize = largeSize_;
  int lastAddr = (int)largePool_ + chunkSize * NUM_LARGE_CHUNKS;
  bool used = isUsed(chunkAddr);
  assert(used);
  int numChunks = getNumChunks(chunkAddr);
  assert(numChunks > 0);
  int nextAddr = (int)chunkAddr + numChunks*chunkSize;
  assert (nextAddr <= lastAddr);
  int newNumChunks = numChunks;
  if (nextAddr < lastAddr) {
    bool nextUsed = isUsed((char*)nextAddr);
    if (!nextUsed) {
      int nextChunks = getNumChunks((char*)nextAddr);
      newNumChunks += nextChunks;
    }
  }
  allocatedLarge_ -= numChunks;
  int newInfo = 0;
  newInfo |= newNumChunks;
  *((int*)chunkAddr) = newInfo;
}

