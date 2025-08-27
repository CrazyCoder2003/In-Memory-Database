#include "/Users/ayush/CLionProjects/CPP_project/src/parser/QueryParser.h"
#include <iostream>

int main() {
    try {
        auto stmt = QueryParser::parse("SELECT * FROM table");
        std::cout << "Parser test passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Parser test failed: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}