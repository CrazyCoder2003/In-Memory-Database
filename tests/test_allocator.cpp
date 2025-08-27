#include "/Users/ayush/CLionProjects/CPP_project/src/allocator/MemoryPool.h"
#include <cassert>
#include <iostream>

int main() {
    MemoryPool pool(256, 10);
    void* block = pool.allocate();
    assert(block != nullptr);
    pool.deallocate(block);
    std::cout << "MemoryPool test passed!" << std::endl;
    return 0;
}