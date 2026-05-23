#include <database/database.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

Database createDatabase(const char* fileName) {
  Database database;
  database.fileName = fileName;

  return database;
}

void databaseOpen(Database* database, const char* mode) {
  database->file = fopen(database->fileName, mode);
}

void databaseClose(Database* database) {
  fclose(database->file);
}

void databaseDelete(Database* database) {
  databaseClose(database);
  database->fileName = NULL;
}

long databaseGetVarLocation(Database* database, const char* varName) {
  databaseOpen(database, "r"); 
  char line[512];
  char extractedName[510];

  while(fgets(line, sizeof(line), database->file)) {
    sscanf(line, "%[^=]", extractedName);
    
    if(!strcmp(extractedName, varName)) {
      databaseClose(database);
      return ftell(database->file);
    }
  }
  printf("Warning: could not find var \"%s\" in database \"%s\"", varName, database->fileName);
  databaseClose(database);
  return NAN;
}

void databaseAddInt(Database* database, const char* varName, int value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%d\n", varName, value);
  databaseClose(database);
}
void databaseAddFloat(Database* database, const char* varName, float value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%f\n", varName, value);
  databaseClose(database);
}
void databaseAddString(Database* database, const char* varName, const char* value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%s\n", varName, value);
  databaseClose(database);
}
void databaseAddChar(Database* database, const char* varName, char value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%c\n", varName, value);
  databaseClose(database);
}


int databaseGetInt(Database* database, const char* varName) {
  char* stringBuffer;
  databaseGetString(database, stringBuffer, varName);
  return atoi(stringBuffer);
}
float databaseGetFloat(Database* database, const char* varName) {
  char* stringBuffer;
  databaseGetString(database, stringBuffer, varName);
  return (float)atof(stringBuffer);
}
void databaseGetString(Database* database, char* outputBuffer, const char* varName) {
  long lineLocation = databaseGetVarLocation(database, varName);
  char line[512];
  databaseOpen(database, "r");

  fseek(database->file, lineLocation, SEEK_SET);
  fgets(line, sizeof(line), database->file);
  sscanf(line, "%*[^=]=%s\n", outputBuffer);

  databaseClose(database);
}
char databaseGetChar(Database* database, const char* varName) {
  char* stringBuffer;
  databaseGetString(database, stringBuffer, varName);
  return stringBuffer[0];
}

