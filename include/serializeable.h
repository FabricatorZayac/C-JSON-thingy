#ifndef SERIALIZEABLE_H_
#define SERIALIZEABLE_H_

#include "field.h"
#include <stdio.h>

typedef struct Serializeable Serializeable;

void *get_value(Serializeable *self, char *key);
char *Serializeable_to_JSON(Serializeable *self);

#endif // SERIALIZEABLE_H_
