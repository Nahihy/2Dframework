#include <database/database.h>
#include <stdio.h>


Database createDatabase(const char* fileName, DBtype type) {
  Database database;
  database.fileName = fileName;
  database.file = NULL;
  database.type = type;
  return database;
}

void databaseOpen(Database* database, const char* mode) {
  char finalMode[5];
  sprintf(finalMode, "%s%c", mode, database->type);
  database->file = fopen(database->fileName, finalMode);
}

void databaseClose(Database* database) {
  if (database->file != NULL) {
    fclose(database->file);
    database->file = NULL;
  }
}

void databaseDelete(Database* database) {
  databaseClose(database);
  database->fileName = NULL;
}

long databaseGetVarLocation(Database* database, const char* varName) {
  return database->type == TEXT ?
  databaseGetVarLocationTEXT(database, varName) : databaseGetVarLocationBINARY(database, varName);
}

void databaseSetInt(Database* database, const char* varName, int value) {
  if(database->type == TEXT)
    databaseSetIntTEXT(database, varName, value);
  else
    databaseSetIntBINARY(database, varName, value);
}

void databaseSetFloat(Database* database, const char* varName, float value) {
  if(database->type == TEXT)
    databaseSetFloatTEXT(database, varName, value);
  else
    databaseSetFloatBINARY(database, varName, value);
}

void databaseSetString(Database* database, const char* varName, const char* value) {
  if(database->type == TEXT)
    databaseSetStringTEXT(database, varName, value);
  else
    databaseSetStringBINARY(database, varName, value);
}

void databaseSetChar(Database* database, const char* varName, char value) {
  if(database->type == TEXT)
    databaseSetCharTEXT(database, varName, value);
  else
    databaseSetCharBINARY(database, varName, value);
}

void databaseRemoveVar(Database* database, const char* varName) {
  if(database->type == TEXT)
    databaseRemoveVarTEXT(database, varName);
  else
    databaseRemoveVarBINARY(database, varName);
}

int databaseGetInt(Database* database, const char* varName) {
  return database->type == TEXT ?
    databaseGetIntTEXT(database, varName) : databaseGetIntBINARY(database, varName);
}

float databaseGetFloat(Database* database, const char* varName) {
  return database->type == TEXT ?
    databaseGetFloatTEXT(database, varName) : databaseGetFloatBINARY(database, varName);
}

void databaseGetString(Database* database, char* outputBuffer, const char* varName) {
  if(database->type == TEXT)
    databaseGetStringTEXT(database, outputBuffer, varName);
  else
    databaseGetStringBINARY(database, outputBuffer, varName);
}

