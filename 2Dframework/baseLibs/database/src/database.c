#include <database/database.h>
#include <string.h>

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

int databaseGetVarLocation(Database* database, const char* varName) {
  if(database->file == NULL) databaseOpen(database, "r"); 
  char line[256];
  char extractedName[128];

  int lineCounter = 0;
  while(fgets(line, sizeof(line), database->file)) {
    sscanf(line, "%[^=]", extractedName);
    
    if(strcmp(extractedName, varName)) {
      databaseClose(database);
      return lineCounter;
    }
    lineCounter++;
  }
  printf("Warning: could not find var \"%s\" in database \"%s\"", varName, database->fileName);
  return -1;
}

void databaseSetInt(Database* database, const char* varName, int value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%d", varName, value);
}
void databaseSetFloat(Database* database, const char* varName, float value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%f", varName, value);
}
void databaseSetString(Database* database, const char* varName, const char* value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%s", varName, value);
}
void databaseSetChar(Database* database, const char* varName, char value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%c", varName, value);
}
