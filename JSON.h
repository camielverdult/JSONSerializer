
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
    memset(dictionary->key, '\0', STRING_LENGTH);
    memset(dictionary->value, '\0', STRING_LENGTH);
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
    dictionary->capacity = new_capacity;
}

uint8_t JSON_String_Valid(const char* string) {
    // This function checks if the passed string has a null-terminator character
    // before reaching the max string length allowed
    uint8_t i = 0;

    while (string[i] != '\0') {
        i++;
        if (i == STRING_LENGTH) {
            // This string is too long or has a broken null-terminator
            return 0;
        }
    }

    return 1;
}

void JSON_Set_String(JSONDictionary* dictionary, const char* key, const char* value) {

    if (!JSON_String_Valid(key) || !JSON_String_Valid(value)) {
        // Passed key or value is broken and unsafe to use, abort
        return;
    }

	// Check if key already exists
	for (uint8_t i = 0; i < dictionary->size; i++) {
		if (strncmp(dictionary->entries[i].key, key, STRING_LENGTH) == 0) {

            if (strncmp(dictionary->entries[i].value, value, STRING_LENGTH) == 0) {
                return;
            }

			// Key exists, update value
			strncpy((char*)&dictionary->entries[i].value, value, STRING_LENGTH);

			return;
		}
	}

    JSON_Check(dictionary, dictionary->size + 1);

    JSON_Entry_Init(&dictionary->entries[dictionary->size]);
	
	// Key does not exist here, add it
    strncpy((char*)&dictionary->entries[dictionary->size].key, key, STRING_LENGTH);

	//dictionary->entries[dictionary->size].value = value;
    strncpy((char*)&dictionary->entries[dictionary->size].value, value, STRING_LENGTH);

	dictionary->size += 1;
}

void JSON_Set_Float(JSONDictionary* dictionary, const char* key, float value) {

    if (!JSON_String_Valid(key)) {
        // Passed key or value is broken and unsafe to use, abort
        return;
    }

    char buffer[STRING_LENGTH];
    memset(buffer, '\0', STRING_LENGTH);

    // Buffer length decides amount of decimals here
    snprintf(buffer, STRING_LENGTH, "%f", value);

	JSON_Set_String(dictionary, key, buffer);
}

void JSON_Set_Integer(JSONDictionary* dictionary, const char* key, uint16_t value) {

    if (!JSON_String_Valid(key)) {
        // Passed key or value is broken and unsafe to use, abort
        return;
    }

	char buffer[STRING_LENGTH];
    memset(buffer, '\0', STRING_LENGTH);

    snprintf(buffer, STRING_LENGTH, "%d", value);
	JSON_Set_String(dictionary, key, buffer);
}

void JSON_Serialize_Entry(JSONEntry* entry, char* buffer, uint8_t buffer_length) {
    memset(buffer, '\0', buffer_length);
	snprintf(buffer, buffer_length, "\"%s\": \"%s\"", entry->key, entry->value);
}

void JSON_Serialize_Dictionary(JSONDictionary* dictionary, char* buffer, uint16_t buffer_length) {

    // Fill the entire buffer with null terminator characters so that strlen is
    // less obliged to not find a null terminator string, strncpy will write these
    // over anyway while building the dictionary
    memset(buffer, '\0', buffer_length);

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
