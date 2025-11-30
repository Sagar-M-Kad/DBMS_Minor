# File Indexing - Concept and Implementation

This document explains the concept of **File Indexing** as demonstrated in the accompanying C program `indexing_demo.c`.

## What is Indexing?

**Indexing** is a technique to speed up data retrieval from files. Think of it like the index at the back of a book:
- Instead of reading every page to find a topic, you look it up in the index.
- The index tells you the exact page number.
- You jump directly to that page.

Similarly, in file systems:
- Instead of reading the entire file sequentially to find a record, we maintain an **Index Table**.
- The index maps a **Key** (like Student ID) to the **Offset** (byte position) in the file.
- We can then use `fseek()` to jump directly to that position and read the record.

---

## Types of Indexing

### 1. Primary Index
- Built on the **primary key** (unique identifier) of records.
- Records are usually stored in **sorted order** by the key.
- Our demo uses this approach.

### 2. Secondary Index
- Built on **non-key fields** (like Name or Age).
- Multiple records can have the same value.
- Allows searching by fields other than the primary key.

### 3. Dense vs Sparse Index
- **Dense Index**: One index entry for **every** record.
- **Sparse Index**: Index entries only for some records (e.g., one per block).

Our program uses a **Dense Primary Index**.

---

## How the Program Works

### Data Structure

#### Student Record (Data File)
```c
typedef struct {
    int id;
    char name[50];
    float gpa;
} Student;
```
This is stored in the binary file `students.dat`.

#### Index Entry (In-Memory)
```c
typedef struct {
    int id;       // Key (Student ID)
    long offset;  // Byte position in the file
} Index;
```
The index table is kept in memory as an array.

---

### Operations

#### 1. Add Record
1. **Check for duplicates** in the index table.
2. Get the current file offset using `ftell()`.
3. Write the student record to the file using `fwrite()`.
4. Add an entry to the index table: `{id, offset}`.

#### 2. Search Record (Fast Retrieval)
1. **Search the index table** for the given ID (linear search, could be binary search if sorted).
2. If found, get the **offset**.
3. Open the data file and use `fseek(fp, offset, SEEK_SET)` to **jump directly** to that position.
4. Read the record using `fread()`.

**Key Advantage**: We don't read the entire file. We jump straight to the record!

#### 3. Display Index Table
Shows the current mapping of IDs to offsets.

---

## Key Functions Used

### `ftell()`
- **Purpose**: Returns the current position (offset) in the file.
- **Usage**: We use this to record where each record is stored.
```c
long offset = ftell(fp);
```

### `fseek()`
- **Purpose**: Moves the file pointer to a specific position.
- **Usage**: Jump directly to a record's offset.
```c
fseek(fp, offset, SEEK_SET);
```

### `fread()` and `fwrite()`
- **Binary I/O**: Read/Write entire structures at once.
```c
fwrite(&s, sizeof(Student), 1, fp);  // Write 1 Student structure
fread(&s, sizeof(Student), 1, fp);   // Read 1 Student structure
```

---

## Performance Comparison

| Operation | Without Index | With Index |
|-----------|--------------|------------|
| **Search by ID** | Read entire file (O(n)) | Search index + 1 read (O(log n) if sorted) |
| **Insert** | Append to file (O(1)) | Append + Update index (O(1)) |

---

## Real-World Applications

1. **Database Systems**: All databases (MySQL, Oracle, etc.) use indexing extensively.
2. **File Systems**: Directories are essentially indexes mapping filenames to disk blocks.
3. **Search Engines**: Inverted indexes map words to document IDs.

---

## Limitations of This Demo

1. **Index is in-memory**: If the program closes, the index is rebuilt on restart. In real systems, the index is also stored on disk.
2. **No sorting**: We use linear search. A sorted index would allow binary search (faster).
3. **No deletion**: Deleting records would require updating the index and handling fragmentation.

---

## How to Run

### Compile
```bash
gcc indexing_demo.c -o indexing_demo
```

### Run
```bash
./indexing_demo
```

### Example Workflow
1. **Add 3 students** (IDs: 101, 102, 103).
2. **Display Index Table** - You'll see the offsets (0, 60, 120, etc. depending on struct size).
3. **Search for ID 102** - The program will instantly retrieve it using the offset.
4. **Exit and restart** - The index is rebuilt by reading the file.

---

## Summary

**Indexing** is a fundamental technique in file handling and database management. By maintaining a mapping of keys to file positions, we can achieve **random access** to records without sequential scanning, dramatically improving search performance.
