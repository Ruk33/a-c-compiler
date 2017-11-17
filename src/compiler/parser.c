#pragma once

// do not use parser_scanner, just use token_list and pass the config instance
// try to change the code so that it can work with many threads (functional style)

#include <stdlib.h>

#include <src/compiler/token.c>
#include <src/compiler/parser_scanner.c>
#include <src/compiler/node.c>
#include <src/compiler/error_message.c>

char *token_matches_or_error(token *token, token_type type, char *error) {
    return token_type_matches(token, type) ? NULL : error;
}

char *node_matches_or_error(node *node, node_type type, char *error) {
    return node && node->type == type ? NULL : error;
}

node *parser_new_variable_type_node(parser_scanner *scanner) {
    token *token = parser_scanner_get_cursor_token(scanner);
    char *error = token_matches_or_error(token, TOKEN_TYPE_IDENTIFIER, error_variable_type(parser_scanner_config(scanner)));
    node *result =  new_node(NODE_TYPE_VARIABLE_TYPE, token, NULL, NULL, error);

    parser_scanner_update_from_new_node(scanner, result);

    return result;
}

node *parser_new_variable_name_node(parser_scanner *scanner) {
    token *token = parser_scanner_get_cursor_token(scanner);
    char *error = token_matches_or_error(token, TOKEN_TYPE_IDENTIFIER, error_variable_name(parser_scanner_config(scanner)));
    node *result = new_node(NODE_TYPE_VARIABLE_NAME, token, NULL, NULL, error);

    parser_scanner_update_from_new_node(scanner, result);

    return result;
}

node *parser_new_variable_declaration_node(parser_scanner *scanner) {
    node *type = parser_new_variable_type_node(scanner);
    node *name = parser_new_variable_name_node(scanner);
    char *type_error = node_matches_or_error(type, NODE_TYPE_VARIABLE_TYPE, error_variable_type(parser_scanner_config(scanner)));
    char *name_error = node_matches_or_error(name, NODE_TYPE_VARIABLE_NAME, error_variable_name(parser_scanner_config(scanner)));
    char *error = type_error ? type_error : name_error;

    return new_node(NODE_TYPE_VARIABLE_DECLARATION, node_token(name), type, name, error);
}

node *parser_new_local_variable_declaration_node(parser_scanner *scanner) {
    token *local_keyword = parser_scanner_get_cursor_token_and_advance(scanner);
    node *variable = parser_new_variable_declaration_node(scanner);
    char *error = token_matches_or_error(local_keyword, TOKEN_TYPE_LOCAL, error_local_variable(parser_scanner_config(scanner)));
    node *result = new_node(NODE_TYPE_LOCAL_VARIABLE_DECLARATION, node_token(variable), variable, NULL, error);

    node_add_extra_token(result, local_keyword);

    return result;
}

node *parser_new_statement_node(parser_scanner *scanner) {
    token *token = parser_scanner_get_cursor_token(scanner);
    node *statement = NULL;

    if (token_type_matches(token, TOKEN_TYPE_LOCAL)) {
        statement = parser_new_local_variable_declaration_node(scanner);
    }

    if (statement) {
        statement = new_node(NODE_TYPE_STATEMENT, NULL, statement, NULL, NULL);
        parser_scanner_sync_if_required(scanner, statement);
    }

    return statement;
}

node *parse(token_list *tokens, config *config) {
    return parser_new_statement_node(new_parser_scanner(tokens, config));
}
