#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ExampleStruct ExampleStruct;
typedef struct Field Field;

enum Type { Type_String, Type_Integer };

// TODO make generic
// NOTE current idea is to feed either a void pointer or some kind of serializeable struct interface
char *ExampleStruct_to_JSON(ExampleStruct *test);
ExampleStruct *ExampleStruct_from_JSON(char *json);
ExampleStruct *ExampleStruct_create(char *name, int value);
void ExampleStruct_destroy(ExampleStruct* es);
Field *Field_create(char *key, enum Type value_type, void *value);
void Field_destroy(Field *field);

void write_to_file(char *filename, char *string);
char *read_from_file(char *filename);

struct Field {
    char *key;
    enum Type value_type;
    void *value;
};

Field *Field_create(char *key, enum Type value_type, void *value) {
    Field *new = malloc(sizeof (Field));

    new->key = malloc(64);
    strcpy(new->key, key);
    new->value_type = value_type;
    new->value = value;

    return new;
}

void Field_destroy(Field *field) {
    free(field->key);
    free(field->value);
    free(field);
}

#define ExampleStruct_FIELDCOUNT 2
struct ExampleStruct {
    Field *fields[ExampleStruct_FIELDCOUNT + 1];
    /* Fields
     * char *name;
     * int value;
     */
};

ExampleStruct *ExampleStruct_create(char *name, int value) {
    ExampleStruct *new = malloc(sizeof (ExampleStruct));

    // char *name = name;
    char *name_buffer = malloc(128);
    strcpy(name_buffer, name);
    new->fields[0] = Field_create("name", Type_String, name_buffer);

    // int value = value;
    int *value_buffer = malloc(sizeof (int));
    *value_buffer = value;
    new->fields[1] = Field_create("value", Type_Integer, value_buffer);

    //terminator
    new->fields[2] = NULL;

    return new;
}

void ExampleStruct_destroy(ExampleStruct* es) {
    for (int i; i < ExampleStruct_FIELDCOUNT; i++) {
        Field_destroy(es->fields[i]);
    }
    free(es);
}
#undef ExampleStruct_FIELDCOUNT

char *ExampleStruct_to_JSON(ExampleStruct *test) {
    // TODO automatic result size thingy
    // in this case 256 is the max length of JSON string
    char *result = (char *)malloc(256);

    sprintf(result, "{");
    Field *buffer = test->fields[0];
    for (int i = 0; buffer != NULL;) {
        switch (test->fields[i]->value_type) {
        case Type_String:
            sprintf(result + strlen(result), "\"%s\": \"%s\",", test->fields[i]->key, (char *)test->fields[i]->value);
            break;
        case Type_Integer:
            sprintf(result + strlen(result), "\"%s\": %d,", test->fields[i]->key, *(int *)test->fields[i]->value);
            break;
        default:
            puts("Unimplemented Type");
            break;
        }
        buffer = test->fields[++i];
    }
    sprintf(result + strlen(result), "}");

    return result;
}

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

void write_to_file(char *filename, char *string) {
    FILE *outfile = fopen(filename, "w");
    fprintf(outfile, "%s", string);
    fclose(outfile);
}

char *read_from_file(char *filename) {
    char *buffer = malloc(256);
    FILE *infile = fopen(filename, "r");
    fscanf(infile, "%s", buffer);
    fclose(infile);

    return buffer;
}

int main() {
    ExampleStruct *test = ExampleStruct_create("test1", 5);
    write_to_file("test.json", ExampleStruct_to_JSON(test));
    ExampleStruct_destroy(test);

    ExampleStruct *test2 = ExampleStruct_from_JSON(read_from_file("test.json"));
    printf("%s: %s\n", test2->fields[0]->key, (char *)test2->fields[0]->value);
    printf("%s: %d\n", test2->fields[1]->key, *(int *)test2->fields[1]->value);
}
