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


