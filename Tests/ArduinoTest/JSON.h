
/*
 * JSON.h
 *
 * Created: 4/3/2022 3:04:52 PM
 *  Author: Camiel Verdult
 */ 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//const uint8_t CAPACITY = 20;
//const uint8_t GROWTH = 3;
//const uint8_t STRING_LENGTH = 15;
//const uint8_t ENTRY_BUFF_LENGTH = STRING_LENGTH * 2.5f;

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
	uint8_t capacity;
	uint8_t size;
} JSONDictionary;

void JSON_Dictionary_Init(JSONDictionary* dictionary) {
	// Allocate a json entry array on the heap
	dictionary->entries = (JSONEntry*)malloc(sizeof(JSONEntry) * CAPACITY);
	dictionary->capacity = CAPACITY;
	dictionary->size = 0;
}

void JSON_Entry_Init(JSONEntry* dictionary) {
    // Allocate a json entry array on the heap
    memset(dictionary->key, 0, STRING_LENGTH);
    memset(dictionary->value, 0, STRING_LENGTH);
}

void JSON_Check(JSONDictionary* dictionary, uint8_t new_length) {
	
	// Check if current length + 1 overflows the array
	if (new_length < dictionary->capacity) {
		// Capacity is fine for now, return
		return;
	}

    printf("Growing array!\n");
	
	uint16_t new_capacity = dictionary->capacity * GROWTH;

	dictionary->entries = (JSONEntry*)realloc(dictionary->entries, sizeof(JSONEntry) * new_capacity);
}

void JSON_Set_String(JSONDictionary* dictionary, const char* key, const char* value) {
	
	JSON_Check(dictionary, dictionary->size + 1);

	// Check if key already exists
	for (uint8_t i = 0; i < dictionary->size; i++) {
		if (strcmp(dictionary->entries[i].key, key) == 0) {

            if (strcmp(dictionary->entries[i].value, value) == 0) {
                return;
            }

			// Key exists, update value
			strcpy((char*)&dictionary->entries[i].value, value);

			return;
		}
	}

    JSON_Entry_Init(&dictionary->entries[dictionary->size]);
	
	// Key does not exist here, add it
    strcpy((char*)&dictionary->entries[dictionary->size].key, key);

	//dictionary->entries[dictionary->size].value = value;
    strcpy((char*)&dictionary->entries[dictionary->size].value, value);

	dictionary->size += 1;
}

void JSON_Set_Float(JSONDictionary* dictionary, const char* key, float value) {
    char buffer[STRING_LENGTH];
    memset(buffer, 0, STRING_LENGTH);

    snprintf(buffer, STRING_LENGTH, "%f", value);

	JSON_Set_String(dictionary, key, buffer);
}

void JSON_Set_Integer(JSONDictionary* dictionary, const char* key, uint16_t value) {
	char buffer[STRING_LENGTH];
    memset(buffer, 0, STRING_LENGTH);

    snprintf(buffer, STRING_LENGTH, "%d", value);
	JSON_Set_String(dictionary, key, buffer);
}

void JSON_Serialize_Entry(JSONEntry* entry, char* buffer, uint8_t buffer_length) {
    memset(buffer, 0, buffer_length);
	snprintf(buffer, buffer_length, "\"%s\": \"%s\"", entry->key, entry->value);
}

void JSON_Serialize_Dictionary(JSONDictionary* dictionary, char* buffer, uint16_t buffer_length) {

    memset(buffer, 0, buffer_length);

	uint32_t buffer_index = 0;
		
	// Add each entry as dictionary
	for (uint8_t i = 0; i < dictionary->size; i++) {

		char entry_buffer[ENTRY_BUFF_LENGTH];

        // Call helper function to convert each entry to string
        JSON_Serialize_Entry(&dictionary->entries[i], entry_buffer, ENTRY_BUFF_LENGTH);

        if (dictionary->size == 1) {
            // Serializing single entry only
            snprintf(&buffer[buffer_index], ENTRY_BUFF_LENGTH, "{ %s }", entry_buffer);
        } else if (i == 0) {
            // Serializing multiple entries; start of dictionary
			snprintf(&buffer[buffer_index], ENTRY_BUFF_LENGTH, "{ %s, ", entry_buffer);
		} else if (i < (dictionary->size - 1)) {
            // Serializing multiple entries; middle of dictionary
			snprintf(&buffer[buffer_index], ENTRY_BUFF_LENGTH, "%s, ", entry_buffer);
		} else if (i == (dictionary->size - 1)) {
            // Serializing multiple entries; end dictionary
			snprintf(&buffer[buffer_index], ENTRY_BUFF_LENGTH, "%s }", entry_buffer);
		}

        buffer_index = strlen(buffer);
    }
}

void JSON_Cleanup(JSONDictionary* dictionary) {
	free(dictionary->entries);
}
