#pragma once

#include <string>
#include <vector>
#include <memory>
#include <shared_mutex>
#include <unordered_map>

#include "../hashmap/LockFreeHashMap.h"
#include "../parser/AST.h"
#include "../parser/QueryParser.h"

// Table holds schema (column names) and the data map (key -> CSV row)
struct Table {
    std::string name;
    std::vector<std::string> columns;
    // data: primaryKey -> CSV of remaining columns
    LockFreeHashMap<std::string, std::string> data;

    Table(const std::string& n, const std::vector<std::string>& cols, size_t capacity = 1024)
        : name(n), columns(cols), data(capacity) {}
};

class Database {
public:
    Database() = default;

    // Execute DDL/DML (CREATE, INSERT, DELETE) or SELECT (query returns string)
    void execute(const std::string& stmt);
    std::string query(const std::string& stmt);

private:
    void createTable(const AST::CreateStatement& c);
    void insertInto(const AST::InsertStatement& ins);
    void deleteFrom(const AST::DeleteStatement& del);
    std::vector<std::vector<std::string>> selectFrom(const AST::SelectStatement& s);

    // utilities
    static std::string joinRow(const std::vector<std::string>& row, char sep = ',');
    static std::vector<std::string> splitRow(const std::string& row, char sep = ',');

    // storage: map tableName -> Table pointer (shared so hashmap not copied)
    std::unordered_map<std::string, std::shared_ptr<Table>> tables_;
    mutable std::shared_mutex tablesMutex_;
};
