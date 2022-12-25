#include "field.h"
#include "serializeable.h"

Field *Field_create(char *key, enum FieldType value_type, void *value) {
    Field *new = malloc(sizeof (Field));

    new->key = malloc(64);
    strcpy(new->key, key);
    new->value_type = value_type;
    new->value = value;

    return new;
}

void Field_destroy(Field *field) {
    if (field->value_type == FieldType_Serializeable) {
        Serializeable_destroy(field->value);
    } else {
        free(field->value);
    }
    free(field->key);
    free(field);
}
