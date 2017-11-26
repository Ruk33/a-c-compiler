#pragma once

/*
 * Simple double linked list implementation
 *
 * Example:
 *
 * struct list *my_list = list_new();
 * struct list *node;
 *
 * list_add_value(my_list, (list_value_t) 1);
 * list_add_value(my_list, (list_value_t) 2);
 *
 * node = list_head(my_list);
 *
 * while (list_value(node)) {
 *     // do things with value
 *     node = list_next(node);
 * }
 *
 * list_remove_value(my_list, (list_value_t) 1, list_value_int_comparer); // now my_list only contains 2
 */

typedef void *list_value_t;

struct list *list_new();

struct list *list_next(struct list *node);

struct list *list_prev(struct list *node);

// Get first element of the list
struct list *list_head(struct list *node);

// Get last element of the list
struct list *list_tail(struct list *node);

// Get the value of the node
list_value_t list_value(struct list *node);

// Add a new value to the end of the list and return the new node
// NULL values wont be included
struct list *list_add_value(struct list *list, list_value_t value);

// Function to compare two list values
typedef int(*list_value_comparer)(list_value_t a, list_value_t b);

int list_value_int_comparer(list_value_t a, list_value_t b);

// Remove value from list and return the node containing the value
struct list *list_remove_value(struct list *list, list_value_t value, list_value_comparer comparer);

size_t list_size(struct list *list);

int list_is_empty(struct list *list);

int list_has_next(struct list *node);

int list_has_prev(struct list *node);

struct list *list_clone(struct list *list);
