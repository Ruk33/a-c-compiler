#pragma once

#include <stdlib.h>

typedef struct {
    int flags; // todo
} config;

config *new_config() {
    return malloc(sizeof(config));
}
