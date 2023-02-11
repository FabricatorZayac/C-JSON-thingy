#include "serializable.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
            strcpy(result, "true") :
            strcpy(result, "false");
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
            strcpy(result + strlen(result), intermediate);
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

void SerializableValue_destroy(SerializableValue *value) {
    switch (value->value_type) {
        case ValueType_Bool:
        case ValueType_Integer:
            break;
        case ValueType_String:
            free(value->value.string);
            break;
        case ValueType_Array:
            for (size_t i = 0; i < value->value.array->size; i++) {
                SerializableValue_destroy(&value->value.array->array[i]);
            }
            free(value->value.array->array);
            break;
        case ValueType_Object:
            for (size_t i = 0; i < value->value.object->size; i++) {
                SerializableValue_destroy(&value->value.object->fields[i].value);
            }
            break;
    }
}

void SerializableObject_destroy(SerializableObject *object) {
    SerializableValue_destroy(&(SerializableValue) {
        .value_type = ValueType_Object,
        .value.object = object,
    });
}

SerializableArray SerializableArray_create(size_t size, ...) {
    SerializableValue *array = malloc(size * sizeof(SerializableValue));

    va_list args;
    va_start(args, size);
    for (size_t i = 0; i < size; i++) {
        array[i] = va_arg(args, SerializableValue);
    }

    return (SerializableArray){.size = size, .array = array};
}

SerializableValue SerializableValue_create(enum ValueType type, ...) {
    va_list args;
    va_start(args, type);
    SerializableValue self = {.value_type = type};
    switch (type) {
        case ValueType_Bool:
            self.value.boolean = (bool)va_arg(args, int);
            break;
        case ValueType_Integer:
            self.value.integer = va_arg(args, int);
            break;
        case ValueType_String: {
            char *string = va_arg(args, char *);
            self.value.string = strcpy(calloc(1, strlen(string)), string);
            break;
        }
        case ValueType_Array:
            self.value.array = va_arg(args, SerializableArray *);
            break;
        case ValueType_Object:
            self.value.object = va_arg(args, SerializableObject *);
            break;
    }
    return self;
}
