#pragma once

#include <stdlib.h>
#include <lib/hashmap.h>

#include <src/helpers/string.c>
#include <src/compiler/position.c>

#define FOREACH_TOKEN_TYPE(TOKEN_TYPE) \
    TOKEN_TYPE(TOKEN_TYPE_EXCLAMATION) \
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
    TOKEN_TYPE(TOKEN_TYPE_LINE_COMMENT) \
    TOKEN_TYPE(TOKEN_TYPE_MULTI_LINE_COMMENT) \
    TOKEN_TYPE(TOKEN_TYPE_NL) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_AND) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_OR) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_NULL) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_NOT) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_ARRAY) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_LOCAL) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_SET) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_EXITWHEN) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_CALL) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_GLOBALS) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_END_GLOBALS) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_TYPE) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_NATIVE) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_CONSTANT) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_FUNCTION) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_END_FUNCTION) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_TAKES) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_RETURN) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_RETURNS) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_IF) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_THEN) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_ELSE_IF) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_ELSE) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_END_IF) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_LOOP) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_END_LOOP) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_TRUE) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_FALSE) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_NOTHING) \
    TOKEN_TYPE(TOKEN_TYPE_KEYWORD_EXTENDS) \
    TOKEN_TYPE(TOKEN_TYPE_IDENTIFIER) \
    TOKEN_TYPE(TOKEN_TYPE_STRING) \
    TOKEN_TYPE(TOKEN_TYPE_INTEGER) \
    TOKEN_TYPE(TOKEN_TYPE_REAL) \
    TOKEN_TYPE(TOKEN_TYPE_RAWCODE) \
    TOKEN_TYPE(TOKEN_TYPE_HEXADECIMAL) \
    TOKEN_TYPE(TOKEN_TYPE_INVALID) \

#define GENERATE_TOKEN_TYPE_ENUM(ENUM) ENUM,
#define GENERATE_TOKEN_TYPE_STRING(STRING) #STRING,

typedef enum {
    FOREACH_TOKEN_TYPE(GENERATE_TOKEN_TYPE_ENUM)
} token_type;

char *TOKEN_TYPE_ENUM_STRING[] = {
    FOREACH_TOKEN_TYPE(GENERATE_TOKEN_TYPE_STRING)
};

struct token {
    string *text;
    token_type type;
    struct position *position;
};

struct token *new_token(string *text, token_type type, struct position *position)
{
    struct token *new_token = malloc(sizeof(struct token));

    new_token->text = text;
    new_token->type = type;
    new_token->position = position;

    return new_token;
}

string *token_text(struct token *token)
{
    return token ? token->text : NULL;
}

int token_type_matches(struct token *token, token_type type)
{
    return token ? token->type == type : 0;
}

char *token_type_to_string(token_type type)
{
    return TOKEN_TYPE_ENUM_STRING[type];
}

struct position *token_position(struct token *token)
{
    return token ? token->position : NULL;
}

void token_free(struct token *token)
{
    if (token) {
        string_free(token->text);
        position_free(token->position);
        free(token);
    }
}

map_t *token_keywords()
{
    map_t *tm = hashmap_new();

    if (tm) {
        hashmap_put(tm, "and", (any_t) TOKEN_TYPE_KEYWORD_AND);
        hashmap_put(tm, "or", (any_t) TOKEN_TYPE_KEYWORD_OR);
        hashmap_put(tm, "null", (any_t) TOKEN_TYPE_KEYWORD_NULL);
        hashmap_put(tm, "not", (any_t) TOKEN_TYPE_KEYWORD_NOT);
        hashmap_put(tm, "array", (any_t) TOKEN_TYPE_KEYWORD_ARRAY);
        hashmap_put(tm, "local", (any_t) TOKEN_TYPE_KEYWORD_LOCAL);
        hashmap_put(tm, "set", (any_t) TOKEN_TYPE_KEYWORD_SET);
        hashmap_put(tm, "exitwhen", (any_t) TOKEN_TYPE_KEYWORD_EXITWHEN);
        hashmap_put(tm, "call", (any_t) TOKEN_TYPE_KEYWORD_CALL);
        hashmap_put(tm, "globals", (any_t) TOKEN_TYPE_KEYWORD_GLOBALS);
        hashmap_put(tm, "endglobals", (any_t) TOKEN_TYPE_KEYWORD_END_GLOBALS);
        hashmap_put(tm, "type", (any_t) TOKEN_TYPE_KEYWORD_TYPE);
        hashmap_put(tm, "native", (any_t) TOKEN_TYPE_KEYWORD_NATIVE);
        hashmap_put(tm, "constant", (any_t) TOKEN_TYPE_KEYWORD_CONSTANT);
        hashmap_put(tm, "function", (any_t) TOKEN_TYPE_KEYWORD_FUNCTION);
        hashmap_put(tm, "endfunction", (any_t) TOKEN_TYPE_KEYWORD_END_FUNCTION);
        hashmap_put(tm, "takes", (any_t) TOKEN_TYPE_KEYWORD_TAKES);
        hashmap_put(tm, "return", (any_t) TOKEN_TYPE_KEYWORD_RETURN);
        hashmap_put(tm, "returns", (any_t) TOKEN_TYPE_KEYWORD_RETURNS);
        hashmap_put(tm, "if", (any_t) TOKEN_TYPE_KEYWORD_IF);
        hashmap_put(tm, "then", (any_t) TOKEN_TYPE_KEYWORD_THEN);
        hashmap_put(tm, "elseif", (any_t) TOKEN_TYPE_KEYWORD_ELSE_IF);
        hashmap_put(tm, "else", (any_t) TOKEN_TYPE_KEYWORD_ELSE);
        hashmap_put(tm, "endif", (any_t) TOKEN_TYPE_KEYWORD_END_IF);
        hashmap_put(tm, "loop", (any_t) TOKEN_TYPE_KEYWORD_LOOP);
        hashmap_put(tm, "endloop", (any_t) TOKEN_TYPE_KEYWORD_END_LOOP);
        hashmap_put(tm, "true", (any_t) TOKEN_TYPE_KEYWORD_TRUE);
        hashmap_put(tm, "false", (any_t) TOKEN_TYPE_KEYWORD_FALSE);
        hashmap_put(tm, "nothing", (any_t) TOKEN_TYPE_KEYWORD_NOTHING);
        hashmap_put(tm, "extends", (any_t) TOKEN_TYPE_KEYWORD_EXTENDS);
    }

    return tm;
}
