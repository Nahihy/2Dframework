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
char* databaseGetString(Database* database, const char* varName);
char databaseGetChar(Database* database, const char* varName);
int databaseGetVarLocation(Database* database, const char* varName); 
void databaseSetInt(Database* database, const char* varName, int value);
void databaseSetFloat(Database* database, const char* varName, float value);
void databaseSetString(Database* database, const char* varName, const char* value);
void databaseSetChar(Database* database, const char* varName, char value);
