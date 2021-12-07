#ifndef STACK_TEST_H
#define STACK_TEST_H

#include "RBTree/RBTree.h"

// private stuff within RBTree.h

struct RBT_Stack
{
    size_t size;
    size_t next_index;
    char *buffer;
};

struct RBT_Node *RBT_new_node(int, void *data);
void RBT_destroy_node(struct RBT_Node *, void (*f)(void *));
void RBT_left_rotate(struct RBT_Tree *, struct RBT_Node *);
void RBT_right_rotate(struct RBT_Tree *, struct RBT_Node *);
void RBT_insert_fixup(struct RBT_Tree *, struct RBT_Node *);
struct RBT_Node *RBT_insert(struct RBT_Tree *, struct RBT_Node *);
void RBT_recursive_destroy(struct RBT_Node *, void (*f)(void *));
struct RBT_Stack *RBT_new_stack(size_t size);
void RBT_destroy_stack(struct RBT_Stack *stack);
void RBT_pretty_printer_helper(struct RBT_Node *node, struct RBT_Stack *);
void RBT_pretty_push(struct RBT_Stack *stack, char character);
void RBT_pretty_pop(struct RBT_Stack *stack);
struct RBT_Node *RBT_find_parent(struct RBT_Tree *, struct RBT_Node *);
int RBT_remove(struct RBT_Tree *, struct RBT_Node *);
void RBT_remove_fixup(struct RBT_Tree *, struct RBT_Node *);
void RBT_transplant_tree(struct RBT_Tree *, struct RBT_Node *, struct RBT_Node *);
struct RBT_Node *RBT_minimum(struct RBT_Node *);
struct RBT_Node *RBT_maximum(struct RBT_Node *);
struct RBT_Node *RBT_iterative_find(struct RBT_Node *node, size_t key);
struct RBT_PAIR *RBT_new_pair(size_t key, void *value);

// end of private stuff

// test functions

void RBT_test_is_RB_tree(struct RBT_Tree *);
int RBT_has_all_black_leaves(struct RBT_Node *);
int RBT_has_even_black_height(struct RBT_Node *);
int RBT_red_has_black_children(struct RBT_Node *);
struct RBT_Tree *RBT_test_tree_default(void);
void RBT_test_tree_default_cleanup(struct RBT_Tree *);

void RBT_test_insert(void);
void RBT_test_remove(void);
void RBT_test_find(void);
void RBT_test_min_max(void);
void RBT_test_min_max_null(void);

void RBT_test_creation_deletion_stack_fail(void);
void RBT_test_push_stack_fail(void);
void RBT_test_static_allocate_nodes(void);

// end of functions

#define RBT_IS_RED(node) (node && ((node->key & (UINTMAX_C(1) << (8 * sizeof(uintmax_t) - 1))) != 0))
#define RBT_IS_BLACK(node)  (! RBT_IS_RED(node))

#endif
