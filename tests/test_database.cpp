#include "/Users/ayush/CLionProjects/CPP_project/src/storage/Database.h"
#include <iostream>
#include <cassert>

int main() {
    Database db;

    try {
        db.execute("CREATE TABLE users (id INT, name VARCHAR, email VARCHAR)");
        std::cout << "CREATE TABLE test passed!" << std::endl;

        db.execute("INSERT INTO users VALUES ('1', 'John', 'john@example.com')");
        std::cout << "INSERT test passed!" << std::endl;

        std::string result = db.query("SELECT * FROM users");
        std::cout << "SELECT result: " << result << std::endl;
        std::cout << "SELECT test passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Database test failed: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "All database tests passed!" << std::endl;
    return 0;
}
