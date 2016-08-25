#include "Test_RBTree.h"
#include "cutest.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#define RBT_IS_RED(node) (node != NULL && node->color == RBT_RED)
#define RBT_IS_BLACK(node) (node == NULL || node->color == RBT_BLACK)

static int default_keys[] = { 10, 12, 20, 34, 6, 3 };
static long int *default_values;

RBT_TREE *RBT_test_tree_default() {
    RBT_TREE *tree = RBT_init_tree();

    int n = sizeof(default_keys) / sizeof(default_keys[0]); 
    default_values = malloc(sizeof(long int) * n); 

    for ( int i = 0; i < n; ++i ) {
        default_values[i] = ((long int) i);

        //void *data_ptr = malloc(sizeof(long int));
        //*((long int*) data_ptr) = data;

        RBT_add(tree, default_keys[i], default_values + i );
    }

    TEST_CHECK( RBT_NODE_COUNT(tree) == 6 );

    return tree;
}

void RBT_test_tree_default_cleanup(RBT_TREE *tree) {
    RBT_destroy_tree(tree);
    free(default_values);
}

void RBT_test_is_RB_tree( RBT_TREE *tree ) {

    TEST_CHECK_( tree->root->color == RBT_BLACK, "RB properties: root is not black" );

/*
    // every leaf is NULL which is defined to be black 
    current = RBT_has_all_black_leaves( tree->root );
    is_RB = is_RB && current;

    if ( !current ) {
        fprintf(stderr, "Error: Not all leaves are black\n");
    }
*/
    TEST_CHECK_( RBT_has_even_black_height(tree->root), "RB properties: every path from root does not have equal black height" );

    TEST_CHECK_( RBT_red_has_black_children(tree->root), "RB properties: every red node does not have only black children" );
}

int RBT_has_even_black_height(RBT_NODE *node) {
    if ( node == NULL ) {
        return 1;
    }

    int left, right, this_node;

    left = RBT_has_even_black_height( node->left);
    if ( left == 0 ) {
        return left;
    }

    right = RBT_has_even_black_height( node->right );
    if ( right == 0 ) {
        return right;
    }

    this_node = RBT_IS_BLACK( node ) ? 1 : 0;
    return left == right ? left + this_node : 0;
}

int RBT_red_has_black_children(RBT_NODE *node) {

    if ( node == NULL ) {
        return 1;
    }

    int has_black_children = RBT_IS_RED(node) ? RBT_IS_BLACK(node->right) && 
        RBT_IS_BLACK(node->left) : 1;

    return has_black_children && RBT_red_has_black_children(node->left) 
        && RBT_red_has_black_children(node->left);
}


void RBT_test_insert() {
    RBT_TREE *tree = RBT_init_tree();
    printf("\n");

    RBT_add(tree, 10, NULL);
    RBT_pretty_printer(tree->root);

    RBT_test_is_RB_tree(tree);

    RBT_add(tree, 12, NULL);
    
    RBT_pretty_printer(tree->root);

    RBT_test_is_RB_tree(tree);

    RBT_add(tree, 20, NULL);

    RBT_pretty_printer(tree->root);

    RBT_test_is_RB_tree(tree);

    RBT_add(tree, 9, NULL);

    RBT_pretty_printer(tree->root);

    RBT_test_is_RB_tree(tree);

    RBT_add(tree, 7, NULL);

    RBT_pretty_printer(tree->root);

    RBT_test_is_RB_tree(tree);
    TEST_CHECK( tree->node_count == 5 );

    RBT_destroy_tree(tree);
}

void RBT_test_find() {
    RBT_TREE *tree = RBT_test_tree_default();

    RBT_test_is_RB_tree(tree);

    int node_key = 20;

    long int *j = RBT_find(tree, node_key); // right subtree

    TEST_CHECK( j != NULL && *j == 2L );

    node_key = 10;

    j = RBT_find(tree, node_key); // left subtree

    TEST_CHECK( j != NULL && *j == 0L );

    node_key = tree->root->key;

    j = RBT_find(tree, node_key); // root

    long int *rootdata = tree->root->data;

    TEST_CHECK( j != NULL && *j == *rootdata );

    node_key = 15;

    j = RBT_find(tree, node_key); // non-existing subnode

    TEST_CHECK( j == NULL );

    RBT_test_tree_default_cleanup(tree);
}

void RBT_test_min_max() {
    RBT_TREE *tree = RBT_test_tree_default();
    
    RBT_test_is_RB_tree(tree);

    long int *value;
    RBT_PAIR *pair = NULL;

    pair = RBT_get_minimum(tree);

    if ( TEST_CHECK( pair != NULL ) ) {
        TEST_CHECK( RBT_PAIR_KEY(pair) == 3 );
        value = RBT_PAIR_VALUE(pair);
        TEST_CHECK( *value == 5L );
    }
    RBT_destroy_pair(pair);

    pair = RBT_get_maximum(tree);

    if ( TEST_CHECK( pair != NULL ) ) {
        TEST_CHECK( RBT_PAIR_KEY(pair) == 34 );
        value = RBT_PAIR_VALUE(pair);
        TEST_CHECK( *value == 3L );
    }
    RBT_destroy_pair(pair);

    RBT_test_tree_default_cleanup(tree);
}

void RBT_test_min_max_null() {
    RBT_TREE *tree = RBT_init_tree();

    TEST_CHECK( RBT_get_minimum(tree) == NULL );
    TEST_CHECK( RBT_get_maximum(tree) == NULL );

    RBT_destroy_tree(tree);
}

void RBT_test_remove() {
    RBT_TREE *tree = RBT_test_tree_default();
    printf("\n");

    RBT_PRETTY_PRINT(tree);

    RBT_test_is_RB_tree(tree);
    TEST_CHECK( RBT_NODE_COUNT(tree) == 6 );

    RBT_delete( tree, 6 );

    RBT_PRETTY_PRINT(tree);

    RBT_test_is_RB_tree(tree);
    TEST_CHECK( tree->node_count == 5 );

    RBT_delete( tree, 34 );

    RBT_PRETTY_PRINT(tree);

    RBT_test_is_RB_tree(tree);

    TEST_CHECK( tree->node_count == 4 );

    RBT_test_tree_default_cleanup(tree);
}

void RBT_test_creation_deletion_stack() {
    size_t size = 20;
    RBT_STACK *stack = RBT_new_stack( size );

    TEST_CHECK( stack != NULL );

    TEST_CHECK( stack->size == size );

    TEST_CHECK( stack->next_index == 0 );

    RBT_destroy_stack(stack);
}

void RBT_test_creation_deletion_stack_fail() {
    size_t size = 0;
    RBT_STACK *stack = RBT_new_stack( size );

    TEST_CHECK( stack == NULL );

    RBT_destroy_stack(stack);
}

void RBT_test_push_stack() {
    size_t size = 10;
    RBT_STACK *stack = RBT_new_stack( size );

    TEST_CHECK( stack != NULL );

    char push_char = 'l';

    RBT_pretty_push( stack, push_char );

    TEST_CHECK( stack->next_index == 4 );

    char c;
    int i = 0;
    for ( ; ( c = stack->buffer[i] ) != '\0'; ++i ) {
        if ( i == 1  ) {
            TEST_CHECK_( c == push_char, "stack push test: character %c found %c expected at index %i ", c, push_char, i );
        } else {
            TEST_CHECK_( c == ' ', "stack push test: character %c found %c expected at index %i ", c, ' ', i );
        }
    }

    TEST_CHECK( i == 4 );

    RBT_destroy_stack(stack);
}

void RBT_test_push_stack_fail() {
    size_t size = 2;
    RBT_STACK *stack = RBT_new_stack( size );

    TEST_CHECK( stack != NULL );

    size_t old_index = stack->next_index;

    char push_char = 'l';

    RBT_pretty_push( stack, push_char );

    TEST_CHECK_( stack->next_index == old_index, "stack push test: got index: %i expected: %i", stack->next_index, old_index );

    RBT_destroy_stack(stack);
}


TEST_LIST = {
       { "inserting into tree", RBT_test_insert },
       { "finding nodes in tree", RBT_test_find },
       { "getting minimum and maximum from tree", RBT_test_min_max },
       { "getting minimum and maximum from empty tree", RBT_test_min_max_null },
       { "deleting nodes in tree", RBT_test_remove },
       { "zero-sized allocation of stack", RBT_test_creation_deletion_stack_fail },
       { "creation and deletion of print stack", RBT_test_creation_deletion_stack },
       { "legal stack push", RBT_test_push_stack },
       { "stack push overflow try", RBT_test_push_stack_fail },
       { 0 }
};