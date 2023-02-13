#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "file_ops.h"
#include "parser.h"
#include "serializable.h"

DEFINE_STRUCT(DoubleNest, int a, char *b, JsonArray c)
    FIELD(Number, a);
    FIELD(String, b);
    FIELD(Array, c);
STRUCT_END

DEFINE_STRUCT(NestedStruct, bool a, DoubleNest *b, int c)
    FIELD(Bool, a);
    FIELD(Object, b);
    FIELD(Number, c);
STRUCT_END

DEFINE_STRUCT(ExampleStruct, char *name, NestedStruct *nest, int value)
    FIELD(String, name);
    FIELD(Object, nest);
    FIELD(Number, value);
STRUCT_END

int main() {
    JsonArray array = JsonArray(
        JsonValue(ValueType_Number, 10),
        JsonValue(ValueType_Bool, false));

    DoubleNest_json duonest = DoubleNest_create(/*a: */ 7,
                                                /*b: */ "duonest",
                                                /*c: */ array);

    NestedStruct_json nest = NestedStruct_create(/*a: */ true,
                                                 /*b: */ &duonest,
                                                 /*c: */ 4);

    ExampleStruct_json example = ExampleStruct_create(/* name: */ "test1",
                                                      /* nest: */ &nest,
                                                      /*value: */ 5);

    char *stringified = JsonObject_stringify(&example);
    printf("\t\t%s\n", stringified);
    write_to_file("test.json", stringified);
    free(stringified);

    stringified = read_from_file("test.json");
    printf("json:\t%s\n", stringified);
    JsonValue result = JSON_parse(stringified);
    free(stringified);

    stringified = JsonValue_stringify(&result);

    printf("result:\t%s\n", stringified);
    free(stringified);

    ExampleStruct *ex = JsonObject_to_struct(&example);
    assert(ex->nest->c ==
           example.fields[1].value.body.object->fields[2].value.body.number);


    // NOTE clean the memory leaking from JsonObject_to_struct
    free(ex->nest->b);
    free(ex->nest);
    free(ex);

    /* DoubleNest_json dn = DoubleNest_json_from_struct(&(DoubleNest) {5, "db", &array}); */
    /* printf("%s\n", JsonObject_stringify(&dn)); */

    /* JsonValue_clean(&result); */
    /* JsonObject_clean(&example); */

    return EXIT_SUCCESS;
}
