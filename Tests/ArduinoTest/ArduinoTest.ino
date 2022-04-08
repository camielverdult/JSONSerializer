#include "JSON.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  JSONDictionary json;

  JSON_Dictionary_Init(&json);

  // snprintf may not work on embedded systems
  JSON_Set_Float(&json, "Voltage", analogRead(A0) * (5.0 / 1023.0));
  JSON_Set_Integer(&json, "Mode", 2);
  JSON_Set_String(&json, "Current", "432mA");
  JSON_Set_String(&json, "Color", "Blue");

  char buffer[200];

  JSON_Serialize_Dictionary(&json, buffer, 200);

  Serial.println(buffer);

  JSON_Cleanup(&json);

  delay(500);
}
