#include "QueryParser.h"
#include <regex>
#include <stdexcept>

AST::Statement QueryParser::parse(const std::string& query) {
    std::regex selectRegex(R"(SELECT\s+(.*?)\s+FROM\s+(\w+)(?:\s+WHERE\s+(.*))?)",
                          std::regex_constants::icase);
    std::regex insertRegex(R"(INSERT\s+INTO\s+(\w+)\s+VALUES\s*\((.*)\))",
                          std::regex_constants::icase);
    std::regex createRegex(R"(CREATE\s+TABLE\s+(\w+)\s*\((.*)\))",
                          std::regex_constants::icase);
    std::regex deleteRegex(R"(DELETE\s+FROM\s+(\w+)\s+WHERE\s+(\w+)\s*=\s*'([^']*)')",
                          std::regex_constants::icase);

    std::smatch matches;
    if (std::regex_search(query, matches, selectRegex)) {
        return parseSelect(query);
    } else if (std::regex_search(query, matches, insertRegex)) {
        return parseInsert(query);
    } else if (std::regex_search(query, matches, createRegex)) {
        return parseCreate(query);
    } else if (std::regex_search(query, matches, deleteRegex)) {
        return parseDelete(query);
    } else {
        throw std::runtime_error("Unsupported SQL statement");
    }
}

AST::SelectStatement QueryParser::parseSelect(const std::string& query) {
    AST::SelectStatement stmt;
    std::regex selectRegex(R"(SELECT\s+(.*?)\s+FROM\s+(\w+)(?:\s+WHERE\s+(.*))?)",
                          std::regex_constants::icase);
    std::smatch matches;
    if (std::regex_search(query, matches, selectRegex)) {
        std::string columnsStr = matches[1].str();
        if (columnsStr == "*") {
            stmt.columns.push_back("*");
        } else {
            std::regex columnRegex(R"(\s*(\w+)\s*(?:,|$))");
            auto it = std::sregex_iterator(columnsStr.begin(), columnsStr.end(), columnRegex);
            auto end = std::sregex_iterator();
            for (; it != end; ++it) stmt.columns.push_back((*it)[1].str());
        }
        stmt.table = matches[2].str();
        if (matches.size() > 3) stmt.whereClause = matches[3].str();
    }
    return stmt;
}

AST::InsertStatement QueryParser::parseInsert(const std::string& query) {
    AST::InsertStatement stmt;
    std::regex insertRegex(R"(INSERT\s+INTO\s+(\w+)\s+VALUES\s*\((.*)\))",
                          std::regex_constants::icase);
    std::smatch matches;
    if (std::regex_search(query, matches, insertRegex)) {
        stmt.table = matches[1].str();
        std::string valuesStr = matches[2].str();
        std::regex valueRegex(R"(\s*'([^']*)'\s*(?:,|$))");
        for (auto it = std::sregex_iterator(valuesStr.begin(), valuesStr.end(), valueRegex);
             it != std::sregex_iterator(); ++it) {
            stmt.values.push_back((*it)[1].str());
        }
    } else {
        throw std::runtime_error("Invalid INSERT statement");
    }
    return stmt;
}

AST::CreateStatement QueryParser::parseCreate(const std::string& query) {
    AST::CreateStatement stmt;
    std::regex createRegex(R"(CREATE\s+TABLE\s+(\w+)\s*\((.*)\))",
                          std::regex_constants::icase);
    std::smatch matches;
    if (std::regex_search(query, matches, createRegex)) {
        stmt.table = matches[1].str();
        std::string columnsStr = matches[2].str();
        std::regex columnRegex(R"(\s*(\w+)\s+(\w+)\s*(?:,|$))");
        for (auto it = std::sregex_iterator(columnsStr.begin(), columnsStr.end(), columnRegex);
             it != std::sregex_iterator(); ++it) {
            stmt.columns.emplace_back((*it)[1].str(), (*it)[2].str());
        }
    } else {
        throw std::runtime_error("Invalid CREATE statement");
    }
    return stmt;
}

AST::DeleteStatement QueryParser::parseDelete(const std::string& query) {
    AST::DeleteStatement stmt;
    std::regex deleteRegex(R"(DELETE\s+FROM\s+(\w+)\s+WHERE\s+(\w+)\s*=\s*'([^']*)')",
                          std::regex_constants::icase);
    std::smatch matches;
    if (std::regex_search(query, matches, deleteRegex)) {
        stmt.table = matches[1].str();
        // matches[2] is column, matches[3] is value -> assume primary key
        stmt.key = matches[3].str();
    } else {
        throw std::runtime_error("Invalid DELETE statement");
    }
    return stmt;
}
