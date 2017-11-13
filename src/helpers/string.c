#pragma once

#include <stdlib.h>
#include <string.h>
#include <lib/utf8.h>

#define STRING_MEMORY_SIZE 8

typedef char string;

void str_free(string *str) {
    if (str) {
        free(str);
    }
}

size_t str_len(string *src) {
    size_t length = 0;
    char *next = src;
    uint32_t current_char[4];
    int error = 0;

    while (*next && error == 0) {
        next = utf8_decode(next, current_char, &error);
        length++;
    }

    return length;
}

size_t str_mem_size(string *str) {
    return str ? (size_t) *(str - sizeof(size_t)) : 0;
}

string *str_set_mem_size(string *str, size_t new_mem) {
    string *result = str;

    if (str) {
        result = realloc(result - sizeof(size_t), new_mem);

        if (result) {
            *result = new_mem;
            result += sizeof(size_t);
        }
    }

    return result;
}

string *str_realloc_if_required(string *str, size_t required_mem) {
    string *result = str;
    size_t str_mem = str_mem_size(result);
    size_t str_new_mem = str_mem;

    if (result && str_mem < required_mem) {
        while (str_new_mem < required_mem) {
            str_new_mem *= 2;
        }

        result = str_set_mem_size(result, str_new_mem);
    }

    return result;
}

string *str_n_concat(string *dest, string *src, size_t chars_to_concat) {
    string *result = dest;
    string *next = src;
    uint32_t current_char[4];
    int error = 0;
    size_t required_mem;
    size_t src_len;

    if (dest && src && chars_to_concat > 0) {
        src_len = strlen(src);
        chars_to_concat = src_len < chars_to_concat ? src_len : chars_to_concat;

        while (*next && chars_to_concat > 0) {
            next = utf8_decode(next, current_char, &error);
            chars_to_concat -= 1;
        }

        required_mem = (sizeof(size_t) + strlen(dest) + next - src + 1) * sizeof(string);
        result = str_realloc_if_required(result, required_mem);

        strncat(result, src, next - src);
    }

    return result;
}

string *str_concat(string *dest, string *src) {
    return str_n_concat(dest, src, src ? str_len(src) : 0);
}

string *new_string(char *content) {
    string *str = calloc(sizeof(size_t) + STRING_MEMORY_SIZE, sizeof(string));

    *str = STRING_MEMORY_SIZE;

    return str_concat(sizeof(size_t) + str, content);
}
