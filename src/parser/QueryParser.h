#pragma once
#include "AST.h"
#include <string>

class QueryParser {
public:
    // Parse returns an AST::Statement or throws on unsupported SQL
    static AST::Statement parse(const std::string& query);

private:
    static AST::SelectStatement parseSelect(const std::string& query);
    static AST::InsertStatement parseInsert(const std::string& query);
    static AST::CreateStatement parseCreate(const std::string& query);
    static AST::DeleteStatement parseDelete(const std::string& query);
};
