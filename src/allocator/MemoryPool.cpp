#include "MemoryPool.h"
#include <cstdlib>
#include <iostream>

MemoryPool::MemoryPool(size_t blockSize, size_t numBlocks)
    : blockSize_(blockSize), availableBlocks_(numBlocks) {
    memoryPool_ = static_cast<char*>(malloc(blockSize * numBlocks));

    // Initialize free list
    for (size_t i = 0; i < numBlocks; ++i) {
        freeList_.push_back(memoryPool_ + i * blockSize);
    }
}

MemoryPool::~MemoryPool() {
    free(memoryPool_);
}

void* MemoryPool::allocate() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (freeList_.empty()) {
        return nullptr; // Out of memory
    }

    void* ptr = freeList_.back();
    freeList_.pop_back();
    availableBlocks_--;

    return ptr;
}

void MemoryPool::deallocate(void* ptr) {
    std::lock_guard<std::mutex> lock(mutex_);

    freeList_.push_back(ptr);
    availableBlocks_++;
}