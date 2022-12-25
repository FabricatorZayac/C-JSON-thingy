#ifndef SERIALIZEABLE_H_
#define SERIALIZEABLE_H_

#include "field.h"
#include <stdio.h>

typedef struct Serializeable Serializeable;

void *get_value(Serializeable *self, char *key);
char *Serializeable_to_JSON(Serializeable *self);
void Serializeable_destroy(Serializeable *self);

#define DEFINE_STRUCT(struct_name, field_count)\
typedef struct {\
    size_t size;\
    Field *fields[field_count];\
} struct_name;\
const size_t struct_name##_field_count = field_count

#define CONSTRUCTOR_BEGIN(struct_name, ...)\
struct_name *struct_name##_create(__VA_ARGS__) {\
    struct_name *self = malloc(sizeof (struct_name));\
    self->size = struct_name##_field_count;\

#define CONSTRUCTOR_END return self;}

#endif // SERIALIZEABLE_H_
