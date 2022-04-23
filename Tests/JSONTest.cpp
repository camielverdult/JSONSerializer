//
// Created by Camiel Verdult on 23/04/2022.
//

#include <gtest/gtest.h>
#include "../JSON.h"
#include <string>

TEST(JSONSerializer, BasicStringSerialization) {

    // Expect two strings not to be equal.
    JSONDictionary json;

    JSON_Dictionary_Init(&json);

    JSON_Set_String(&json, "google", "test");

    char buffer[50];

    JSON_Serialize_Dictionary(&json, buffer, 50);

    std::string dictionary = R"({ "google": "test" })";

    EXPECT_EQ(std::string(buffer), dictionary);
}

TEST(JSONSerializer, BasicIntSerialization) {

    // Expect two strings not to be equal.
    JSONDictionary json;

    JSON_Dictionary_Init(&json);

    JSON_Set_Integer(&json, "number", 1);

    char buffer[50];

    JSON_Serialize_Dictionary(&json, buffer, 50);

    std::string dictionary = R"({ "number": "1" })";

    EXPECT_EQ(std::string(buffer), dictionary);
}