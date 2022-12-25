#include "serializeable.h"
#include "field.h"
#include <stddef.h>

struct Serializeable {
    size_t size;
    Field *fields[];
};

void *get_value(Serializeable *self, char *key) {
    Field *buffer = self->fields[0];
    for (int i = 0; self->size; buffer = self->fields[++i])
        if (!strcmp(buffer->key, key)) return buffer->value;
    return NULL;
}

char *Serializeable_to_JSON(Serializeable *self) {
    // TODO automatic result size thingy
    // in this case 256 is the max length of JSON string
    char *result = (char *)malloc(256);

    sprintf(result, "{");
    Field *buffer = self->fields[0];
    for (int i = 0; i < self->size; buffer = self->fields[++i]) {
        switch (self->fields[i]->value_type) {
        case FieldType_String:
            sprintf(result + strlen(result), "\"%s\": \"%s\"",
                    self->fields[i]->key,
                    (char *)get_value(self, self->fields[i]->key));
            break;
        case FieldType_Integer:
            sprintf(result + strlen(result), "\"%s\": %d",
                    self->fields[i]->key,
                    *(int *)get_value(self, self->fields[i]->key));
            break;
        case FieldType_Serializeable:
            sprintf(result + strlen(result), "\"%s\": %s",
                    self->fields[i]->key,
                    Serializeable_to_JSON(get_value(self, self->fields[i]->key)));
            break;
        default:
            puts("Unimplemented Type");
            break;
        }
        if (i != self->size - 1) sprintf(result + strlen(result), ", ");
    }
    sprintf(result + strlen(result), "}");

    return result;
}

void Serializeable_destroy(Serializeable *self) {
    for (int i; i < self->size; i++) {
        Field_destroy(self->fields[i]);
    }
    free(self);
}
