#pragma once

#include <src/compiler/config.c>

char *error_variable_type(config *config) {
    return "Expecting variable type";
}

char *error_variable_name(config *config) {
    return "Expecting variable name";
}

char *error_local_variable(config *config) {
    return "Expecting 'local' keyword";
}
