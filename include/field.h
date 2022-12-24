#ifndef FIELD_H_
#define FIELD_H_

#include <stdlib.h>
#include <string.h>

enum FieldType { FieldType_String, FieldType_Integer };

typedef struct Field {
    char *key;
    enum FieldType value_type;
    void *value;
} Field;

Field *Field_create(char *key, enum FieldType value_type, void *value);
void Field_destroy(Field *field);

#endif // SERIALIZE_H_
