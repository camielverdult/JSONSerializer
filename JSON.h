
/*
 * JSON.h
 *
 * Created: 4/3/2022 3:04:52 PM
 *  Author: Camiel Verdult
 */ 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define CAPACITY 20
#define GROWTH 3
#define STRING_LENGTH 15
#define ENTRY_BUFF_LENGTH ((STRING_LENGTH * 2) + 10)

typedef struct {
	char key[STRING_LENGTH];
	char value[STRING_LENGTH];
} JSONEntry;

typedef struct {
	JSONEntry* entries;
	int capacity;
	int size;
} JSONDictionary;

void JSON_Dictionary_Init(JSONDictionary* dictionary);

void JSON_Entry_Init(JSONEntry* dictionary);

void JSON_Check(JSONDictionary* dictionary, int new_length);

int JSON_String_Valid(const char* string);

void JSON_Set_String(JSONDictionary* dictionary, const char* key, const char* value);

void JSON_Set_Float(JSONDictionary* dictionary, const char* key, float value);

void JSON_Set_Integer(JSONDictionary* dictionary, const char* key, int value);

void JSON_Serialize_Entry(JSONEntry* entry, char* buffer, int buffer_length);

void JSON_Serialize_Dictionary(JSONDictionary* dictionary, char* buffer, int buffer_length);

void JSON_Cleanup(JSONDictionary* dictionary);
