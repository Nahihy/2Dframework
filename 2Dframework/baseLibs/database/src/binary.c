#include <database/database.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


typedef struct {
  char name[DB_MAX_NAME_SIZE];
  union {
    int i;
    float f;
    char s[DB_MAX_STRING_SIZE];
    char c;
  } value;
} Record;


int databaseGetIntBINARY(Database* database, const char* varName) {
  long index = databaseGetVarLocationBINARY(database, varName);
  if(index == -1) return NAN;
  databaseOpen(database, "r");
  Record record;
  fseek(database->file, index * sizeof(Record), SEEK_SET);
  fread(&record, sizeof(Record), 1, database->file);
  databaseClose(database);
  return record.value.i;
}

float databaseGetFloatBINARY(Database* database, const char* varName) {
  long index = databaseGetVarLocationBINARY(database, varName);
  if(index == -1) return NAN;
  databaseOpen(database, "r");
  Record record;
  fseek(database->file, index * sizeof(Record), SEEK_SET);
  fread(&record, sizeof(Record), 1, database->file);
  databaseClose(database);
  return record.value.f;
}

void databaseGetStringBINARY(Database* database, char* outputBuffer, const char* varName) {
  long index = databaseGetVarLocationBINARY(database, varName);
  if(index == -1) return;
  databaseOpen(database, "r");
  Record record;
  fseek(database->file, index * sizeof(Record), SEEK_SET);
  fread(&record, sizeof(Record), 1, database->file);
  strcpy(outputBuffer, record.value.s);
  databaseClose(database);
}

char databaseGetCharBINARY(Database* database, const char* varName) {
  long index = databaseGetVarLocationBINARY(database, varName);
  if(index == -1) return '\0';
  databaseOpen(database, "r");
  Record record;
  fseek(database->file, index * sizeof(Record), SEEK_SET);
  fread(&record, sizeof(Record), 1, database->file);
  databaseClose(database);
  return record.value.c;
}

long databaseGetVarLocationBINARY(Database* database, const char* varName) {
  databaseOpen(database, "r");
  if(database->file == NULL) return -1;
  Record record;
  int index = 0;
  while(fread(&record, sizeof(Record), 1, database->file) == 1) {
    if(!strcmp(record.name, varName)) {
      databaseClose(database);
      return index;
    }
    index++;
  }
  databaseClose(database);
  return -1;
}

void databaseSetIntBINARY(Database* database, const char* varName, int value) {
  databaseRemoveVarBINARY(database, varName);
  databaseOpen(database, "a");
  Record record;
  strncpy(record.name, varName, DB_MAX_NAME_SIZE);
  record.value.i = value;
  fwrite(&record, sizeof(Record), 1, database->file);
  databaseClose(database);
}

void databaseSetFloatBINARY(Database* database, const char* varName, float value) {
  databaseRemoveVarBINARY(database, varName);
  databaseOpen(database, "a");
  Record record;
  strncpy(record.name, varName, DB_MAX_NAME_SIZE);
  record.value.f = value;
  fwrite(&record, sizeof(Record), 1, database->file);
  databaseClose(database);
}

void databaseSetStringBINARY(Database* database, const char* varName, const char* value) {
  databaseRemoveVarBINARY(database, varName);
  databaseOpen(database, "a");
  Record record;
  strncpy(record.name, varName, DB_MAX_NAME_SIZE);
  strncpy(record.value.s, value, DB_MAX_STRING_SIZE);
  fwrite(&record, sizeof(Record), 1, database->file);
  databaseClose(database);
}

void databaseSetCharBINARY(Database* database, const char* varName, char value) {
  databaseRemoveVarBINARY(database, varName);
  databaseOpen(database, "a");
  Record record;
  strncpy(record.name, varName, DB_MAX_NAME_SIZE);
  record.value.c = value;
  fwrite(&record, sizeof(Record), 1, database->file);
  databaseClose(database);
}

void databaseRemoveVarBINARY(Database* database, const char* varName) {
  int index = databaseGetVarLocationBINARY(database, varName);
  if (index == -1) return;

  databaseOpen(database, "r+");
  if (database->file == NULL) return;

  fseek(database->file, (index + 1) * sizeof(Record), SEEK_SET);

  Record record;
  while (fread(&record, sizeof(Record), 1, database->file) == 1) {
    fseek(database->file, -2 * (long)sizeof(Record), SEEK_CUR);
    fwrite(&record, sizeof(Record), 1, database->file);
    fseek(database->file, sizeof(Record), SEEK_CUR);
  }

  long newSize = (index) * sizeof(Record);
  ftruncate(fileno(database->file), newSize);
  databaseClose(database);
}
