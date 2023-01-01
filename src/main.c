#include <ctype.h>

#include "file_ops.h"
#include "serializable.h"

typedef struct ExampleStruct ExampleStruct;
typedef struct NestedStruct NestedStruct;
typedef struct Nestle Nestle;

DEFINE_STRUCT(ExampleStruct, char *name, NestedStruct *nest, int value)
    FIELD_STR(name);
    FIELD_SERIALIZEABLE(nest);
    FIELD_INT(value);
STRUCT_END

DEFINE_STRUCT(NestedStruct, int a, Nestle *b, int c)
    FIELD_INT(a);
    FIELD_SERIALIZEABLE(b);
    FIELD_INT(c);
STRUCT_END

DEFINE_STRUCT(Nestle, int a, char *b)
    FIELD_INT(a);
    FIELD_STR(b);
STRUCT_END

void remove_spaces(char* s) {
    char* d = s;
    do
        while (isspace(*d)) ++d;
    while ((*s++ = *d++));
}

int main() {
    Nestle nestle = Nestle_create(/*a: */7,
                                  /*b: */"nestle");

    NestedStruct nest = NestedStruct_create(/*a: */2,
                                            /*b: */&nestle,
                                            /*c: */4);

    ExampleStruct test = ExampleStruct_create(/* name: */"test1",
                                              /* nest: */&nest,
                                              /*value: */5);

    write_to_file("test.json", Serializeable_to_JSON(&test));
}
