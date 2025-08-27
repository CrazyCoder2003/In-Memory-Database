cmake_minimum_required(VERSION 3.16)
project(InMemoryDB LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
enable_testing()

# -----------------------------------------------------------------------------
# Core library (Database, Query Parser, Allocator, Hashmap, etc.)
# -----------------------------------------------------------------------------
add_library(db_core
        src/storage/Database.cpp
        src/parser/QueryParser.cpp
        src/allocator/MemoryPool.cpp
)

target_include_directories(db_core PUBLIC
        ${PROJECT_SOURCE_DIR}/src
)

# Thread support
find_package(Threads REQUIRED)
target_link_libraries(db_core PUBLIC Threads::Threads)

# -----------------------------------------------------------------------------
# Main executable (interactive shell for the in-memory DB)
# -----------------------------------------------------------------------------
add_executable(in-memory-db
        src/main.cpp
)

target_link_libraries(in-memory-db PRIVATE db_core)

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
add_executable(test_allocator tests/test_allocator.cpp)
target_link_libraries(test_allocator PRIVATE db_core)
add_test(NAME AllocatorTests COMMAND test_allocator)

add_executable(test_hashmap tests/test_hashmap.cpp)
target_link_libraries(test_hashmap PRIVATE db_core)
add_test(NAME HashmapTests COMMAND test_hashmap)

add_executable(test_parser tests/test_parser.cpp)
target_link_libraries(test_parser PRIVATE db_core)
add_test(NAME ParserTests COMMAND test_parser)

add_executable(test_database tests/test_database.cpp)
target_link_libraries(test_database PRIVATE db_core)
add_test(NAME DatabaseTests COMMAND test_database)

# -----------------------------------------------------------------------------
# Benchmarks (compare against SQLite)
# -----------------------------------------------------------------------------
find_package(SQLite3 REQUIRED)

add_executable(benchmark_vs_sqlite benchmarks/benchmark_vs_sqlite.cpp)
target_link_libraries(benchmark_vs_sqlite PRIVATE db_core SQLite::SQLite3)
