#pragma once

#include <stdlib.h>
#include <lib/hashmap.h>

#include <src/helpers/string.c>
#include <src/compiler/token.c>
#include <src/compiler/token_list.c>
#include <src/compiler/position.c>
#include <src/compiler/token_scanner.c>

token *tokenizer_multi_line_comment(string *text, size_t line, size_t col) {

}

token *tokenizer_token(token_scanner *scanner, map_t *tokens) {
    string *cursor_text = token_scanner_cursor_text(scanner);
    string *cursor = token_scanner_cursor(scanner);
    size_t line = token_scanner_line(scanner);
    size_t col = token_scanner_col(scanner);
    token_type type;

    if (hashmap_get(tokens, cursor_text, (any_t) &type) == MAP_OK) {
        switch (type) {
            case TOKEN_TYPE_NL:
                return new_token(cursor_text, TOKEN_TYPE_NL, new_position(line, line + 1, col, 1));
            case TOKEN_TYPE_MULTI_LINE_COMMENT_START:
                return tokenizer_multi_line_comment(cursor, line, col);
        }
    }

    // create a new string to perform string operations, DO NOT MUTATE the scanner
    // check if last char is space = isspace(cursor_text[strlen(cursor_text) - 1])
    // if it is, cut it cursor_text[strlen(cursor_text) - 1] = 0
    // and check for keywords
    // if no keywords, check for identifier or digit
    // delegate string operations to string.c

    return NULL;
}

token_list *tokenize(char *code) {
    map_t *tokens_map = token_map();
    token_scanner *scanner = new_token_scanner(code);
    token_list *tokens;

    while (token_scanner_has_chars_left(scanner)) {
        token_scanner_add_token_and_advance(
            scanner,
            tokenizer_token(scanner, tokens_map)
        );
    }

    tokens = token_list_clone(token_scanner_tokens(scanner));

    hashmap_free(tokens_map);
    token_scanner_free(scanner);

    return tokens;
}
