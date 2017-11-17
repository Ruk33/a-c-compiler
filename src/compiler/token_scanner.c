#pragma once

#include <stdlib.h>

#include <src/helpers/string.c>
#include <src/compiler/token.c>
#include <src/compiler/token_list.c>

typedef struct {
    string *code;
    string *cursor;
    string *cursor_text;
    size_t col;
    size_t line;
    token_list *tokens;
} token_scanner;

token_scanner *new_token_scanner(char *code) {
    token_scanner *scanner = malloc(sizeof(token_scanner));

    scanner->code = new_string(code);
    scanner->cursor = scanner->code;
    scanner->cursor_text = new_string(NULL);
    scanner->col = 1;
    scanner->line = 1;
    scanner->tokens = new_token_list(NULL);

    return scanner;
}

int token_scanner_has_chars_left(token_scanner *scanner) {
    return scanner ? *(scanner->cursor) : 0;
}

size_t token_scanner_line(token_scanner *scanner) {
    return scanner ? scanner->line : 0;
}

size_t token_scanner_col(token_scanner *scanner) {
    return scanner ? scanner->col : 0;
}

token_list *token_scanner_tokens(token_scanner *scanner) {
    return scanner ? scanner->tokens : NULL;
}

void token_scanner_add_token(token_scanner *tokens, token *token) {
    token_list_add_token(token_scanner_tokens(scanner), token);
}

void token_scanner_advance(token_scanner *scanner, token *token) {
    size_t to_advance;

    if (scanner) {
        to_advance = token ? string_len(token_text(token)) : 1;

        scanner->cursor_text = string_concat_n_chars(scanner->cursor, to_advance);
        scanner->cursor = string_advance_n_chars(scanner->cursor, to_advance);
        scanner->line = token ? position_line_end(token_position(token)) : scanner->line;
        scanner->col = token ? position_col_end(token_position(token)) : scanner->col;
    }
}

string *token_scanner_cursor_text(token_scanner *scanner) {
    return scanner ? scanner->cursor_text : NULL;
}

string *token_scanner_cursor(token_scanner *scanner) {
    return scanner ? scanner->cursor : NULL;
}

void token_scanner_add_token_and_advance(token_scanner *scanner, token *token) {
    if (token) {
        string_clear(token_scanner_cursor_text(scanner));
    }

    token_scanner_add_token(scanner, token);
    token_scanner_advance(scanner, token);
}

void token_scanner_free(token_scanner *scanner) {
    if (scanner) {
        string_free(scanner->code);
        string_free(scanner->cursor_text);
        token_list_free(scanner->tokens);
        free(scanner);
    }
}
