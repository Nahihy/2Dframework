#include <database/database.h>
#include <stdio.h>
#include <string.h>
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
  char line[DB_MAX_LINE_SIZE];
  char extractedName[DB_MAX_LINE_SIZE];

  while(fgets(line, sizeof(line), database->file)) {
    char formatStr[20];
    snprintf(formatStr, sizeof(formatStr), "%%%d[^=]", (int)sizeof(extractedName) - 1);
    sscanf(line, formatStr, extractedName);
    
    if(!strcmp(extractedName, varName)) {
      fseek(database->file, -strlen(line), SEEK_CUR);
      long location = ftell(database->file);
      databaseClose(database);
      return location;
    }
  }
  printf("Warning: could not find var \"%s\" in database \"%s\"", varName, database->fileName);
  databaseClose(database);
  return -1;
}

void databaseSetInt(Database* database, const char* varName, int value) {
  databaseRemoveVar(database, varName);
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%d\n", varName, value);
  databaseClose(database);
}
void databaseSetFloat(Database* database, const char* varName, float value) {
  databaseRemoveVar(database, varName);
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%f\n", varName, value);
  databaseClose(database);
}
void databaseSetString(Database* database, const char* varName, const char* value) {
  databaseRemoveVar(database, varName);
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%s\n", varName, value);
  databaseClose(database);
}
void databaseSetChar(Database* database, const char* varName, char value) {
  databaseRemoveVar(database, varName);
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%c\n", varName, value);
  databaseClose(database);
}

void databaseRemoveVar(Database* database, const char* varName) {
  long location = databaseGetVarLocation(database, varName);
  
  if (location == -1) 
    return;
  
  
  FILE* temp = fopen("temp.txt", "w");
  if (temp == NULL) {
    return;
  }
  
  databaseOpen(database, "r");
  
  char buffer[DB_FILE_COPY_BUFFER_SIZE];
  long bytesLeft = location;
  
  while (bytesLeft > 0) {
    size_t chunkSize = (bytesLeft > sizeof(buffer)) ? sizeof(buffer) : bytesLeft;
    size_t bytesRead = fread(buffer, 1, chunkSize, database->file);
    
    if (bytesRead == 0) break;
    
    fwrite(buffer, 1, bytesRead, temp);
    bytesLeft -= bytesRead;
  }
  
  char line[DB_MAX_LINE_SIZE];
  fgets(line, sizeof(line), database->file);
  
  size_t bytesRead;
  while ((bytesRead = fread(buffer, 1, sizeof(buffer), database->file)) > 0) {
    fwrite(buffer, 1, bytesRead, temp);
  }
  
  databaseClose(database);
  fclose(temp);
  
  remove(database->fileName);
  rename("temp.txt", database->fileName);
}

int databaseGetInt(Database* database, const char* varName) {
  char stringBuffer[DB_MAX_LINE_SIZE];
  databaseGetString(database, stringBuffer, varName);
  return atoi(stringBuffer);
}
float databaseGetFloat(Database* database, const char* varName) {
  char stringBuffer[DB_MAX_LINE_SIZE];
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
  char stringBuffer[DB_MAX_LINE_SIZE];
  databaseGetString(database, stringBuffer, varName);
  return stringBuffer[0];
}

