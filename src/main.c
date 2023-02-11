#include <stdbool.h>
#include <stddef.h>
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
    JsonArray array = JsonArray(
        JsonValue(ValueType_Integer, 5),
        JsonValue(ValueType_Bool, false));

    DoubleNest duonest = DoubleNest_create(/*a: */ 7,
                                           /*b: */ "duonest",
                                           /*c: */ &array);

    NestedStruct nest = NestedStruct_create(/*a: */ true,
                                            /*b: */ &duonest,
                                            /*c: */ 4);

    ExampleStruct example = ExampleStruct_create(/* name: */ "test1",
                                                 /* nest: */ &nest,
                                                 /*value: */ 5);

    write_to_file("test.json", JsonObject_stringify(&example));

    char *stringified = read_from_file("test.json");

    printf("json:\t%s\n", stringified);
    JsonValue result = JSON_parse(stringified);

    printf("result:\t%s\n", JsonValue_stringify(&result));

    printf("%d\n", example.fields[1].value.body.object->fields[2].value.body.integer);

    JsonValue_destroy(&result);
    JsonObject_destroy(&example);
    free(stringified);
    return EXIT_SUCCESS;
}

