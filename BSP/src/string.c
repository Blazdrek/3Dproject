#include "string.h"

#define DEFAULT_SIZE 128

void string_expand(string* str) {
    str->value = realloc(str->value, sizeof(char)*(str->true_size+DEFAULT_SIZE));
    str->true_size += DEFAULT_SIZE;
}

string* string_create() {
    return string_create_from("");
}

string* string_add_char(string* str, char c) {
    if (str->size +1 > str->true_size) string_expand(str);
    str->value[str->size] = c;
    str->size += 1;
    return str;
}

string* string_concat(string* str, char* input) {
    size_t i = 0;
    while (input[i] != 0) {
        string_add_char(str, input[i]);
        i++;
    }
    return str;
}

string* string_create_from(char* input) {
    string* res = malloc(sizeof(string));
    res->size = 0;
    res->true_size = DEFAULT_SIZE;
    res->value = malloc(sizeof(char) * DEFAULT_SIZE);
    string_concat(res, input);
    return res;
}

char* string_get(string* str) {
    string_add_char(str, 0);
    str->size -=1;
    return str->value;
}

void string_destroy(string* str) {
    free(str->value);
    free(str);
}

