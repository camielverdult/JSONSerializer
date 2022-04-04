#include <stdio.h>
#include <stdlib.h>
#include "JSON.h"

int main() {
    JSONDictionary* new = JSON_New();

    JSON_Set_Float(new, "Voltage", 4.832);
    JSON_Set_Integer(new, "Mode", 2);
    JSON_Set_String(new, "Current", "432mA");
    JSON_Set_String(new, "Color", "Blue");

    const uint16_t buffer_length = JSON_Dictionary_Calc_Buffer_Size(new);

    printf("Projected length: %d\n", buffer_length);

    char buffer[buffer_length];

    JSON_Serialize_Dictionary(new, buffer, buffer_length);

    printf("%s\n", buffer);

    JSON_Cleanup(new);

    return EXIT_SUCCESS;
}
