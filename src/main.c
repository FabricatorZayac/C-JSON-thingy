#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

DEFINE_STRUCT(TestStructInt, int a)
    FIELD_INT(a);
STRUCT_END

DEFINE_STRUCT(TestStructString, char *a, char *b)
    FIELD_STR(a);
    FIELD_STR(b);
STRUCT_END

int main() {
    SerializableArray array = {.size = 2,
    .array = (SerializableValue[]){
        {.value_type = ValueType_Integer, .value.integer = 5},
        {.value_type = ValueType_Bool, .value.boolean = false}
    }};
    /* print_array(array); */

    DoubleNest duonest = DoubleNest_create(/*a: */7,
                                           /*b: */"duonest",
                                           /*c: */&array);

    NestedStruct nest = NestedStruct_create(/*a: */true,
                                            /*b: */&duonest,
                                            /*c: */4);

    ExampleStruct example = ExampleStruct_create(/* name: */"test1",
                                                 /* nest: */&nest,
                                                 /*value: */5);

    write_to_file("test.json", SerializableObject_to_JSON(&example));


    SerializableArray test = {.size = 3,
    .array = (SerializableValue[]){
        /* {.value_type = ValueType_Integer, .value.integer = 10}, */
        /* {.value_type = ValueType_Integer, .value.integer = 4}, */
        {.value_type = ValueType_String, .value.string = "asdf"},
        {.value_type = ValueType_String, .value.string = "qwer"},
        {.value_type = ValueType_String, .value.string = "qwerty"},
        {.value_type = ValueType_String, .value.string = "zxc"}
    }};

    TestStructInt test_int = TestStructInt_create(5);
    TestStructString test_str = TestStructString_create("qwerty", "xc");

    /* char *stringified = read_from_file("test.json"); */
    /* char *stringified = SerializableValue_to_JSON(&(SerializableValue){.value_type = ValueType_String, .value.string = "asdf"}); */
    /* char *stringified = SerializableObject_to_JSON(&test_int); */
    /* char *stringified = SerializableValue_to_JSON(&(SerializableValue){.value_type = ValueType_Integer, .value.integer = 5}); */
    char *stringified = SerializableValue_to_JSON(&(SerializableValue){.value_type = ValueType_Array, .value.array = &test});
    /* char *stringified = SerializableValue_to_JSON(&(SerializableValue){.value_type = ValueType_Bool, .value.boolean = true}); */
    /* printf("json: %s\n", stringified); */
    SerializableValue *result = JSON_to_SerializableValue(stringified);

    printf("%s\n", SerializableValue_to_JSON(result));

    free(result);
    free(stringified);
    return EXIT_SUCCESS;
}
