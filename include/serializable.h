#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

struct JsonValue {
    enum ValueType type;
    union {
        char *string;
        int integer;
        JsonObject *object;
        JsonArray *array;
        bool boolean;
    } body;
};

struct Field {
    char *key;
    JsonValue value;
};

struct JsonArray {
    size_t size;
    JsonValue *array;
};

struct JsonObject {
    size_t size;
    Field fields[];
};

char *JsonValue_stringify(JsonValue *self);
char *JsonObject_stringify(JsonObject *self);

void JsonValue_destroy(JsonValue *value);
void JsonObject_destroy(JsonObject *object);

JsonArray JsonArray_create(size_t size, ...);
JsonValue JsonValue_create(enum ValueType type, ...);

#define _NUM_ARGS2(X,X64,X63,X62,X61,X60,X59,X58,X57,X56,X55,X54,X53,X52,X51,X50,X49,X48,X47,X46,X45,X44,X43,X42,X41,X40,X39,X38,X37,X36,X35,X34,X33,X32,X31,X30,X29,X28,X27,X26,X25,X24,X23,X22,X21,X20,X19,X18,X17,X16,X15,X14,X13,X12,X11,X10,X9,X8,X7,X6,X5,X4,X3,X2,X1,N,...) N
#define NUM_ARGS(...) _NUM_ARGS2(0, __VA_ARGS__ ,64,63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define DEFINE_STRUCT(struct_name, ...)                     \
typedef struct {                                            \
    size_t size;                                            \
    Field fields[NUM_ARGS(__VA_ARGS__)];                    \
} struct_name;                                              \
const size_t struct_name##_size = NUM_ARGS(__VA_ARGS__);    \
struct_name struct_name##_create(__VA_ARGS__) {             \
    struct_name self = {.size = struct_name##_size};        \
    size_t i = 0;

// To be used to initialize fields in serializeable struct constructor
#define FIELD_BOOL(value) self.fields[i++] = \
        (Field){#value, ValueType_Bool, {.boolean = value}};

#define FIELD_INT(value) self.fields[i++] = \
        (Field){#value, ValueType_Integer, {.integer = value}};

#define FIELD_STR(value) self.fields[i++] = \
        (Field){#value, ValueType_String, {.string = strcpy(calloc(1, strlen(value)), value)}};

#define FIELD_ARRAY(value) self.fields[i++] = \
        (Field){#value, ValueType_Array, {.array = value}};

#define FIELD_OBJECT(value) self.fields[i++] = \
        (Field){#value, ValueType_Object, {.object = value}};

#define STRUCT_END return self;}

#define JsonValue(type, value) JsonValue_create(type, value)

#define JsonArray(...) JsonArray_create(NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

#endif // SERIALIZABLE_H_
