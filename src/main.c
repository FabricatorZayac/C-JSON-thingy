#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file_ops.h"
#include "serializeable.h"

typedef struct ExampleStruct ExampleStruct;

ExampleStruct *ExampleStruct_from_JSON(char *json);
ExampleStruct *ExampleStruct_create(char *name, int value);
void ExampleStruct_destroy(ExampleStruct* self);

#define FIELDCOUNT 2
struct ExampleStruct {
    char _;
    Field *fields[FIELDCOUNT + 1];
    // Fields
    // char *name;
    // int value;
};

ExampleStruct *ExampleStruct_create(char *name, int value) {
    ExampleStruct *new = malloc(sizeof (ExampleStruct));

    // char *name = name;
    char *name_buffer = malloc(128);
    strcpy(name_buffer, name);
    new->fields[0] = Field_create("name", FieldType_String, name_buffer);

    // int value = value;
    int *value_buffer = malloc(sizeof (int));
    *value_buffer = value;
    new->fields[1] = Field_create("value", FieldType_Integer, value_buffer);

    //terminator
    new->fields[FIELDCOUNT] = NULL;

    return new;
}

void ExampleStruct_destroy(ExampleStruct* self) {
    for (int i; i < FIELDCOUNT; i++) {
        Field_destroy(self->fields[i]);
    }
    free(self);
}
#undef FIELDCOUNT

ExampleStruct *ExampleStruct_from_JSON(char *json) {
    char *name = malloc(128);
    int value;

    // TODO figure out how scanf formatting actually works
    // FIXME
    sscanf(json, "{\"name\": \"%s\",\"value\": %d,}", name, &value);

    ExampleStruct *result = ExampleStruct_create(name, value);
    free(name);
    return result;
}

int main() {
    ExampleStruct *test = ExampleStruct_create("test1", 5);
    write_to_file("test.json", Serializeable_to_JSON((Serializeable *)test));
    ExampleStruct_destroy(test);

    ExampleStruct *test2 = ExampleStruct_from_JSON(read_from_file("test.json"));
    printf("%s: %s\n", test2->fields[0]->key, (char *)test2->fields[0]->value);
    printf("%s: %d\n", test2->fields[1]->key, *(int *)test2->fields[1]->value);
}
