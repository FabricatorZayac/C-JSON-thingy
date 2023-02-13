#include "serializable.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

char *JsonValue_stringify(JsonValue *self) {
    char *result = calloc(1, BUFSIZ);
    switch (self->type) {
        case ValueType_Bool:
            self->body.boolean ?
                strcpy(result, "true") :
                strcpy(result, "false");
            break;
        case ValueType_Number:
            // TODO add float support somehow
            sprintf(result, "%d", self->body.number);
            break;
        case ValueType_String:
            sprintf(result, "\"%s\"", self->body.string);
            break;
        case ValueType_Array:
            sprintf(result, "%c", '[');
            for (size_t i = 0; i < self->body.array.size; i++) {
                sprintf(result + strlen(result), "%s, ",
                        JsonValue_stringify(&self->body.array.body[i]));
            }
            sprintf(result + strlen(result) - 2, "%c", ']');
            break;
        case ValueType_Object: {
                char *intermediate = JsonObject_stringify(self->body.object);
                strcpy(result + strlen(result), intermediate);
                free(intermediate);
            }
            break;
        default:
            printf("in Value_to_JSON Unimplemented Type %d", self->type);
            break;
    }
    return result;
}

char *JsonObject_stringify(JsonObject *self) {
    char *result = calloc(1, BUFSIZ);

    sprintf(result, "{");
    Field buffer = self->fields[0];
    for (int i = 0; i < self->size; buffer = self->fields[++i]) {
        char *intermediate = JsonValue_stringify(&buffer.value);
        sprintf(result + strlen(result), "\"%s\": %s, ",
                buffer.key,
                intermediate);
        free(intermediate);
    }
    sprintf(result + strlen(result) - 2, "}");

    return result;
}

void JsonObject_clean(JsonObject *object) {
    free(object->offsets);
}

JsonArray _JsonArray_create(size_t size, ...) {
    JsonValue *array = malloc(size * sizeof(JsonValue));

    va_list args;
    va_start(args, size);
    for (size_t i = 0; i < size; i++) {
        array[i] = va_arg(args, JsonValue);
    }

    return (JsonArray){.size = size, .body = array};
}

JsonValue _JsonValue_create(enum ValueType type, ...) {
    va_list args;
    va_start(args, type);

    JsonValue self = {.type = type, va_arg(args, Value)};
    return self;
}

// NOTE Deep copies objects, shallow copies everything else. Subject to change
void *JsonObject_to_struct(JsonObject *object) {
    void *dest = malloc(BUFSIZ); // NOTE potentially leaks memory
    for (size_t i = 0; i < object->size; i++) {
        switch (object->fields[i].value.type) {
            case ValueType_Object: {
                *(void **)(dest + object->offsets[i]) = JsonObject_to_struct(object->fields[i].value.body.object);
                break;
            default:
                *(Value *)(dest + object->offsets[i]) = object->fields[i].value.body;
                break;
            }
        }
    }
    return dest;
}
