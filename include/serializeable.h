#ifndef SERIALIZEABLE_H_
#define SERIALIZEABLE_H_

#include "field.h"
#include <stdio.h>

typedef struct Serializeable Serializeable;

void *get_value(Serializeable *self, char *key);
char *Serializeable_to_JSON(Serializeable *self);
void Serializeable_destroy(Serializeable *self);

#define define_struct(struct_name, field_count) \
typedef struct {\
    size_t size;\
    Field *fields[field_count];\
} struct_name\

#endif // SERIALIZEABLE_H_
