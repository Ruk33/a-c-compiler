#pragma once

#include <stdlib.h>

#include <src/compiler/token.c>
#include <src/compiler/config.c>

typedef struct {
    token_list *tokens;
    token_list *cursor;
    config *config;
} parser_scanner;

parser_scanner *new_parser_scanner(token_list *tokens, config *config) {
    parser_scanner *scanner = malloc(sizeof(parser_scanner));

    scanner->tokens = tokens ? tokens : new_token_list(NULL);
    scanner->cursor = scanner->tokens;
    scanner->config = config;

    return scanner;
}

void parser_scanner_advance_nth_times(parser_scanner *scanner, size_t times) {
    while (scanner && scanner->cursor && scanner->cursor->next && times > 0) {
        scanner->cursor = scanner->cursor->next;
        times--;
    }
}

void parser_scanner_update_from_new_node(parser_scanner *scanner, node *node) {
    parser_scanner_advance_nth_times(scanner, node_count_tokens(node));
}

void parser_scanner_sync_if_required(parser_scanner *scanner, node *node) {
    if (node && !node_is_valid(node)) {
        // TODO
        // advance until any statement token
    }
}

token *parser_scanner_get_nth_token_from_cursor(parser_scanner *scanner, size_t nth) {
    return scanner ? next_nth_token_from_list(scanner->cursor, nth) : NULL;
}

token *parser_scanner_get_cursor_token(parser_scanner *scanner) {
    return scanner ? token_from_list(scanner->cursor) : NULL;
}

token *parser_scanner_get_cursor_token_and_advance(parser_scanner *scanner) {
    token *token = parser_scanner_get_cursor_token(scanner);
    parser_scanner_advance_nth_times(scanner, 1);
    return token;
}

config *parser_scanner_config(parser_scanner *scanner) {
    return scanner ? scanner->config : NULL;
}
