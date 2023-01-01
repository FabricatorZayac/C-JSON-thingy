#ifndef FIELD_H_
#define FIELD_H_

#include <stdlib.h>
#include <string.h>
/* #include "serializeable.h" */

typedef struct Serializable Serializable;

enum ValueType {
    ValueType_String,
    ValueType_Integer,
    ValueType_Serializable,
    /* FieldType_Array, */
    /* FieldType_NULL, */
    /* FieldType_Bool */
};

typedef union Value {
    char *string;
    int integer;
    Serializable *object;
} Value;

typedef struct Field {
    char *key;
    enum ValueType value_type;
    Value value;
} Field;

#endif // SERIALIZE_H_
