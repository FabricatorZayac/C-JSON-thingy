#include "serializable.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

Value get_value(SerializableObject *self, char *key) {
    Field buffer = self->fields[0];
    for (int i = 0; self->size; buffer = self->fields[++i])
        if (!strcmp(buffer.key, key)) return buffer.value.value;

    // TODO error handling
    exit(EXIT_FAILURE);
}

char *SerializableValue_to_JSON(SerializableValue *self) {
    char *result = calloc(1, BUFSIZ);
    switch (self->value_type) {
    case ValueType_Bool:
        result = self->value.boolean ?
            strncpy(result, "true", 4) :
            strncpy(result, "false", 5);
        break;
    case ValueType_Integer:
        sprintf(result, "%d", self->value.integer);
        break;
    case ValueType_String:
        sprintf(result, "\"%s\"", self->value.string);
        break;
    case ValueType_Array:
        sprintf(result, "%c", '[');
        for (size_t i = 0; i < self->value.array->size; i++) {
            sprintf(result + strlen(result), "%s, ",
                    SerializableValue_to_JSON(&self->value.array->array[i]));
        }
        sprintf(result + strlen(result) - 2, "%c", ']');
        break;
    case ValueType_Object: {
            char *intermediate = SerializableObject_to_JSON(self->value.object);
            strncpy(result + strlen(result), intermediate, strlen(intermediate));
            free(intermediate);
        }
        break;
    default:
        printf("in Value_to_JSON Unimplemented Type %d", self->value_type);
        break;
    }
    return result;
}

char *SerializableObject_to_JSON(SerializableObject *self) {
    char *result = calloc(1, BUFSIZ);

    sprintf(result, "{");
    Field buffer = self->fields[0];
    for (int i = 0; i < self->size; buffer = self->fields[++i]) {
        char *intermediate = SerializableValue_to_JSON(&buffer.value);
        sprintf(result + strlen(result), "\"%s\": %s, ",
                buffer.key,
                intermediate);
        free(intermediate);
    }
    sprintf(result + strlen(result) - 2, "}");

    return result;
}
