#pragma once
#include <stdio.h>

#define DB_MAX_LINE_SIZE 256
#define DB_FILE_COPY_BUFFER_SIZE 1024

typedef enum {
  BINARY = 'b',
  TEXT = '\0'
} DBtype;

typedef struct {
  FILE* file;
  const char* fileName;
  DBtype type;
} Database;


Database createDatabase(const char* fileName, DBtype type);
void databaseDelete(Database* database);
void databaseOpen(Database* database, const char* mode);
void databaseClose(Database* database);
int databaseGetInt(Database* database, const char* varName);
float databaseGetFloat(Database* database, const char* varName);
void databaseGetString(Database* database, char* outputBuffer, const char* varName);
char databaseGetChar(Database* database, const char* varName);
long databaseGetVarLocation(Database* database, const char* varName); 
void databaseSetInt(Database* database, const char* varName, int value);
void databaseSetFloat(Database* database, const char* varName, float value);
void databaseSetString(Database* database, const char* varName, const char* value);
void databaseSetChar(Database* database, const char* varName, char value);
void databaseRemoveVar(Database* database, const char* varName);


int databaseGetIntTEXT(Database* database, const char* varName);
float databaseGetFloatTEXT(Database* database, const char* varName);
void databaseGetStringTEXT(Database* database, char* outputBuffer, const char* varName);
char databaseGetCharTEXT(Database* database, const char* varName);
long databaseGetVarLocationTEXT(Database* database, const char* varName); 
void databaseSetIntTEXT(Database* database, const char* varName, int value);
void databaseSetFloatTEXT(Database* database, const char* varName, float value);
void databaseSetStringTEXT(Database* database, const char* varName, const char* value);
void databaseSetCharTEXT(Database* database, const char* varName, char value);
void databaseRemoveVarTEXT(Database* database, const char* varName);


int databaseGetIntBINARY(Database* database, const char* varName);
float databaseGetFloatBINARY(Database* database, const char* varName);
void databaseGetStringBINARY(Database* database, char* outputBuffer, const char* varName);
char databaseGetCharBINARY(Database* database, const char* varName);
long databaseGetVarLocationBINARY(Database* database, const char* varName); 
void databaseSetIntBINARY(Database* database, const char* varName, int value);
void databaseSetFloatBINARY(Database* database, const char* varName, float value);
void databaseSetStringBINARY(Database* database, const char* varName, const char* value);
void databaseSetCharBINARY(Database* database, const char* varName, char value);
void databaseRemoveVarBINARY(Database* database, const char* varName);

