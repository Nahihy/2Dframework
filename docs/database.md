# Database Library

A lightweight persistent key-value storage library for C. Saves and loads named variables from a file in either a human-readable text format or a compact binary format.

---

## Requirements

- Standart C compiler
- CMake 

Include with:
```c
#include <database/database.h>
```

---

## Limits

| Constant | Value | Meaning |
|---|---|---|
| `DB_MAX_LINE_SIZE` | 256 | Max characters per line (TEXT mode) |
| `DB_MAX_NAME_SIZE` | 32 | Max characters in a variable name (BINARY mode) |
| `DB_MAX_STRING_SIZE` | 64 | Max characters in a stored string (BINARY mode) |
| `DB_FILE_COPY_BUFFER_SIZE` | 1024 | Internal buffer size used when removing entries (TEXT mode) |

---

## Types

### `DBtype`
```c
typedef enum {
  BINARY = 'b',
  TEXT   = '\0'
} DBtype;
```
Selects the storage format. `TEXT` produces a human-readable `key=value` file. `BINARY` stores fixed-size records and is faster for frequent reads and writes.

### `Database`
```c
typedef struct {
  FILE*       file;
  const char* fileName;
  DBtype      type;
} Database;
```
Represents a database instance. `file` is only open during an active operation; it is `NULL` the rest of the time.

---

## Basic Usage

```c
#include <database/database.h>
#include <math.h>
#include <stdio.h>

int main() {
  // Create a binary database (file is created automatically on first write)
  Database db = createDatabase("save.dat", BINARY);

  // Write some values
  databaseSetInt(&db, "score", 4200);
  databaseSetFloat(&db, "posX", 3.14f);
  databaseSetString(&db, "playerName", "Hero");
  databaseSetChar(&db, "grade", 'A');

  // Read them back
  int   score = databaseGetInt(&db, "score");
  float posX  = databaseGetFloat(&db, "posX");

  char name[64];
  databaseGetString(&db, name, "playerName");

  char grade = databaseGetChar(&db, "grade");

  printf("Score: %d  posX: %.2f  name: %s  grade: %c\n",
         score, posX, name, grade);

  // Setting a key that already exists replaces the old value
  databaseSetInt(&db, "score", 9999);

  // Remove a variable entirely
  databaseRemoveVar(&db, "grade");

  // Check if a float value was found (returns NAN when missing)
  float missing = databaseGetFloat(&db, "notHere");
  if(isnan(missing))
    printf("Variable not found.\n");

  databaseDelete(&db);
  return 0;
}
```

### TEXT mode
The TEXT format is identical in API but stores a plain-text file you can open in any editor:
```
score=9999
posX=3.140000
playerName=Hero
```
Use `TEXT` when human readability or external editing of the file matters.

---

## Function Reference

### Lifecycle

#### `createDatabase`
```c
Database createDatabase(const char* fileName, DBtype type);
```
Creates and returns a `Database` struct pointing at `fileName`. Does not open or create the file — the file is created automatically on the first write. `type` is either `BINARY` or `TEXT`.

#### `databaseDelete`
```c
void databaseDelete(Database* database);
```
Closes the file if it happens to be open and clears internal state. Does not delete the file on disk.

#### `databaseOpen` / `databaseClose`
```c
void databaseOpen(Database* database, const char* mode);
void databaseClose(Database* database);
```
Manually open or close the underlying file. Normally you do not need to call these — every `Get`, `Set`, and `Remove` function opens and closes the file automatically.

---

### Writing Values

Every setter removes any existing entry with the same name before writing the new one, so values are always up to date with no duplicates.

#### `databaseSetInt`
```c
void databaseSetInt(Database* database, const char* varName, int value);
```
Stores an `int` under `varName`.

#### `databaseSetFloat`
```c
void databaseSetFloat(Database* database, const char* varName, float value);
```
Stores a `float` under `varName`.

#### `databaseSetString`
```c
void databaseSetString(Database* database, const char* varName, const char* value);
```
Stores a null-terminated string under `varName`. Maximum length is `DB_MAX_STRING_SIZE` (64) in BINARY mode and `DB_MAX_LINE_SIZE` (256) in TEXT mode.

#### `databaseSetChar`
```c
void databaseSetChar(Database* database, const char* varName, char value);
```
Stores a single `char` under `varName`.

---

### Reading Values

#### `databaseGetInt`
```c
int databaseGetInt(Database* database, const char* varName);
```
Returns the stored `int`. Returns `NAN` cast to `int` if the variable is not found.

#### `databaseGetFloat`
```c
float databaseGetFloat(Database* database, const char* varName);
```
Returns the stored `float`. Returns `NAN` if the variable is not found. Use `isnan()` to check for a missing variable.

#### `databaseGetString`
```c
void databaseGetString(Database* database, char* outputBuffer, const char* varName);
```
Writes the stored string into `outputBuffer`. If the variable is not found, sets `outputBuffer[0]` to `'\n'` (TEXT mode) or leaves the buffer unchanged (BINARY mode).

#### `databaseGetChar`
```c
char databaseGetChar(Database* database, const char* varName);
```
Returns the stored `char`. Returns `'\0'` if not found (BINARY mode) or `'\n'` if not found (TEXT mode).

---

### Removing Values

#### `databaseRemoveVar`
```c
void databaseRemoveVar(Database* database, const char* varName);
```
Removes the entry with the given name from the file. If the variable does not exist, this is a no-op. All setter functions call this internally before writing, so you rarely need to call it directly.

---

### Lookup

#### `databaseGetVarLocation`
```c
long databaseGetVarLocation(Database* database, const char* varName);
```
Returns the byte offset (TEXT mode) or record index (BINARY mode) of `varName` in the file, or `-1` if not found. Primarily for internal use.

---

## TEXT vs BINARY — When to Use Which

| | TEXT | BINARY |
|---|---|---|
| File readable by humans | Yes | No |
| Performance | Slower (string parsing) | Faster (fixed-size records) |
| Variable name limit | 256 chars per line | 32 chars |
| String value limit | 256 chars | 64 chars |
| Good for | Config files, debug saves | Game saves, frequent I/O |
