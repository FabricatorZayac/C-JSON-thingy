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
#define NEW_FIELD_INT(fieldnum, name, value) {\
    int *buffer_##fieldnum = malloc(sizeof (int));\
    *buffer_##fieldnum = value;\
    self->fields[fieldnum] = Field_create(name, FieldType_Integer, buffer_##fieldnum);\
}

#define NEW_FIELD_STR(fieldnum, name, value) {\
    char *buffer_##fieldnum = malloc(128);\
    strcpy(buffer_##fieldnum, value);\
    self->fields[fieldnum] = Field_create(name, FieldType_String, buffer_##fieldnum);\
}

#define NEW_FIELD_SERIALIZEABLE(fieldnum, name, value) {\
    self->fields[fieldnum] = Field_create(name, FieldType_Serializeable, value);\
}

#endif // SERIALIZE_H_
