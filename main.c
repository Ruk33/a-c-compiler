#include <stdio.h>
#include <stdlib.h>

#include <lib/hashmap.h>
#include <src/compiler/tokenizer.c>

int main() {
    map_t *h = token_map();
    token_type v;

    hashmap_get(h, "extends", (any_t) &v);

    printf("%d, %s\n", v, token_type_to_string(v));

    return 0;
}
