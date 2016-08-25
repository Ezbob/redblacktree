
#ifndef TEST_RBT_H
#define TEST_RBT_H

#include "RBTree.h"

typedef struct RBT_STACK {
    size_t size;
    size_t next_index;
    char *buffer; 
} RBT_STACK;

RBT_NODE *RBT_new_node( int, void *data );
void RBT_destroy_node( RBT_NODE * );
void RBT_left_rotate( RBT_TREE *, RBT_NODE * );
void RBT_right_rotate( RBT_TREE *, RBT_NODE * );
void RBT_insert_fixup( RBT_TREE *, RBT_NODE * );
RBT_NODE *RBT_insert( RBT_TREE *, RBT_NODE * );
void RBT_recursive_destroy( RBT_NODE * );
RBT_STACK *RBT_new_stack( size_t size );
void RBT_destroy_stack( RBT_STACK * stack );
void RBT_pretty_printer_helper(RBT_NODE *node, RBT_STACK * );
void RBT_pretty_push( RBT_STACK * stack, char character );
void RBT_pretty_pop( RBT_STACK * stack );
RBT_NODE *RBT_find_parent( RBT_TREE *, RBT_NODE * );
int RBT_remove( RBT_TREE *, RBT_NODE *);
void RBT_remove_fixup( RBT_TREE *, RBT_NODE *);
void RBT_transplant_tree( RBT_TREE *, RBT_NODE *, RBT_NODE * );
RBT_NODE *RBT_minimum( RBT_NODE * );
RBT_NODE *RBT_maximum( RBT_NODE * );
RBT_NODE *RBT_iterative_find( RBT_NODE *node, int key );
RBT_PAIR *RBT_new_pair( int key, void *value );

void RBT_test_insert(void);
void RBT_test_remove(void);
void RBT_test_find(void);
void RBT_test_min_max(void);
void RBT_test_min_max_null(void);
void RBT_test_creation_deletion_stack(void);
void RBT_test_push_stack(void);
void RBT_test_creation_deletion_stack_fail(void);
void RBT_test_push_stack_fail(void);

int RBT_is_RB_tree( RBT_TREE * );
int RBT_has_all_black_leaves( RBT_NODE * );
int RBT_has_even_black_height( RBT_NODE * );
int RBT_red_has_black_children( RBT_NODE * );
RBT_TREE *RBT_test_tree_default(void);
void RBT_test_tree_default_cleanup( RBT_TREE * );


#endif