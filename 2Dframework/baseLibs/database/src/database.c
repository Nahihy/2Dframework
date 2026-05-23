#include <database/database.h>
#include <string.h>
#include <math.h>

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
  char extractedName[128];

  while(fgets(line, sizeof(line), database->file)) {
    sscanf(line, "%[^=]", extractedName);
    
    if(!strcmp(extractedName, varName)) {
      databaseClose(database);
      return ftell(database->file);
    }
  }
  printf("Warning: could not find var \"%s\" in database \"%s\"", varName, database->fileName);
  return NAN;
}

void databaseAddInt(Database* database, const char* varName, int value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%d\n", varName, value);
}
void databaseAddFloat(Database* database, const char* varName, float value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%\n", varName, value);
}
void databaseAddString(Database* database, const char* varName, const char* value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%s\n", varName, value);
}
void databaseAddChar(Database* database, const char* varName, char value) {
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%c\n", varName, value);
}


int databaseGetInt(Database* database, const char* varName) {

}
float databaseGetFloat(Database* database, const char* varName);
char* databaseGetString(Database* database, const char* varName);
char databaseGetChar(Database* database, const char* varName);

