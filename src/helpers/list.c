#include <stdlib.h>
#include <string.h>

#include <src/helpers/list.h>

struct list {
    list_value_t value;

    struct list *head;
    size_t size;

    struct list *prev;
    struct list *next;
};

struct list *list_new()
{
    struct list *list = malloc(sizeof(struct list));

    list->value = NULL;

    list->head = list;
    list->size = 0;

    list->prev = list;
    list->next = list;

    return list;
}

struct list *list_prev(struct list *node)
{
    return node ? node->prev : NULL;
}

struct list *list_next(struct list *node)
{
    return node ? node->next : NULL;
}

struct list *list_head(struct list *node)
{
    return node ? list_next(node->head) : NULL;
}

struct list *list_tail(struct list *node)
{
    return node ? list_prev(node->head) : NULL;
}

list_value_t list_value(struct list *node)
{
    return node ? node->value : NULL;
}

int list_has_next(struct list *node)
{
    return list_value(list_next(node)) != NULL;
}

int list_has_prev(struct list *node)
{
    return list_value(list_prev(node)) != NULL;
}

size_t list_size(struct list *list)
{
    return list && list->head ? list->head->size : 0;
}

int list_is_empty(struct list *list)
{
    return list_size(list) == 0;
}

struct list *list_add_value(struct list *list, list_value_t value)
{
    struct list *head;
    struct list *tail;
    struct list *new_node = NULL;

    if (list && value) {
        head = list->head ? list->head : list;
        tail = list_tail(list);
        tail = tail ? tail : list;

        new_node = malloc(sizeof(struct list));

        new_node->value = value;
        new_node->head = head;
        new_node->prev = tail;
        new_node->next = head;

        tail->next = new_node;
        head->prev = new_node;

        head->size += 1;
    }

    return new_node;
}

int list_value_int_comparer(list_value_t a, list_value_t b)
{
    return (int *)a == (int *)b;
}

struct list *list_remove_value(struct list *list, list_value_t value, list_value_comparer compare)
{
    struct list *head;
    struct list *node;
    list_value_t node_value;

    struct list *removed_node = NULL;

    if (value && compare) {
        node = list_head(list);

        while (node_value = list_value(node)) {
            if (compare(node_value, value)) {
                removed_node = node;
                break;
            }

            node = list_next(node);
        }

        if (removed_node) {
            if (removed_node->prev) {
                removed_node->prev->next = removed_node->next;
            }

            if (removed_node->next) {
                removed_node->next->prev = removed_node->prev;
            }

            head = list->head ? list->head : list;
            head->size -= 1;
        }
    }

    return removed_node;
}

struct list *list_clone(struct list *list)
{
    struct list *clone = list_new();
    struct list *node = list_head(list);
    list_value_t node_value;

    while (node_value = list_value(node)) {
        list_add_value(clone, node_value);
        node = list_next(node);
    }

    return clone;
}
