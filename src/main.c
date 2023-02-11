#include <stdbool.h>
#include <stdio.h>

#include "file_ops.h"
#include "parser.h"
#include "serializable.h"

DEFINE_STRUCT(DoubleNest, int a, char *b, int* c)
    FIELD_INT(a);
    FIELD_STR(b);
    FIELD_ARRAY(c);
STRUCT_END

DEFINE_STRUCT(NestedStruct, bool a, DoubleNest *b, int c)
    FIELD_BOOL(a);
    FIELD_OBJECT(b);
    FIELD_INT(c);
STRUCT_END

DEFINE_STRUCT(ExampleStruct, char *name, NestedStruct *nest, int value)
    FIELD_STR(name);
    FIELD_OBJECT(nest);
    FIELD_INT(value);
STRUCT_END

int main() {
    SerializableArray array = SerializableArray_create(
        2,
        SerializableValue_create(ValueType_Integer, 5),
        SerializableValue_create(ValueType_Bool, false));

    DoubleNest duonest = DoubleNest_create(/*a: */ 7,
                                           /*b: */ "duonest",
                                           /*c: */ &array);

    NestedStruct nest = NestedStruct_create(/*a: */ true,
                                            /*b: */ &duonest,
                                            /*c: */ 4);

    ExampleStruct example = ExampleStruct_create(/* name: */ "test1",
                                                 /* nest: */ &nest,
                                                 /*value: */ 5);

    write_to_file("test.json", SerializableObject_to_JSON(&example));

    char *stringified = read_from_file("test.json");

    printf("json:\t%s\n", stringified);
    SerializableValue result = JSON_to_SerializableValue(stringified);

    printf("result:\t%s\n", SerializableValue_to_JSON(&result));

    SerializableValue_destroy(&result);
    SerializableObject_destroy(&example);
    free(stringified);
    return EXIT_SUCCESS;
}
