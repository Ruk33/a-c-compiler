#pragma once

#include <stdlib.h>

#include <src/compiler/token.c>
#include <src/compiler/token_list.c>

#define FOREACH_NODE_TYPE(NODE_TYPE) \
    NODE_TYPE(NODE_TYPE_PROGRAM) \
    NODE_TYPE(NODE_TYPE_VARIABLE_TYPE) \
    NODE_TYPE(NODE_TYPE_VARIABLE_NAME) \
    NODE_TYPE(NODE_TYPE_VARIABLE_DECLARATION) \
    NODE_TYPE(NODE_TYPE_LOCAL_VARIABLE_DECLARATION) \
    NODE_TYPE(NODE_TYPE_STATEMENT) \

#define GENERATE_NODE_TYPE_ENUM(ENUM) ENUM,
#define GENERATE_NODE_TYPE_STRING(STRING) #STRING,

typedef enum NODE_TYPE_ENUM {
    FOREACH_NODE_TYPE(GENERATE_NODE_TYPE_ENUM)
} node_type;

char *NODE_TYPE_STRING[] = {
    FOREACH_NODE_TYPE(GENERATE_NODE_TYPE_STRING)
};

typedef struct node {
    node_type type;
    token *token;
    token_list *extra_tokens;
    struct node *left;
    struct node *right;
    char *error;
} node;

node *new_node(node_type type, token *token, node *left, node *right, char *error) {
    node *new_node = malloc(sizeof(node));

    new_node->type = type;
    new_node->token = token;
    new_node->extra_tokens = NULL;
    new_node->left = left;
    new_node->right = right;
    new_node->error = error;

    return new_node;
}

void node_add_extra_token(node *node, token *extra_token) {
    if (node) {
        if (node->extra_tokens == NULL) {
            node->extra_tokens = new_token_list(NULL);
        }

        token_list_add_token(node->extra_tokens, extra_token);
    }
}

position *node_position(node *node) {
    position *result = NULL;
    token *token;
    position *left_pos;
    position *right_pos;

    if (node && node->token && node->left == NULL && node->right == NULL) {
        token = node->token;

        result = new_position(
            token->position->line_start,
            token->position->line_end,
            token->position->col_start,
            token->position->col_end
        );
    } else if (node && node->left && node->right == NULL) {
        result = node_position(node->left);
    } else if (node && node->right && node->left == NULL) {
        result = node_position(node->right);
    } else if (node && node->left && node->right) {
        left_pos = node_position(node->left);
        right_pos = node_position(node->right);

        result = new_position(
            left_pos->line_start,
            right_pos->line_end,
            left_pos->col_start,
            right_pos->col_end
        );

        free_position(left_pos);
        free_position(right_pos);
    } else {
        result = new_position(0, 0, 0, 0);
    }

    return result;
}

int node_is_valid(node *node) {
    if (node && node->left == NULL && node->right == NULL) {
        return node->error == NULL;
    } else if (node && node->left && node->right == NULL) {
        return node->error == NULL && node_is_valid(node->left);
    } else if (node && node->right && node->left == NULL) {
        return node->error == NULL && node_is_valid(node->right);
    } else if (node && node->left && node->right) {
        return node->error == NULL && node_is_valid(node->left) && node_is_valid(node->right);
    }

    return 0;
}

node *node_first_invalid_node(node *node) {
    struct node *tmp;

    if (node && node->error) {
        return node;
    } else if (node && node->left && node->left->error) {
        return node->left;
    } else if (node && node->right && node->right->error) {
        return node->right;
    } else if (node) {
        tmp = node_first_invalid_node(node->left);
        return tmp ? tmp : node_first_invalid_node(node->right);
    }

    return NULL;
}

size_t node_count_tokens(node *node) {
    size_t result = 0;

    if (node) {
        result += token_list_count(node->extra_tokens);

        if (node->token && node->left == NULL && node->right == NULL) {
            result += 1;
        } else {
            result += node_count_tokens(node->left) + node_count_tokens(node->right);
        }
    }

    return result;
}

token *node_token(node *node) {
    return node ? node->token : NULL;
}

char *node_type_to_string(node_type type) {
    return NODE_TYPE_STRING[type];
}
