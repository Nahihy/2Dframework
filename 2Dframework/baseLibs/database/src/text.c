#include <database/database.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


long databaseGetVarLocationTEXT(Database* database, const char* varName) {
  databaseOpen(database, "r"); 
  if(database->file == NULL) return -1;
  char line[DB_MAX_LINE_SIZE];
  char extractedName[DB_MAX_LINE_SIZE];
  extractedName[0] = '\0';
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
    extractedName[0] = '\0';
  }
  printf("Warning: could not find var \"%s\" in database \"%s\"", varName, database->fileName);
  databaseClose(database);
  return -1;
}

void databaseSetIntTEXT(Database* database, const char* varName, int value) {
  databaseRemoveVarTEXT(database, varName);
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%d\n", varName, value);
  databaseClose(database);
}
void databaseSetFloatTEXT(Database* database, const char* varName, float value) {
  databaseRemoveVarTEXT(database, varName);
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%f\n", varName, value);
  databaseClose(database);
}
void databaseSetStringTEXT(Database* database, const char* varName, const char* value) {
  databaseRemoveVarTEXT(database, varName);
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%s\n", varName, value);
  databaseClose(database);
}
void databaseSetCharTEXT(Database* database, const char* varName, char value) {
  databaseRemoveVarTEXT(database, varName);
  databaseOpen(database, "a");
  fprintf(database->file, "%s=%c\n", varName, value);
  databaseClose(database);
}

void databaseRemoveVarTEXT(Database* database, const char* varName) {
  long location = databaseGetVarLocationTEXT(database, varName);
  
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

int databaseGetIntTEXT(Database* database, const char* varName) {
  char stringBuffer[DB_MAX_LINE_SIZE];
  databaseGetStringTEXT(database, stringBuffer, varName);
  if(stringBuffer[0] == '\n') return NAN;
  return atoi(stringBuffer);
}
float databaseGetFloatTEXT(Database* database, const char* varName) {
  char stringBuffer[DB_MAX_LINE_SIZE];
  databaseGetStringTEXT(database, stringBuffer, varName);
  if(stringBuffer[0] == '\n') return NAN;
  return (float)atof(stringBuffer);
}

void databaseGetStringTEXT(Database* database, char* outputBuffer, const char* varName) {
  long lineLocation = databaseGetVarLocationTEXT(database, varName);
  if(lineLocation == -1) {
    outputBuffer[0] = '\n';
    return;
  }
  char line[512];
  databaseOpen(database, "r");

  fseek(database->file, lineLocation, SEEK_SET);
  fgets(line, sizeof(line), database->file);
  sscanf(line, "%*[^=]=%s\n", outputBuffer);

  databaseClose(database);
}
char databaseGetCharTEXT(Database* database, const char* varName) {
  char stringBuffer[DB_MAX_LINE_SIZE];
  databaseGetStringTEXT(database, stringBuffer, varName);
  return stringBuffer[0];
}

