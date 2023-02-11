#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "cursed_macros.h"

typedef struct JsonObject JsonObject;
typedef struct JsonArray JsonArray;
typedef struct JsonValue JsonValue;
typedef struct Field Field;

enum ValueType {
    ValueType_Bool,
    ValueType_Integer,
    ValueType_String,
    ValueType_Array,
    ValueType_Object,
};

typedef union {
    char *string;
    int integer;
    JsonObject *object;
    JsonArray *array;
    bool boolean;
} Value;

struct JsonValue {
    enum ValueType type;
    Value body;
};

struct Field {
    char *key;
    JsonValue value;
};

struct JsonArray {
    size_t size;
    JsonValue *body;
};

struct JsonObject {
    size_t size;
    size_t *offsets;
    Field fields[];
};

char *JsonValue_stringify(JsonValue *self);
char *JsonObject_stringify(JsonObject *self);

void JsonValue_clean(JsonValue *value);
void JsonObject_clean(JsonObject *object);

JsonArray _JsonArray_create(size_t size, ...);
JsonValue _JsonValue_create(enum ValueType type, ...);

void *JsonObject_to_struct(JsonObject *object);

#define DEFINE_STRUCT(struct_name, ...)                                 \
    typedef struct {                                                    \
        CONCAT(__VA_ARGS__)                                             \
    } struct_name;                                                      \
    typedef struct {                                                    \
        size_t size;                                                    \
        size_t *offsets;                                                \
        Field fields[NUM_ARGS(__VA_ARGS__)];                            \
    } struct_name##_json;                                               \
    struct_name##_json struct_name##_create(__VA_ARGS__) {              \
        struct_name##_json self = {.size = NUM_ARGS(__VA_ARGS__)};      \
        self.offsets = calloc(NUM_ARGS(__VA_ARGS__), sizeof(size_t));    \
        size_t i = 0;                                                   \
        typedef struct_name self_type;                                  \

// To be used to initialize fields in serializeable struct constructor
// Objects and arrays are passed by reference, strings get copied, but I might change that
#define FIELD_BOOL(value)                                               \
    self.offsets[i] = offsetof(self_type, value);                       \
    self.fields[i++] =                                                  \
        (Field){#value, JsonValue(ValueType_Bool, value)};              \

#define FIELD_INT(value)                                                \
    self.offsets[i] = offsetof(self_type, value);                       \
    self.fields[i++] =                                                  \
        (Field){#value, JsonValue(ValueType_Integer, value)};

#define FIELD_STR(value)                                                \
    self.offsets[i] = offsetof(self_type, value);                       \
    self.fields[i++] =                                                  \
        (Field){#value, JsonValue(ValueType_String, strcpy(calloc(1, strlen(value)), value))};

#define FIELD_ARRAY(value)                                              \
    self.offsets[i] = offsetof(self_type, value);                       \
    self.fields[i++] =                                                  \
        (Field){#value, JsonValue(ValueType_Array, value)};

#define FIELD_OBJECT(value)                                             \
    self.offsets[i] = offsetof(self_type, value);                       \
    self.fields[i++] =                                                  \
        (Field){#value, JsonValue(ValueType_Object, value)};

#define STRUCT_END return self;}

#define JsonValue(type, value) _JsonValue_create(type, value)

#define JsonArray(...) _JsonArray_create(NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

#endif // SERIALIZABLE_H_
