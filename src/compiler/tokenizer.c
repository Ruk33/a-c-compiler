#pragma once

#include <stdlib.h>
#include <ctype.h>
#include <lib/hashmap.h>

#include <src/helpers/string.c>
#include <src/helpers/list.h>
#include <src/compiler/token.c>
#include <src/compiler/position.c>

struct token *tokenizer_line_comment(string *cursor, size_t line, size_t col)
{
    string *comment = new_string(NULL);

    while (*cursor && *cursor != '\n') {
        comment = string_concat_n_chars(comment, cursor, 1);
        cursor = string_advance_n_chars(cursor, 1);
    }

    return new_token(
        comment,
        TOKEN_TYPE_LINE_COMMENT,
        new_position(line, line, col, col + string_len(comment))
    );
}

struct token *tokenizer_multi_line_comment(string *cursor, size_t line, size_t col)
{
    string *comment = new_string(NULL);
    size_t end_line = line;
    size_t end_col = col;

    while (*cursor) {
        comment = string_concat_n_chars(comment, cursor, 1);
        end_col += 1;

        if (*cursor == '\n') {
            end_line += 1;
            end_col = 0;
        }

        if (*(cursor - 1) == '*' && *cursor == '/') {
            break;
        }

        cursor = string_advance_n_chars(cursor, 1);
    }

    return new_token(
        comment,
        TOKEN_TYPE_MULTI_LINE_COMMENT,
        new_position(line, end_line, col, line == end_line ? col + end_col : end_col)
    );
}

struct token *tokenizer_identifier(map_t *keywords, string *cursor, size_t line, size_t col)
{
    string *identifier = new_string(NULL);
    token_type *type = malloc(sizeof(token_type));
    struct token *result;

    while (isalnum(*cursor)) {
        identifier = string_concat_n_chars(identifier, cursor, 1);
        cursor = string_advance_n_chars(cursor, 1);
    }

    if (!keywords || hashmap_get(keywords, identifier, (any_t) type) == MAP_MISSING) {
        *type = TOKEN_TYPE_IDENTIFIER;
    }

    result = new_token(
        identifier,
        *type,
        new_position(line, line, col, col + string_len(identifier))
    );

    free(type);

    return result;
}

struct token *tokenizer_digit(string *cursor, size_t line, size_t col)
{
    string *number = new_string(NULL);

    while (isdigit(*cursor)) {
        number = string_concat_n_chars(number, cursor, 1);
        cursor = string_advance_n_chars(cursor, 1);
    }

    return new_token(
        number,
        TOKEN_TYPE_INTEGER,
        new_position(line, line, col, col + string_len(number))
    );
}

int tokenizer_valid_identifier_char(char ch)
{
    return isalpha(ch) || ch == '_';
}

struct token *tokenizer_token(map_t keywords, string *cursor, size_t line, size_t col)
{
    switch (cursor[0]) {
    case ' ':
    case '\t':
        return NULL;
    case '\n':
        return new_token("\n", TOKEN_TYPE_NL, new_position(line, line + 1, col, 0));
    case '(':
        return new_token("(", TOKEN_TYPE_LPAREN, new_position(line, line, col, col + 1));
    case ')':
        return new_token(")", TOKEN_TYPE_RPAREN, new_position(line, line, col, col + 1));
    case '[':
        return new_token("[", TOKEN_TYPE_LBRACKET, new_position(line, line, col, col + 1));
    case ']':
        return new_token("]", TOKEN_TYPE_RBRACKET, new_position(line, line, col, col + 1));
    case ',':
        return new_token(",", TOKEN_TYPE_COMMA, new_position(line, line, col, col + 1));
    case '*':
        return new_token("*", TOKEN_TYPE_STAR, new_position(line, line, col, col + 1));
    case '-':
        return new_token("-", TOKEN_TYPE_MINUS, new_position(line, line, col, col + 1));
    case '+':
        return new_token("+", TOKEN_TYPE_PLUS, new_position(line, line, col, col + 1));
    case '=':
        if (cursor[1] == '=') return new_token("==", TOKEN_TYPE_EQEQ, new_position(line, line, col, col + 2));
        return new_token("=", TOKEN_TYPE_EQ, new_position(line, line, col, col + 1));
    case '!':
        if (cursor[1] == '=') return new_token("!=", TOKEN_TYPE_NOT_EQ, new_position(line, line, col, col + 2));
        return new_token("!", TOKEN_TYPE_EXCLAMATION, new_position(line, line, col, col + 1));
    case '<':
        if (cursor[1] == '=') return new_token("<=", TOKEN_TYPE_LESS_EQ_THAN, new_position(line, line, col, col + 2));
        return new_token("<", TOKEN_TYPE_LESS_THAN, new_position(line, line, col, col + 1));
    case '>':
        if (cursor[1] == '=') return new_token(">=", TOKEN_TYPE_GREATER_EQ_THAN, new_position(line, line, col, col + 2));
        return new_token(">", TOKEN_TYPE_GREATER_THAN, new_position(line, line, col, col + 1));
    case '/':
        switch (cursor[1]) {
        case '/': return tokenizer_line_comment(cursor, line, col);
        case '*': return tokenizer_multi_line_comment(cursor, line, col);
        default:  return new_token("/", TOKEN_TYPE_SLASH, new_position(line, line, col, col + 1));
        }
    default:
        if (tokenizer_valid_identifier_char(cursor[0])) {
            return tokenizer_identifier(keywords, cursor, line, col);
        } else if (isdigit(cursor[0])) {
            return tokenizer_digit(cursor, line, col);
        }
    }

    printf("Unexpected token %c\n", cursor[0]);

    return NULL;
}

struct list *tokenize(string *code)
{
    map_t keywords = token_keywords();
    struct list *tokens = list_new();
    string *cursor = code;
    size_t line = 0;
    size_t col = 0;

    struct token *token;

    while (*cursor) {
        token = tokenizer_token(keywords, cursor, line, col);

        if (token) {
            line = position_line_end(token_position(token));
            col = position_col_end(token_position(token));
            cursor = string_advance_n_chars(cursor, string_len(token_text(token)));

            list_add_value(tokens, (list_value_t) token);
        } else {
            col += 1;
            cursor = string_advance_n_chars(cursor, 1);
        }
    }

    return tokens;
}
