#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "field.h"
#include "file_ops.h"
#include "serializeable.h"

/* typedef struct ExampleStruct ExampleStruct; */

DEFINE_STRUCT(ExampleStruct, 3);
DEFINE_STRUCT(NestedStruct, 3);
DEFINE_STRUCT(Nestle, 2);

CONSTRUCTOR_BEGIN(ExampleStruct, char *name, NestedStruct *nest, int value)
    NEW_FIELD_STR(0, "name", name);
    NEW_FIELD_SERIALIZEABLE(1, "nest", nest);
    NEW_FIELD_INT(2, "value", value);
CONSTRUCTOR_END

/* ExampleStruct *ExampleStruct_create(char *name, NestedStruct *nest, int value) { */
/*     ExampleStruct *self = malloc(sizeof (ExampleStruct)); */
/*     self->size = 3; */

/*     NEW_FIELD_STR(0, "name", name); */
/*     NEW_FIELD_SERIALIZEABLE(1, "nest", nest); */
/*     NEW_FIELD_INT(2, "value", value); */

/*     return self; */
/* } */

NestedStruct *NestedStruct_create(int a, Nestle *b, int c) {
    NestedStruct *self = malloc(sizeof (NestedStruct));
    self->size = 3;

    NEW_FIELD_INT(0, "a", a);
    NEW_FIELD_SERIALIZEABLE(1, "b", b);
    NEW_FIELD_INT(2, "c", c);

    return self;
}

Nestle *Nestle_create(int a, char *b) {
    Nestle *self = malloc(sizeof (Nestle));
    self->size = 2;

    NEW_FIELD_INT(0, "a", a);
    NEW_FIELD_STR(1, "b", b);

    return self;
}

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
