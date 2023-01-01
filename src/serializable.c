#include "serializable.h"
#include "field.h"
#include <stddef.h>
#include <stdlib.h>

struct Serializable {
    size_t size;
    Field fields[];
};

Value get_value(Serializable *self, char *key) {
    Field buffer = self->fields[0];
    for (int i = 0; self->size; buffer = self->fields[++i]) if (!strcmp(buffer.key, key)) return buffer.value;

    // TODO error handling
    exit(EXIT_FAILURE);
}

char *Serializeable_to_JSON(Serializable *self) {
    // TODO automatic result size thingy
    // in this case 256 is the max length of JSON string
    char *result = (char *)malloc(256);

    sprintf(result, "{");
    Field buffer = self->fields[0];
    for (int i = 0; i < self->size; buffer = self->fields[++i]) {
        switch (buffer.value_type) {
        case ValueType_String:
            sprintf(result + strlen(result), "\"%s\": \"%s\"",
                    buffer.key,
                    get_value(self, buffer.key).string);
            break;
        case ValueType_Integer:
            sprintf(result + strlen(result), "\"%s\": %d",
                    buffer.key,
                    get_value(self, buffer.key).integer);
            break;
        case ValueType_Serializable:
            sprintf(result + strlen(result), "\"%s\": %s",
                    buffer.key,
                    Serializeable_to_JSON(get_value(self, buffer.key).object));
            break;
        /* case FieldType_Array: */
        /*     sprintf(result + strlen(result), "\"%s\": %s", */
        /*             self->fields[i]->key); */
        /*     break; */
        default:
            puts("Unimplemented Type");
            break;
        }
        if (i != self->size - 1) sprintf(result + strlen(result), ", ");
    }
    sprintf(result + strlen(result), "}");

    return result;
}
