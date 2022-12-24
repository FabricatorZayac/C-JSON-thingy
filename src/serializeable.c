#include "serializeable.h"

struct Serializeable {
    char _;
    Field *fields[];
};

void *get_value(Serializeable *self, char *key) {
    Field *buffer = self->fields[0];
    for (int i = 0; buffer != NULL; buffer = self->fields[++i])
        if (!strcmp(buffer->key, key)) return buffer->value;
    return NULL;
}

char *Serializeable_to_JSON(Serializeable *self) {
    // TODO automatic result size thingy
    // in this case 256 is the max length of JSON string
    char *result = (char *)malloc(256);

    sprintf(result, "{");
    Field *buffer = self->fields[0];
    for (int i = 0; buffer != NULL; buffer = self->fields[++i]) {
        switch (self->fields[i]->value_type) {
        case FieldType_String:
            sprintf(result + strlen(result), "\"%s\": \"%s\",",
                    self->fields[i]->key,
                    (char *)self->fields[i]->value);
            break;
        case FieldType_Integer:
            sprintf(result + strlen(result), "\"%s\": %d,",
                    self->fields[i]->key,
                    *(int *)self->fields[i]->value);
            break;
        default:
            puts("Unimplemented Type");
            break;
        }
    }
    sprintf(result + strlen(result), "}");

    return result;
}
