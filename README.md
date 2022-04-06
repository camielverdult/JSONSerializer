# JSONSerializer

This is a simple and light C library for serializing strings to a string. 

`JSON_New` returns a new JSON object, with the object and its internal array allocated on the heap.

`JSON_Set_x` sets/updates a key value pair, calls `JSON_Check` first to make sure no out of bounds errors occur.

`JSON_Dictionary_Calc_Buffer_Size` returns the amount of characters needed to hold the serialized object in a character buffer.

`JSON_Serialize_Dictionary` serializes a `JSONDictionary` into a character buffer.

Here is an example:

```c
JSONDictionary new;

JSON_Dictionary_Init(&new);
JSON_Set_Float(&new, "Voltage", 4.832);
JSON_Set_Integer(&new, "Mode", 2);
JSON_Set_String(&new, "Current", "432mA");
JSON_Set_String(&new, "Color", "Blue");

char buffer[1000];

JSON_Serialize_Dictionary(&new, buffer, 1000);

printf("%s\n", buffer);

JSON_Cleanup(&new);
```