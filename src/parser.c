#include "parser.h"
#include "serializable.h"
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Integer_state(char c);

/* It'll be a miracle if this thing works at all */
/* NOTE it works */
/* TODO benchmark */
/* TODO make parser not static, and optimize in general */

enum TokenType { Token_Field, Token_Value, Token_Key, Token_Boundary };
typedef union {
    Field field;
    JsonValue value;
    char *key;
    char boundary;
} Token;
typedef struct {
    enum TokenType token_type;
    Token token;
} ParserToken;

enum ParserState {
    Parser_String,
    Parser_Integer,
    Parser_Bool,
    Parser_Key,
    Parser_Value,
    Parser_Separator,
    Parser_Boundary,
    Parser_Keyseeker,
};
typedef struct {
    enum ParserState state;

    size_t buffer_size;
    char buffer[BUFSIZ];

    size_t tokens_size;
    ParserToken tokens[BUFSIZ];
} Parser;

static Parser parser = { .state = Parser_Value, .buffer_size = 0, .tokens_size = 0 };

void Value_state(char c) {
    switch (c) {
    case '{':
        parser.state = Parser_Keyseeker;
        /* Fallthrough */
    case '[':
        parser.tokens[parser.tokens_size++] = (ParserToken) {.token_type = Token_Boundary, .token.boundary = c};
        break;
    case '"':
        parser.state = Parser_String;
        break;
    case 't':
    case 'f':
        parser.state = Parser_Bool;
        break;
    default:
        if (isdigit(c) || c == '-') {
            parser.state = Parser_Integer;
        }
        break;
    }
}

void Key_state(char c) {
    if (c == '"' && parser.buffer[parser.buffer_size - 1] != '\\') {
        parser.tokens[parser.tokens_size].token_type = Token_Key;
        parser.tokens[parser.tokens_size++].token =
            (Token){.key = strncpy(calloc(1, parser.buffer_size),
                                   parser.buffer, parser.buffer_size)};

        parser.buffer_size = 0;
        memset(parser.buffer, 0, BUFSIZ);

        parser.state = Parser_Separator;
    } else {
        parser.buffer[parser.buffer_size++] = c;
    }
}

void String_state(char c) {
    if (c == '"' && parser.buffer[parser.buffer_size - 1] != '\\') {
        parser.tokens[parser.tokens_size].token_type = Token_Value;
        parser.tokens[parser.tokens_size++].token = (Token){
            .value = JsonValue(ValueType_String,
                               strncpy(calloc(1, parser.buffer_size + 1),
                                       parser.buffer, parser.buffer_size))};

        parser.buffer_size = 0;
        memset(parser.buffer, 0, BUFSIZ);

        parser.state = Parser_Boundary;
        return;
    } else {
        parser.buffer[parser.buffer_size++] = c;
    }
}

void Boundary_state(char c) {
    if (parser.tokens[parser.tokens_size - 2].token_type == Token_Key
        && parser.tokens[parser.tokens_size - 1].token_type == Token_Value) {
        Field result = (Field){
            .key = parser.tokens[parser.tokens_size - 2].token.key,
            .value = parser.tokens[parser.tokens_size - 1].token.value};
        memset(parser.tokens + parser.tokens_size - 2, 0, sizeof(ParserToken) * 2);
        parser.tokens_size -= 2;
        parser.tokens[parser.tokens_size].token_type = Token_Field;
        parser.tokens[parser.tokens_size++].token.field = result;
    }
    switch (c) {
    case '}': {
            size_t i, object_size = 0;
            // Can be made safer by type checking for boundary, but I am tired and don't give a shit
            for (i = parser.tokens_size - 1; parser.tokens[i].token.boundary != '{'; i--)
                if (parser.tokens[i].token_type == Token_Field) object_size++;
            size_t object_start = i;

            JsonValue result = JsonValue(
                ValueType_Object,
                calloc(1, sizeof(JsonObject) + object_size * sizeof(Field)));
            result.body.object->size = object_size;

            for (size_t j = 0; j < object_size; j++, i++) {
                result.body.object->fields[j] = parser.tokens[i + 1].token.field;
            }
            memset(parser.tokens + object_start, 0, sizeof(ParserToken) * object_size);
            parser.tokens_size = object_start;
            parser.tokens[parser.tokens_size].token_type = Token_Value;
            parser.tokens[parser.tokens_size++].token.value = result;
        }
        break;
    case ']': {
            size_t i, array_size = 0;
            for (i = parser.tokens_size; parser.tokens[i].token.boundary != '['; i--)
                if (parser.tokens[i].token_type == Token_Value) array_size++;
            size_t array_start = i;

            JsonValue result =
                JsonValue(ValueType_Array, calloc(1, sizeof(JsonArray)));
            result.body.array->size = array_size;
            result.body.array->body = calloc(array_size, sizeof(JsonValue));

            for (size_t j = 0; j < array_size; i++, j++) {
                result.body.array->body[j] = parser.tokens[i + 1].token.value;
            }
            memset(parser.tokens + array_start, 0, sizeof(ParserToken) * array_size);
            parser.tokens_size = array_start;
            parser.tokens[parser.tokens_size].token_type = Token_Value;
            parser.tokens[parser.tokens_size++].token.value = result;
        }
        break;
    case ',':
        for (size_t i = parser.tokens_size - 1; i >= 0; i--) {
            if (parser.tokens[i].token_type == Token_Boundary) {
                if (parser.tokens[i].token.boundary == '[') {
                    parser.state = Parser_Value;
                    return;
                } else if (parser.tokens[i].token.boundary == '{') {
                    parser.state = Parser_Keyseeker;
                    return;
                }
            }
        }
        break;
    }
}

JsonValue JSON_parse(char *json) {
    for (size_t i = 0; i < strlen(json); i++) {
        switch (parser.state) {
        case Parser_Value:
            Value_state(json[i]);
            if (parser.state == Parser_Integer || parser.state == Parser_Bool)
                i--;
            break;
        case Parser_String:
            String_state(json[i]);
            break;
        case Parser_Separator:
            parser.state = Parser_Value;
            break;
        case Parser_Key:
            Key_state(json[i]);
            break;
        case Parser_Boundary:
            Boundary_state(json[i]);
            break;
        case Parser_Integer:
            /* Integer_state(json[i]); */
            parser.tokens[parser.tokens_size++] = (ParserToken){
                .token_type = Token_Value,
                .token.value = JsonValue(ValueType_Integer, atoi(json + i))};
            parser.state = Parser_Boundary;
            break;
        case Parser_Bool:
            if (strstr(json + i, "true") == json + i) {
                parser.tokens[parser.tokens_size++] = (ParserToken){
                    .token_type = Token_Value,
                    .token.value = JsonValue(ValueType_Bool, true)};
            } else if (strstr(json + i, "false") == json + i) {
                parser.tokens[parser.tokens_size++] = (ParserToken){
                    .token_type = Token_Value,
                    .token.value = JsonValue(ValueType_Bool, false)};
            }
            parser.state = Parser_Boundary;
            break;
        case Parser_Keyseeker:
            if (json[i] == '"') parser.state = Parser_Key;
            break;
        }
    }

    JsonValue result = parser.tokens[0].token.value;
    memset(&parser, 0, sizeof(parser));
    parser = (Parser){ .state = Parser_Value, .buffer_size = 0, .tokens_size = 0 };
    return result;
}
