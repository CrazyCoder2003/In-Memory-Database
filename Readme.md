# In-Memory Database (C++)

A lightweight **in-memory SQL-like database** implemented in C++.  
It supports creating tables, inserting rows, selecting data, and basic storage using a lock-free hashmap.

⚡ **Note:** This is a prototype — all data exists only in memory and is lost once the program exits.

---

## Features
- SQL-like REPL interface
- `CREATE TABLE` support
- `INSERT INTO` support
- `SELECT * FROM table` support
- Lock-free hashmap backend for fast key/value storage
- Exception handling for invalid queries

---

## Getting Started

### 1. Build
```bash
mkdir build && cd build
cmake ..
make
```

### 2. Run the main program
```bash
./in-memory-db
```

You will see:
```bash
In-Memory Database Started
Type SQL, or EXIT/QUIT to leave
DB>
```
## Example Usage

```bash
-- Create a table
CREATE TABLE users;

-- Insert some rows
INSERT INTO users (1, 'Ayush');
INSERT INTO users (2, 'John Doe');

-- Select data
SELECT * FROM users;

-- Exit
EXIT;
```
### Running Tests
```bash
./test_allocator
./test_database
./test_hashmap
./test_parser
```

### Expected Outputs
```bash
$ ./test_allocator
MemoryPool test passed!

$ ./test_database
CREATE TABLE test passed!
INSERT test passed!
SELECT result: 1 | John | john@example.com
SELECT test passed!
All database tests passed!

$ ./test_hashmap
LockFreeHashMap test passed!

$ ./test_parser
Parser test passed!
```
### Known Limitations
- Data is not persistent — all tables/rows are deleted when the process exits.

- Only very basic SQL parsing is supported (```CREATE, INSERT, SELECT```).
- No type enforcement — all values are stored as strings internally.
- No query optimization (linear scans for now).

### Project Structure

```bash
CPP_project/
├── src/
│   ├── main.cpp              # Entry point (REPL)
│   ├── parser/               # SQL parsing logic
│   ├── storage/              # Table & data storage
│   ├── hashmap/              # LockFreeHashMap implementation
│   └── allocator/            # Custom memory pool
├── tests/                    # Unit tests
├── CMakeLists.txt            # Build configuration
└── README.md                 # Project documentation
```

## Development Notes
- Backend: Lock-free hashmap (```LockFreeHashMap```) for storage

- Parser: Very simple SQL parser (regex + string splitting)

- Main Entry: ```main.cpp```

- Future Plans:

    Add persistence (save/load from file)

  Support ```DELETE```, ```UPDATE```

    Add schema (columns + types)

    Indexing for faster ```SELECT```

### Quick Hack for Persistence
Since everything is in-memory, you can preload your DB with a bootstrap file:

```bash 
./in-memory-db < bootstrap.sql
```
Where ```bootstrap.sql``` contains:

```bash
CREATE TABLE users;
INSERT INTO users (1, 'Ayush');
INSERT INTO users (2, 'Jane Doe');
```




