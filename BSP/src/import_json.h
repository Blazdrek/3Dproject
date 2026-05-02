#pragma once

#include "mega.h"
#include "string.h"

typedef struct JSON_value_s 
{
    //TODO : On peut tightly pack
    char** attributes_names;
    void* attributes_values;    // JSON_value, list, or any type
    void* next_value;   //JSON_value or NULL
} JSON_value;

typedef struct JSON_object_s
{
    void* json_values;   //JSON_value or NULL
} JSON_object;

JSON_object* JSON_parse(char* filename);

//private
//JSON_object* JSON_get_object(char** read_pointer);

// Pas besoin, si ?
//JSON_object* JSON_add_attribute(JSON_object* object, char* attribute, JSON_object* value);

void* JSON_get_attribute(JSON_object* object, char* attribute);
