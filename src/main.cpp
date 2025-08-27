#include <iostream>
#include "storage/Database.h"

int main() {
    Database db;
    std::cout << "In-Memory Database Started\nType SQL, or EXIT/QUIT to leave\n";

    std::string line;
    while (true) {
        std::cout << "DB> ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT" || line == "QUIT") break;
        if (line.empty()) continue;

        try {
            std::string out = db.query(line);
            if (!out.empty()) std::cout << out;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }
    return 0;
}
