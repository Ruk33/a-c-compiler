#pragma once

#include <stdlib.h>
#include <lib/hashmap.h>

#include <src/helpers/string.c>
#include <src/compiler/position.c>

#define FOREACH_TOKEN_TYPE(TOKEN_TYPE) \
    TOKEN_TYPE(TOKEN_TYPE_LPAREN) \
    TOKEN_TYPE(TOKEN_TYPE_RPAREN) \
    TOKEN_TYPE(TOKEN_TYPE_LBRACKET) \
    TOKEN_TYPE(TOKEN_TYPE_RBRACKET) \
    TOKEN_TYPE(TOKEN_TYPE_COMMA) \
    TOKEN_TYPE(TOKEN_TYPE_SLASH) \
    TOKEN_TYPE(TOKEN_TYPE_STAR) \
    TOKEN_TYPE(TOKEN_TYPE_MINUS) \
    TOKEN_TYPE(TOKEN_TYPE_PLUS) \
    TOKEN_TYPE(TOKEN_TYPE_EQ) \
    TOKEN_TYPE(TOKEN_TYPE_EQEQ) \
    TOKEN_TYPE(TOKEN_TYPE_NOT_EQ) \
    TOKEN_TYPE(TOKEN_TYPE_LESS_THAN) \
    TOKEN_TYPE(TOKEN_TYPE_LESS_EQ_THAN) \
    TOKEN_TYPE(TOKEN_TYPE_GREATER_THAN) \
    TOKEN_TYPE(TOKEN_TYPE_GREATER_EQ_THAN) \
    TOKEN_TYPE(TOKEN_TYPE_AND) \
    TOKEN_TYPE(TOKEN_TYPE_OR) \
    TOKEN_TYPE(TOKEN_TYPE_NULL) \
    TOKEN_TYPE(TOKEN_TYPE_NOT) \
    TOKEN_TYPE(TOKEN_TYPE_ARRAY) \
    TOKEN_TYPE(TOKEN_TYPE_LOCAL) \
    TOKEN_TYPE(TOKEN_TYPE_SET) \
    TOKEN_TYPE(TOKEN_TYPE_EXITWHEN) \
    TOKEN_TYPE(TOKEN_TYPE_CALL) \
    TOKEN_TYPE(TOKEN_TYPE_GLOBALS) \
    TOKEN_TYPE(TOKEN_TYPE_END_GLOBALS) \
    TOKEN_TYPE(TOKEN_TYPE_TYPE) \
    TOKEN_TYPE(TOKEN_TYPE_NATIVE) \
    TOKEN_TYPE(TOKEN_TYPE_CONSTANT) \
    TOKEN_TYPE(TOKEN_TYPE_FUNCTION) \
    TOKEN_TYPE(TOKEN_TYPE_END_FUNCTION) \
    TOKEN_TYPE(TOKEN_TYPE_TAKES) \
    TOKEN_TYPE(TOKEN_TYPE_RETURN) \
    TOKEN_TYPE(TOKEN_TYPE_RETURNS) \
    TOKEN_TYPE(TOKEN_TYPE_IF) \
    TOKEN_TYPE(TOKEN_TYPE_THEN) \
    TOKEN_TYPE(TOKEN_TYPE_ELSE_IF) \
    TOKEN_TYPE(TOKEN_TYPE_ELSE) \
    TOKEN_TYPE(TOKEN_TYPE_END_IF) \
    TOKEN_TYPE(TOKEN_TYPE_LOOP) \
    TOKEN_TYPE(TOKEN_TYPE_END_LOOP) \
    TOKEN_TYPE(TOKEN_TYPE_TRUE) \
    TOKEN_TYPE(TOKEN_TYPE_FALSE) \
    TOKEN_TYPE(TOKEN_TYPE_NOTHING) \
    TOKEN_TYPE(TOKEN_TYPE_EXTENDS) \
    TOKEN_TYPE(TOKEN_TYPE_IDENTIFIER) \
    TOKEN_TYPE(TOKEN_TYPE_STRING) \
    TOKEN_TYPE(TOKEN_TYPE_INTEGER) \
    TOKEN_TYPE(TOKEN_TYPE_REAL) \
    TOKEN_TYPE(TOKEN_TYPE_RAWCODE) \
    TOKEN_TYPE(TOKEN_TYPE_HEXADECIMAL) \
    TOKEN_TYPE(TOKEN_TYPE_LINE_COMMENT_START) \
    TOKEN_TYPE(TOKEN_TYPE_MULTI_LINE_COMMENT_START) \
    TOKEN_TYPE(TOKEN_TYPE_MULTI_LINE_COMMENT_END) \
    TOKEN_TYPE(TOKEN_TYPE_NL) \
    TOKEN_TYPE(TOKEN_TYPE_INVALID) \

#define GENERATE_TOKEN_TYPE_ENUM(ENUM) ENUM,
#define GENERATE_TOKEN_TYPE_STRING(STRING) #STRING,

typedef enum {
    FOREACH_TOKEN_TYPE(GENERATE_TOKEN_TYPE_ENUM)
} token_type;

char *TOKEN_TYPE_ENUM_STRING[] = {
    FOREACH_TOKEN_TYPE(GENERATE_TOKEN_TYPE_STRING)
};

typedef struct {
    string *text;
    token_type type;
    position *position;
} token;

token *new_token(char *text, token_type type, position *position) {
    token *new_token = malloc(sizeof(token));

    new_token->text = new_string(text);
    new_token->type = type;
    new_token->position = position;

    return new_token;
}

string *token_text(token *token) {
    return token ? token->text : NULL;
}

int token_type_matches(token *token, token_type type) {
    return token ? token->type == type : 0;
}

char *token_type_to_string(token_type type) {
    return TOKEN_TYPE_ENUM_STRING[type];
}

position *token_position(token *token) {
    return token ? token->position : NULL;
}

void token_free(token *token) {
    if (token) {
        string_free(token->text);
        position_free(token->position);
        free(token);
    }
}

map_t *token_map() {
    map_t *tm = hashmap_new();

    if (tm) {
        hashmap_put(tm, "(", (any_t) TOKEN_TYPE_LPAREN);
        hashmap_put(tm, ")", (any_t) TOKEN_TYPE_RPAREN);
        hashmap_put(tm, "{", (any_t) TOKEN_TYPE_LBRACKET);
        hashmap_put(tm, "}", (any_t) TOKEN_TYPE_RBRACKET);
        hashmap_put(tm, ",", (any_t) TOKEN_TYPE_COMMA);
        hashmap_put(tm, "/", (any_t) TOKEN_TYPE_SLASH);
        hashmap_put(tm, "*", (any_t) TOKEN_TYPE_STAR);
        hashmap_put(tm, "-", (any_t) TOKEN_TYPE_MINUS);
        hashmap_put(tm, "+", (any_t) TOKEN_TYPE_PLUS);
        hashmap_put(tm, "=", (any_t) TOKEN_TYPE_EQ);
        hashmap_put(tm, "==", (any_t) TOKEN_TYPE_EQEQ);
        hashmap_put(tm, "!=", (any_t) TOKEN_TYPE_NOT_EQ);
        hashmap_put(tm, "<", (any_t) TOKEN_TYPE_LESS_THAN);
        hashmap_put(tm, "<=", (any_t) TOKEN_TYPE_LESS_EQ_THAN);
        hashmap_put(tm, ">", (any_t) TOKEN_TYPE_GREATER_THAN);
        hashmap_put(tm, ">=", (any_t) TOKEN_TYPE_GREATER_EQ_THAN);
        hashmap_put(tm, "and", (any_t) TOKEN_TYPE_AND);
        hashmap_put(tm, "or", (any_t) TOKEN_TYPE_OR);
        hashmap_put(tm, "null", (any_t) TOKEN_TYPE_NULL);
        hashmap_put(tm, "not", (any_t) TOKEN_TYPE_NOT);
        hashmap_put(tm, "array", (any_t) TOKEN_TYPE_ARRAY);
        hashmap_put(tm, "local", (any_t) TOKEN_TYPE_LOCAL);
        hashmap_put(tm, "set", (any_t) TOKEN_TYPE_SET);
        hashmap_put(tm, "exitwhen", (any_t) TOKEN_TYPE_EXITWHEN);
        hashmap_put(tm, "call", (any_t) TOKEN_TYPE_CALL);
        hashmap_put(tm, "globals", (any_t) TOKEN_TYPE_GLOBALS);
        hashmap_put(tm, "endglobals", (any_t) TOKEN_TYPE_END_GLOBALS);
        hashmap_put(tm, "type", (any_t) TOKEN_TYPE_TYPE);
        hashmap_put(tm, "native", (any_t) TOKEN_TYPE_NATIVE);
        hashmap_put(tm, "constant", (any_t) TOKEN_TYPE_CONSTANT);
        hashmap_put(tm, "function", (any_t) TOKEN_TYPE_FUNCTION);
        hashmap_put(tm, "endfunction", (any_t) TOKEN_TYPE_END_FUNCTION);
        hashmap_put(tm, "takes", (any_t) TOKEN_TYPE_TAKES);
        hashmap_put(tm, "return", (any_t) TOKEN_TYPE_RETURN);
        hashmap_put(tm, "returns", (any_t) TOKEN_TYPE_RETURNS);
        hashmap_put(tm, "if", (any_t) TOKEN_TYPE_IF);
        hashmap_put(tm, "then", (any_t) TOKEN_TYPE_THEN);
        hashmap_put(tm, "elseif", (any_t) TOKEN_TYPE_ELSE_IF);
        hashmap_put(tm, "else", (any_t) TOKEN_TYPE_ELSE);
        hashmap_put(tm, "endif", (any_t) TOKEN_TYPE_END_IF);
        hashmap_put(tm, "loop", (any_t) TOKEN_TYPE_LOOP);
        hashmap_put(tm, "endloop", (any_t) TOKEN_TYPE_END_LOOP);
        hashmap_put(tm, "true", (any_t) TOKEN_TYPE_TRUE);
        hashmap_put(tm, "false", (any_t) TOKEN_TYPE_FALSE);
        hashmap_put(tm, "nothing", (any_t) TOKEN_TYPE_NOTHING);
        hashmap_put(tm, "extends", (any_t) TOKEN_TYPE_EXTENDS);
        hashmap_put(tm, "//", (any_t) TOKEN_TYPE_LINE_COMMENT_START);
        hashmap_put(tm, "/*", (any_t) TOKEN_TYPE_MULTI_LINE_COMMENT_START);
        hashmap_put(tm, "*/", (any_t) TOKEN_TYPE_MULTI_LINE_COMMENT_END);
        hashmap_put(tm, "\n", (any_t) TOKEN_TYPE_NL);
    }

    return tm;
}
