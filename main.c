#include <stdio.h>
#include <stdlib.h>
#include "JSON.h"

int main() {

    JSONDictionary new;

    JSON_Dictionary_Init(&new);

    JSON_Set_Float(&new, "Voltage", 4.832);
    JSON_Set_Integer(&new, "Mode", 2);
    JSON_Set_String(&new, "Current", "432mA");
    JSON_Set_String(&new, "Color", "RedGreenBlueWhite");

    // This will not set a new string because the value string length is too long
    JSON_Set_String(&new, "Alphabet", "abcdefghijklmnopqrstuvwxyz");

    char buffer[1000];

    JSON_Serialize_Dictionary(&new, buffer, 1000);

    printf("Length of buffer: %lu\n%s\n", strlen(buffer), buffer);

    JSON_Cleanup(&new);

    return EXIT_SUCCESS;
}
