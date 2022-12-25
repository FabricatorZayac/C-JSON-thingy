#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "field.h"
#include "file_ops.h"
#include "serializeable.h"

typedef struct ExampleStruct ExampleStruct;
typedef struct NestedStruct NestedStruct;
typedef struct Nestle Nestle;

DEFINE_STRUCT(ExampleStruct, char *name, NestedStruct *nest, int value)
    FIELD_STR("name", name);
    FIELD_SERIALIZEABLE("nest", nest);
    FIELD_INT("value", value);
STRUCT_END

DEFINE_STRUCT(NestedStruct, int a, Nestle *b, int c)
    FIELD_INT("a", a);
    FIELD_SERIALIZEABLE("b", b);
    FIELD_INT("c", c);
STRUCT_END

DEFINE_STRUCT(Nestle, int a, char *b)
    FIELD_INT("a", a);
    FIELD_STR("b", b);
STRUCT_END

void remove_spaces(char* s) {
    char* d = s;
    do
        while (isspace(*d)) ++d;
    while ((*s++ = *d++));
}

void tokenize_json(char *json) {
    printf("%s\n", json);
    remove_spaces(json);
    printf("%s\n", json);
}

int main() {
    ExampleStruct *test = ExampleStruct_create("test1", NestedStruct_create(2, Nestle_create(7, "nestle"), 4), 5);
    write_to_file("test.json", Serializeable_to_JSON((Serializeable *)test));
    Serializeable_destroy((Serializeable *)test);

    tokenize_json(read_from_file("test.json"));
    /* tokenize_json(read_from_file("data.json")); */
}
