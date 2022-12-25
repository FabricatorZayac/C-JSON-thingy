#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "field.h"
#include "file_ops.h"
#include "serializeable.h"
define_struct(ExampleStruct, 3);
define_struct(NestedStruct, 3);
define_struct(Nestle, 2);

ExampleStruct *ExampleStruct_create(char *name, NestedStruct *nest, int value);
NestedStruct *NestedStruct_create(int a, Nestle *b, int c);
Nestle *Nestle_create(int a, char *b);

ExampleStruct *ExampleStruct_create(char *name, NestedStruct *nest, int value) {
    ExampleStruct *self = malloc(sizeof (ExampleStruct));
    self->size = 3;

    new_field_str(0, "name", name);
    new_field_serializeable(1, "nest", nest);
    new_field_int(2, "value", value);

    return self;
}

NestedStruct *NestedStruct_create(int a, Nestle *b, int c) {
    NestedStruct *self = malloc(sizeof (NestedStruct));
    self->size = 3;

    new_field_int(0, "a", a);
    new_field_serializeable(1, "b", b);
    new_field_int(2, "c", c);

    return self;
}

Nestle *Nestle_create(int a, char *b) {
    Nestle *self = malloc(sizeof (Nestle));
    self->size = 2;

    new_field_int(0, "a", a);
    new_field_str(1, "b", b);

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
