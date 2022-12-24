#include "field.h"

Field *Field_create(char *key, enum FieldType value_type, void *value) {
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
