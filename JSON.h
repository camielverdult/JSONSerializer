
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

typedef struct {
	char key[20];
	char value[20];
	uint8_t key_length;
	uint8_t value_length;
} JSONEntry;

typedef struct {
	JSONEntry* entries;
	uint8_t capacity;
	uint8_t size;
	uint16_t text_length;
} JSONDictionary;

JSONDictionary* JSON_New() {
	
	// Allocate a json entry array on the heap
	JSONDictionary* dictionary = malloc(sizeof(JSONDictionary));
	
	dictionary->entries = malloc(sizeof(JSONEntry) * CAPACITY);
	dictionary->capacity = CAPACITY;
	dictionary->size = 0;
	
//	// Initialize all key value pairs to be empty
// 	for (uint8_t i = 0; i < dictionary->capacity; i++) {
//// 		dictionary->entries[i].key = "";
//        memcpy(dictionary->entries[i].key, "", 1);
//
//// 		dictionary->entries[i].value = "";
//        memcpy(dictionary->entries[i].value, "", 1);
//
// 		dictionary->entries[i].key_length = 0;
// 		dictionary->entries[i].value_length = 0;
// 	}
	
	return dictionary;
}

void JSON_Check(JSONDictionary* dictionary, uint8_t new_length) {
	
	// Check if current length + 1 overflows the array
	if (dictionary->size + 1 < dictionary->capacity) {
		// Capacity is fine for now, return
		return;
	}
	
	uint16_t new_capacity = dictionary->capacity * GROWTH;

	dictionary->entries = realloc(dictionary->entries, sizeof(JSONEntry) * new_capacity);
}

void JSON_Set_String(JSONDictionary* dictionary, const char* key, const char* value) {
	
	JSON_Check(dictionary, dictionary->size + 1);

	// Check if key already exists
	for (uint8_t i = 0; i < dictionary->size; i++) {
		if (strcmp(dictionary->entries[i].key, key) == 0) {

			// Remove length of previous value
			dictionary->text_length -= strlen(dictionary->entries[i].value);

			// Key exists, update value
			memcpy(dictionary->entries[i].value, value, strlen(value));
			dictionary->entries[i].value_length = strlen(value);

			dictionary->text_length += strlen(value);
			return;
		}
	}
	
	// Key does not exist here, add it
	// dictionary->entries[dictionary->size].key = key;
	memcpy(&dictionary->entries[dictionary->size].key, key, strlen(key));
	
	dictionary->entries[dictionary->size].key_length = strlen(key);
	dictionary->text_length += strlen(key);

	//dictionary->entries[dictionary->size].value = value;
	memcpy(&dictionary->entries[dictionary->size].value, value, strlen(value));
	
	dictionary->entries[dictionary->size].value_length = strlen(value);
	dictionary->text_length += strlen(value);

	dictionary->size++;
}

void JSON_Set_Float(JSONDictionary* dictionary, const char* key, float value) {
	char buffer[10];

    snprintf(buffer, 10, "%f", value);

	JSON_Set_String(dictionary, key, buffer);
}

void JSON_Set_Integer(JSONDictionary* dictionary, const char* key, uint16_t value) {
	char buffer[10];

    snprintf(buffer, 10, "%d", value);
	
	JSON_Set_String(dictionary, key, buffer);
}

uint16_t JSON_Serialize_Entry(JSONEntry* entry, char* buffer) {
	
	uint16_t buffer_index = 0;
	
	// Opening " for key
	buffer[buffer_index++] = '"';
	
	// Add key
	for (uint8_t i = 0; i < strlen(entry->key); i++) {
		buffer[buffer_index++] = entry->key[i];
	}
	
	// Closing " for key
	buffer[buffer_index++] = '"';
	
	// Key value indication :
	buffer[buffer_index++] = ':';
	
	// Add space
	buffer[buffer_index++] = ' ';
	
	// Opening " for value
	buffer[buffer_index++] = '"';
	
	// Add value
	for (uint8_t i = 0; i < strlen(entry->value); i++) {
		buffer[buffer_index++] = entry->value[i];
	}
	
	// Closing " and } to close the dictionary
	buffer[buffer_index++] = '"';
	
	// Return length of dictionary string
	return buffer_index;
}

uint16_t JSON_Dictionary_Calc_Buffer_Size(JSONDictionary* dictionary) {
	// Text length is the length of the text in the dictionary
	// dictionary->entries * 6 is the amount of characters needed for quotes, spaces, etc.
	// (dictionary->entries - 1) * 2 is for the comma's and spaces between each entries
	// + 2 is for opening and closing space
    // + 1 for terminating character
	return dictionary->text_length + (dictionary->size * 6) + ((dictionary->size - 1) * 2) + 2 + 1;
}

uint16_t JSON_Entry_Calc_Buffer_Size(JSONEntry* entry) {
	// Text length is the length of the text in the dictionary
	// dictionary->entries * 6 is the amount of characters needed for quotes, spaces, etc.
	// (dictionary->entries - 1) * 2 is for the comma's and spaces between each entries
	// + 2 is for opening and closing space
	return entry->key_length + entry->value_length + 6;
}

uint16_t JSON_Serialize_Dictionary(JSONDictionary* dictionary, char* buffer, uint16_t buffer_length) {

    memset(buffer, 0, buffer_length);

	uint32_t buffer_index = 0;
	
	// First, we open the dictionary
	buffer[buffer_index++] = '{';
	
	// Add each entry as dictionary
	for (uint8_t i = 0; i < dictionary->size; i++) {
		// Create a new buffer
		uint16_t buffer_length = JSON_Entry_Calc_Buffer_Size(&dictionary->entries[i]);

		char entry_buffer[buffer_length];
		
		// Call helper function to convert entry to string
		uint16_t length = JSON_Serialize_Entry(&dictionary->entries[i], entry_buffer);
		
		// Copy the buffer content from entry buffer to dictionary buffer
		memcpy(buffer + buffer_index, entry_buffer, length);
		
		// Increase the buffer index to keep track of where we are
		buffer_index += length;
		
		// Add a comma and a space between each entry (so everywhere but the last)
		if (i != (dictionary->size - 1)) {
			buffer[buffer_index++] = ',';
			buffer[buffer_index++] = ' ';
		}
	}
	
	// Lastly, we close the dictionary
	buffer[buffer_index++] = '}';
	
	return buffer_index;
}

void JSON_Cleanup(JSONDictionary* dictionary) {
	free(dictionary->entries);
	free(dictionary);
	dictionary = NULL;
}
