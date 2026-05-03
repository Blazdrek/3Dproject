#include "import_json.h"

#define DEBUG_LINECOUNT 5

// Pas mal cette fonction, non ?
void err_parsing(FILE* fptr, const char* message, ...) {
    va_list args;
    va_start(args, message);    // Init args a partir de ce qu'il y a apres message
    vprintf(message, args);   // Comme fprintf, mais prends une liste pour le format
    printf("\n");

    printf("\t Here is the text following the error :\n");
    char* line1 = NULL;
    int count;
    getline(&line1, &count, fptr);
    printf("<ERROR>%s", line1);

    for (size_t i = 0; i < DEBUG_LINECOUNT; i++)
    {
        getline(&line1, &count, fptr);
        printf("%s", line1);
        
    }
    

    errx(EXIT_FAILURE, "");
}

/* The JSON Grammar (this is context grammar)
start : object

data_type: object | array | string | value | bool | null;

array: '[' { data_type {',' data_type} }? ']';

object: '{' { JSON_Att {',' JSON_Att} }? '}';
JSON_Att: string ':' data_type;



string: '"' [^ \n\t\r]* '"';
value: -? num+ ('.' num+)?; #osef des exponentiels


num: [0-9];
bool: "true" | "false"
null: "null"

*/

// Declaration anticipé
void* evaluate_datatype(FILE* fptr);


// Helper functions
// All evaluate function are called after first token removed {, [, "...
// But put back the last character }, ], "

// Call after first '"' until next '"'
char* evaluate_string(FILE* fptr) {
    string* buffer = string_create();
    char ch;
    char* res;
    while ((ch = fgetc(fptr)) != EOF) {
        switch (ch)
        {
        case '\t':
        case '\n':
            err_parsing(fptr, "[Import JSON] Error in string definition. Found character \"%c\".", ch);

        case '"':
            res = string_get(buffer);
            free(buffer);
            return res;
            break;
        
        default:
            string_add_char(buffer, ch);
            break;
        }
    }
    errx(EXIT_FAILURE, "[Import JSON] Error in attribute name definition. String no closed");
}

float evaluate_value(FILE* fptr) {
    string* buffer = string_create();
    char ch;
    float res;
    while ((ch = fgetc(fptr)) != EOF) {
        switch (ch)
        {
        case '"':
            res = atof(string_get(buffer));
            free(buffer);
            return res;
            break;
        
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            string_add_char(buffer, ch);
            break;

        default:
            err_parsing(fptr, "[Import JSON] Error in float definition. Found character \"%c\".", ch);
        }
    }
    errx(EXIT_FAILURE, "[Import JSON] Error in float definition.");

}

JSON_value* evaluate_JSON_attribute();

JSON_object* evaluate_object(FILE* fptr) {
    JSON_object* res = malloc(sizeof(JSON_object));
    res->json_values = NULL;
    
    JSON_value** json_values_tail = &(res->json_values);
    
    char ch;
    char *attribute_name;
    JSON_object* attributes_value;
    while ((ch = fgetc(fptr)) != EOF) {
        switch (ch)
        {
        case ' ':
        case '\t':
        case '\n':
            break;

        case ',':
            
            break;
        case ':':
            attributes_value = (JSON_object*) evaluate_datatype(fptr);
            (*json_values_tail) = malloc(sizeof(JSON_value));
            //(*json_values_tail)  //TODO : J'etais en train de dev ici avant de me depecher de push
            break;
        case '"':
            // attribute name
            attribute_name = evaluate_string(fptr);
            if ((ch = fgetc(fptr)) != ':') err_parsing(fptr, "[Import JSON] Missing \":\" after attribute name.");
            
            break;
        
        default:
            break;
        }
    }

    return NULL;
}

// WARNING : Put back the last char in the descriptor, as it is useful to parse
void* evaluate_datatype(FILE* fptr) {
    switch (fgetc(fptr))
        {
        case ' ':
        case '\t':
        case '\n':
            break;
        
        case '{':
            break;

        case '[':
            break;

        case '"':
            return evaluate_string(fptr);
            
        case 't': //true
            break;
    
        case 'f': //false
            break;
    
        default:
            //putback and evaluate_value
            break;
        }
}

JSON_object* JSON_parse(char* filename) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        err_parsing(fptr, "[Import JSON] Unable to open the file \"%s\".", filename);
    }
    if (fgetc(fptr) != '{') {
        err_parsing(fptr, "[Import JSON] First character is not a \"{\".");
    }

    
    JSON_object* res = evaluate_object(fptr);
    fclose(fptr);
    return res;
}


//JSON_object* JSON_add_attribute(JSON_object* object, char* attribute, JSON_object* value) {}

void* JSON_get_attribute(JSON_object* object, char* attribute) {

}