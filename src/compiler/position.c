#pragma once

#include <stdlib.h>

typedef struct {
    size_t line_start;
    size_t line_end;
    size_t col_start;
    size_t col_end;
} position;

position *new_position(size_t line_start, size_t line_end, size_t col_start, size_t col_end) {
    position *pos = malloc(sizeof(position));

    pos->line_start = line_start;
    pos->line_end = line_end;
    pos->col_start = col_start;
    pos->col_end = col_end;

    return pos;
}

position *new_empty_position() {
    return new_position(0, 0, 0, 0);
}

position *position_clone(position *pos) {
    return pos ? new_position(pos->line_start, pos->line_end, pos->col_start, pos->col_end) : NULL;
}

size_t position_line_start(position *pos) {
    return pos ? pos->line_start : 0;
}

size_t position_line_end(position *pos) {
    return pos ? pos->line_end : 0;
}

size_t position_col_start(position *pos) {
    return pos ? pos->col_start : 0;
}

size_t position_col_end(position *pos) {
    return pos ? pos->col_end : 0;
}

void position_free(position *pos) {
    if (pos) {
        free(pos);
    }
}
