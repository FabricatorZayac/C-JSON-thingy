#ifndef FIELD_H_
#define FIELD_H_

#include <stdlib.h>
#include <string.h>

enum FieldType { FieldType_String, FieldType_Integer, FieldType_Serializeable };

typedef struct Field {
    char *key;
    enum FieldType value_type;
    void *value;
} Field;

Field *Field_create(char *key, enum FieldType value_type, void *value);
void Field_destroy(Field *field);

// To be used to initialize fields in serializeable struct constructor
#define FIELD_INT(name, value) {\
    int *buffer_##__LINE__ = malloc(sizeof (int));\
    *buffer_##__LINE__ = value;\
    self->fields[i] = Field_create(name, FieldType_Integer, buffer_##__LINE__);\
    i++;\
}

#define FIELD_STR(name, value) {\
    char *buffer_##__LINE__ = malloc(128);\
    strcpy(buffer_##__LINE__, value);\
    self->fields[i] = Field_create(name, FieldType_String, buffer_##__LINE__);\
    i++;\
}

#define FIELD_SERIALIZEABLE(name, value) {\
    self->fields[i] = Field_create(name, FieldType_Serializeable, value);\
    i++;\
}

#endif // SERIALIZE_H_
