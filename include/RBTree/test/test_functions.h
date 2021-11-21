#ifndef STACK_TEST_H
#define STACK_TEST_H

#include "RBTree/RBTree.h"

// private stuff within RBTree.h

struct RBT_STACK {
    size_t size;
    size_t next_index;
    char *buffer; 
};

struct RBT_NODE *RBT_new_node( int, void *data );
void RBT_destroy_node(struct RBT_NODE *, void (*f)(void *));
void RBT_left_rotate(struct RBT_TREE *, struct RBT_NODE *);
void RBT_right_rotate(struct RBT_TREE *, struct RBT_NODE *);
void RBT_insert_fixup(struct RBT_TREE *, struct RBT_NODE *);
struct RBT_NODE *RBT_insert(struct RBT_TREE *, struct RBT_NODE *);
void RBT_recursive_destroy(struct RBT_NODE *, void (*f)(void *));
struct RBT_STACK *RBT_new_stack( size_t size );
void RBT_destroy_stack(struct RBT_STACK * stack);
void RBT_pretty_printer_helper(struct RBT_NODE *node, struct RBT_STACK *);
void RBT_pretty_push(struct RBT_STACK * stack, char character);
void RBT_pretty_pop(struct RBT_STACK * stack );
struct RBT_NODE *RBT_find_parent(struct RBT_TREE *, struct RBT_NODE *);
int RBT_remove(struct RBT_TREE *, struct RBT_NODE *);
void RBT_remove_fixup(struct RBT_TREE *, struct RBT_NODE *);
void RBT_transplant_tree(struct RBT_TREE *, struct RBT_NODE *, struct RBT_NODE *);
struct RBT_NODE *RBT_minimum(struct RBT_NODE *);
struct RBT_NODE *RBT_maximum(struct RBT_NODE *);
struct RBT_NODE *RBT_iterative_find(struct RBT_NODE *node, size_t key );
struct RBT_PAIR *RBT_new_pair(size_t key, void *value);

// end of private stuff

// test functions

void RBT_test_is_RB_tree( struct RBT_TREE * );
int RBT_has_all_black_leaves( struct RBT_NODE * );
int RBT_has_even_black_height( struct RBT_NODE * );
int RBT_red_has_black_children( struct RBT_NODE * );
struct RBT_TREE *RBT_test_tree_default( void );
void RBT_test_tree_default_cleanup( struct RBT_TREE * );

void RBT_test_insert( void );
void RBT_test_remove( void );
void RBT_test_find( void );
void RBT_test_min_max( void );
void RBT_test_min_max_null( void );

void RBT_test_creation_deletion_stack( void );
void RBT_test_push_stack( void );
void RBT_test_creation_deletion_stack_fail( void );
void RBT_test_push_stack_fail( void );

// end of functions

#endif
