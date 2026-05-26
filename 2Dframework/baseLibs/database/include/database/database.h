#pragma once
#include <stdio.h>

#define DB_MAX_LINE_SIZE 256
#define DB_FILE_COPY_BUFFER_SIZE 1028


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
void databaseSetInt(Database* database, const char* varName, int value);
void databaseSetFloat(Database* database, const char* varName, float value);
void databaseSetString(Database* database, const char* varName, const char* value);
void databaseSetChar(Database* database, const char* varName, char value);
void databaseRemoveVar(Database* database, const char* varName);
