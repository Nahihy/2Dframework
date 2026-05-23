#pragma once
#include <stdio.h>

typedef struct {
  FILE* file;
  const char* fileName;
} Database;


Database createDatabase(const char* fileName);
void databaseDelete(Database* database);
void databaseOpen(Database* database, const char* mode);
void databaseClose(Database* database);
int databaseGetInt(Database* database, const char* varName);
float databaseGetFloat(Database* database, const char* varName);
void databaseGetString(Database* database, char* outputBuffer, const char* varName);
char databaseGetChar(Database* database, const char* varName);
long databaseGetVarLocation(Database* database, const char* varName); 
void databaseAddInt(Database* database, const char* varName, int value);
void databaseAddFloat(Database* database, const char* varName, float value);
void databaseAddString(Database* database, const char* varName, const char* value);
void databaseAddChar(Database* database, const char* varName, char value);
