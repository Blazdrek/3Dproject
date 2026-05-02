#pragma once

#include "mega.h"


typedef struct string_s {
    char* value;
    int size;
    int true_size;
} string ;

string* string_create();
string* string_convert(char* input);
string* string_create_from(char* input);
string* string_add_char(string* str, char c);
string* string_concat(string* str, char* input);
char* string_get(string* str);
void string_destroy(string* str);


