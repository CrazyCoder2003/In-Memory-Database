#include "Database.h"
#include "../parser/QueryParser.h"
#include <sstream>
#include <iostream>
#include <stdexcept>

void Database::execute(const std::string& stmt) {
    AST::Statement ast = QueryParser::parse(stmt);

    if (std::holds_alternative<AST::CreateStatement>(ast)) {
        createTable(std::get<AST::CreateStatement>(ast));
    } else if (std::holds_alternative<AST::InsertStatement>(ast)) {
        insertInto(std::get<AST::InsertStatement>(ast));
    } else if (std::holds_alternative<AST::DeleteStatement>(ast)) {
        deleteFrom(std::get<AST::DeleteStatement>(ast));
    } else {
        std::cerr << "[execute] unsupported statement (or SELECT)\n";
    }
}

std::string Database::query(const std::string& stmt) {
    AST::Statement ast = QueryParser::parse(stmt);

    if (std::holds_alternative<AST::SelectStatement>(ast)) {
        auto rows = selectFrom(std::get<AST::SelectStatement>(ast));
        std::ostringstream oss;
        for (auto &r : rows) {
            for (size_t i = 0; i < r.size(); ++i) {
                if (i) oss << " | ";
                oss << r[i];
            }
            oss << "\n";
        }
        return oss.str();
    }

    // Non-select => execute and return OK
    execute(stmt);
    return "OK\n";
}

void Database::createTable(const AST::CreateStatement& c) {
    std::unique_lock lock(tablesMutex_);
    if (tables_.count(c.table)) {
        throw std::runtime_error("Table already exists: " + c.table);
    }
    // Build column name list
    std::vector<std::string> cols;
    for (auto &p : c.columns) cols.push_back(p.first);

    tables_[c.table] = std::make_shared<Table>(c.table, cols, 1024);
}

void Database::insertInto(const AST::InsertStatement& ins) {
    std::shared_lock lock(tablesMutex_);
    auto it = tables_.find(ins.table);
    if (it == tables_.end()) {
        throw std::runtime_error("No such table: " + ins.table);
    }
    auto tablePtr = it->second;
    if (!tablePtr) throw std::runtime_error("Internal error: null table ptr");

    if (ins.values.empty()) throw std::runtime_error("INSERT missing values");
    std::string key = ins.values[0];

    std::vector<std::string> row;
    for (size_t i = 1; i < ins.values.size(); ++i) row.push_back(ins.values[i]);
    std::string csv = joinRow(row);
    tablePtr->data.insert(key, csv);
}

void Database::deleteFrom(const AST::DeleteStatement& del) {
    std::shared_lock lock(tablesMutex_);
    auto it = tables_.find(del.table);
    if (it == tables_.end()) throw std::runtime_error("No such table: " + del.table);
    auto tablePtr = it->second;
    if (!tablePtr) throw std::runtime_error("Internal error: null table ptr");
    tablePtr->data.remove(del.key);
}

std::vector<std::vector<std::string>> Database::selectFrom(const AST::SelectStatement& s) {
    std::vector<std::vector<std::string>> result;
    std::shared_lock lock(tablesMutex_);
    auto it = tables_.find(s.table);
    if (it == tables_.end()) throw std::runtime_error("No such table: " + s.table);
    auto tablePtr = it->second;
    if (!tablePtr) throw std::runtime_error("Internal error: null table ptr");

    // Walk all rows in the lock-free hashmap
    tablePtr->data.for_each([&](const std::string& key, const std::string& csv){
        std::vector<std::string> row;
        row.push_back(key);                     // primary key first
        auto fields = splitRow(csv);            // remaining columns
        for (auto &f : fields) row.push_back(f);
        result.push_back(std::move(row));
    });

    return result;
}

std::string Database::joinRow(const std::vector<std::string>& row, char sep) {
    std::string out;
    for (size_t i = 0; i < row.size(); ++i) {
        if (i) out.push_back(sep);
        out += row[i];
    }
    return out;
}

std::vector<std::string> Database::splitRow(const std::string& row, char sep) {
    std::vector<std::string> out;
    if (row.empty()) return out;
    size_t start = 0;
    for (size_t i = 0; i <= row.size(); ++i) {
        if (i == row.size() || row[i] == sep) {
            out.push_back(row.substr(start, i - start));
            start = i + 1;
        }
    }
    return out;
}
