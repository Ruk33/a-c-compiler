#pragma once

#include <stdlib.h>

#include <src/compiler/token.c>

typedef struct token_list {
    token *token;
    struct token_list *tail;
    struct token_list *next;
} token_list;

token_list *new_token_list(token *token) {
    token_list *tokens = malloc(sizeof(token_list));

    tokens->token = token;
    tokens->tail = tokens;
    tokens->next = NULL;

    return tokens;
}

token_list *token_list_next_node(token_list *tokens) {
    return tokens ? tokens->next : NULL;
}

token *token_list_token(token_list *tokens) {
    return tokens ? tokens->token : NULL;
}

token *token_list_next_token(token_list *tokens) {
    return token_list_token(token_list_next_node(tokens));
}

token *token_list_last_token(token_list *tokens) {
    return tokens ? token_list_token(tokens->tail) : NULL;
}

void token_list_add_token(token_list *tokens, token *token) {
    if (tokens && tokens->tail && token) {
        tokens->tail->next = new_token_list(token);
        tokens->tail = tokens->tail->next;
    }
}

size_t token_list_count(token_list *tokens) {
    size_t result = 0;
    token_list *cursor = tokens;

    while (cursor) {
        if (cursor->token) {
            result += 1;
        }

        cursor = cursor->next;
    }

    return result;
}

token_list *token_list_clone(token_list *tokens) {
    token_list *result = new_token_list(token_list_token(tokens));

    while (*tokens) {
        tokens = tokens->next;
        token_list_last_token(result, token_list_token(tokens));
    }

    return result;
}

void token_list_free(token_list *tokens) {
    token_list *next;

    if (tokens) {
        while (*tokens) {
            next = tokens->next;
            token_free(tokens->token);
            tokens = next;
        }
    }
}
