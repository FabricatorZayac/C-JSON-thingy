#include "serializable.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

char *JsonValue_stringify(JsonValue *self) {
    char *result = calloc(1, BUFSIZ);
    switch (self->type) {
    case ValueType_Bool:
        result = self->body.boolean ?
            strcpy(result, "true") :
            strcpy(result, "false");
        break;
    case ValueType_Integer:
        sprintf(result, "%d", self->body.integer);
        break;
    case ValueType_String:
        sprintf(result, "\"%s\"", self->body.string);
        break;
    case ValueType_Array:
        sprintf(result, "%c", '[');
        for (size_t i = 0; i < self->body.array->size; i++) {
            sprintf(result + strlen(result), "%s, ",
                    JsonValue_stringify(&self->body.array->body[i]));
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

void JsonValue_clean(JsonValue *value) {
    switch (value->type) {
        case ValueType_Bool:
        case ValueType_Integer:
            break;
        case ValueType_String:
            free(value->body.string);
            break;
        case ValueType_Array:
            for (size_t i = 0; i < value->body.array->size; i++) {
                JsonValue_clean(&value->body.array->body[i]);
            }
            free(value->body.array->body);
            break;
        case ValueType_Object:
            for (size_t i = 0; i < value->body.object->size; i++) {
                JsonValue_clean(&value->body.object->fields[i].value);
            }
            break;
    }
}

void JsonObject_clean(JsonObject *object) {
    JsonValue_clean(&(JsonValue) {
        .type = ValueType_Object,
        .body.object = object,
    });
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

    JsonValue self = {.type = type};
    switch (type) {
        case ValueType_Bool:
            self.body.boolean = (bool)va_arg(args, int);
            break;
        case ValueType_Integer:
            self.body.integer = va_arg(args, int);
            break;
        case ValueType_String: {
            char *string = va_arg(args, char *);
            self.body.string = strcpy(calloc(1, strlen(string)), string);
            break;
        }
        case ValueType_Array:
            self.body.array = va_arg(args, JsonArray *);
            break;
        case ValueType_Object:
            self.body.object = va_arg(args, JsonObject *);
            break;
    }
    return self;
}

void *JsonObject_to_struct(JsonObject *object) {
    void *dest = malloc(BUFSIZ); // NOTE potentially leaks memory
    for (size_t i = 0; i < object->size; i++) {
        switch (object->fields[i].value.type) {
            case ValueType_Bool:
                *(bool *)(dest + object->offsets[i]) = object->fields[i].value.body.boolean;
                break;
            case ValueType_Integer:
                *(int *)(dest + object->offsets[i]) = object->fields[i].value.body.integer;
                break;
            case ValueType_String:
                /* *(char **)(dest + offsets[i]) = strcpy(calloc(0, sizeof(object.fields[i].value.body.string)), object.fields[i].value.body.string); */
                *(char **)(dest + object->offsets[i]) = object->fields[i].value.body.string;
                break;
            case ValueType_Array:
                *(JsonArray **)(dest + object->offsets[i]) = object->fields[i].value.body.array;
                break;
            case ValueType_Object: {
                *(void **)(dest + object->offsets[i]) = JsonObject_to_struct(object->fields[i].value.body.object);
                break;
            }
        }
    }
    return dest;
}
