#pragma once
#include <cstddef>
#include <vector>
#include <mutex>

class MemoryPool {
public:
    MemoryPool(size_t blockSize, size_t numBlocks);
    ~MemoryPool();
    
    void* allocate();
    void deallocate(void* ptr);
    
    size_t getBlockSize() const { return blockSize_; }
    size_t getAvailableBlocks() const { return availableBlocks_; }
    
private:
    size_t blockSize_;
    size_t availableBlocks_;
    char* memoryPool_;
    std::vector<void*> freeList_;
    std::mutex mutex_;
};