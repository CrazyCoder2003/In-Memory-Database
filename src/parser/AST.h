#pragma once
#include <string>
#include <vector>
#include <utility>
#include <variant>

namespace AST {

    struct SelectStatement {
        std::vector<std::string> columns; // "*" or list
        std::string table;
        std::string whereClause; // not used in prototype
    };

    struct InsertStatement {
        std::string table;
        std::vector<std::string> values; // values as strings (first one assumed primary key)
    };

    struct CreateStatement {
        std::string table;
        std::vector<std::pair<std::string, std::string>> columns; // (name,type)
    };

    struct DeleteStatement {
        std::string table;
        std::string key; // simple delete by primary key
    };

    using Statement = std::variant<SelectStatement, InsertStatement, CreateStatement, DeleteStatement>;

} // namespace AST
