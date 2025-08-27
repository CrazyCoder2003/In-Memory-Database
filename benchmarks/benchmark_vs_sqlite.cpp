#include "../src/storage/Database.h"
#include <iostream>
#include <chrono>

int main() {
    Database db;
    
    // Create a test table
    db.execute("CREATE TABLE test (id INT, data VARCHAR)");
    
    // Benchmark INSERT performance
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < 1000; i++) {
        db.execute("INSERT INTO test VALUES ('" + std::to_string(i) + "', 'test data " + std::to_string(i) + "')");
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Inserted 1000 records in " << duration.count() << " ms" << std::endl;
    
    // Benchmark SELECT performance
    start = std::chrono::high_resolution_clock::now();
    
    std::string result = db.query("SELECT * FROM test");
    
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Selected all records in " << duration.count() << " ms" << std::endl;
    
    return 0;
}