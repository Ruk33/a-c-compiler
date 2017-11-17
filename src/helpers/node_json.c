#pragma once

#include <stdlib.h>

#include <src/compiler/node.c>
#include <src/helpers/str.c>
#include <src/helpers/str_buffer.c>

str_buffer *node_to_json(node *node) {
    str_buffer *result;
    char *type2char;
    str_buffer *left;
    str_buffer *right;

    if (node) {
        result = new_empty_str_buffer("");

        type2char = malloc(sizeof(char) * (node->type + 1));

        if (type2char) {
            snprintf(type2char, node->type, "%d", node->type);
        } else {
            type2char = new_string("?");
        }

        left = node_to_json(node->left);
        right = node_to_json(node->right);

        str_buffer_append_str(result, "{");

        str_buffer_append_str(result, "\"text\": ");
        str_buffer_append_str(result, "\"");
        str_buffer_append_str(result, node->token ? node->token->text : "?");
        str_buffer_append_str(result, "\",");

        str_buffer_append_str(result, "\"type\": ");
        str_buffer_append_str(result, "\"");
        str_buffer_append_str(result, type2char);
        str_buffer_append_str(result, "\",");

        str_buffer_append_str(result, "\"type2string\": ");
        str_buffer_append_str(result, "\"");
        str_buffer_append_str(result, node_type_to_string(node->type));
        str_buffer_append_str(result, "\",");

        str_buffer_append_str(result, "\"error\": ");
        str_buffer_append_str(result, "\"");
        str_buffer_append_str(result, node->error ? node->error : "");
        str_buffer_append_str(result, "\",");

        str_buffer_append_str(result, "\"left\": ");
        str_buffer_append_str(result, str_buffer_to_char(left));
        str_buffer_append_str(result, ",");

        str_buffer_append_str(result, "\"right\": ");
        str_buffer_append_str(result, str_buffer_to_char(right));

        str_buffer_append_str(result, "}");

        if (type2char) {
            free(type2char);
        }

        str_buffer_free(left);
        str_buffer_free(right);
    } else {
        result = new_str_buffer("{}");
    }

    return result;
}
