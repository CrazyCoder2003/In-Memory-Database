#include "/Users/ayush/CLionProjects/CPP_project/src/hashmap/LockFreeHashMap.h"
#include <iostream>
#include <cassert>

int main() {
    LockFreeHashMap<std::string, std::string> map(10);
    
    // Test insert
    map.insert("key1", "value1");
    map.insert("key2", "value2");
    
    // Test find
    auto value1 = map.find("key1");
    assert(value1.has_value());
    assert(value1.value() == "value1");
    
    auto value2 = map.find("key2");
    assert(value2.has_value());
    assert(value2.value() == "value2");
    
    // Test remove
    assert(map.remove("key1"));
    assert(!map.find("key1").has_value());
    
    std::cout << "LockFreeHashMap test passed!" << std::endl;
    return 0;
}