#include <ctype.h>

#include "file_ops.h"
#include "serializable.h"

DEFINE_STRUCT(Nestle, int a, char *b, int* c)
    FIELD_INT(a);
    FIELD_STR(b);
    FIELD_ARRAY(c);
STRUCT_END

DEFINE_STRUCT(NestedStruct, bool a, Nestle *b, int c)
    FIELD_BOOL(a);
    FIELD_OBJECT(b);
    FIELD_INT(c);
STRUCT_END

DEFINE_STRUCT(ExampleStruct, char *name, NestedStruct *nest, int value)
    FIELD_STR(name);
    FIELD_OBJECT(nest);
    FIELD_INT(value);
STRUCT_END

void remove_spaces(char* s) {
    char* d = s;
    do
        while (isspace(*d)) ++d;
    while ((*s++ = *d++));
}

int main() {
    SerializableArray array = {.size = 2,
    .array = (SerializableValue[]){
        {.value_type = ValueType_Integer, .value = {.integer = 5}},
        {.value_type = ValueType_Bool, .value = {.boolean = false}}
    }};

    Nestle nestle = Nestle_create(/*a: */7,
                                  /*b: */"nestle",
                                  /*c: */&array);

    NestedStruct nest = NestedStruct_create(/*a: */true,
                                            /*b: */&nestle,
                                            /*c: */4);

    ExampleStruct test = ExampleStruct_create(/* name: */"test1",
                                              /* nest: */&nest,
                                              /*value: */5);

    write_to_file("test.json", SerializeableObject_to_JSON(&test));
}
