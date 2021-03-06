#include <stdlib.h>
#include <stdio.h>
#include "test/pub_cutest.h"
#include "test/test_functions.h"

#define RBT_IS_RED(node) (node != NULL && node->color == RBT_RED)
#define RBT_IS_BLACK(node) (node == NULL || node->color == RBT_BLACK)

static int default_keys[] = { 10, 12, 20, 34, 6, 3 };
static long int *default_values;

void nofree(void *);
void nofree(void *item) { ((void) item); }

struct RBT_TREE *RBT_test_tree_default() {
    struct RBT_TREE *tree = RBT_init_tree();

    int n = sizeof(default_keys) / sizeof(default_keys[0]); 
    default_values = malloc(sizeof(long int) * n); 

    for ( int i = 0; i < n; ++i ) {
        default_values[i] = ((long int) i);
        
        RBT_add(tree, default_keys[i], default_values + i );
    }

    TEST_CHECK( RBT_NODE_COUNT(tree) == 6 );

    return tree;
}

void RBT_test_tree_default_cleanup(struct RBT_TREE *tree) {
    RBT_destroy_tree(tree, nofree);
    free(default_values);
}

void RBT_test_is_RB_tree(struct RBT_TREE *tree) {

    TEST_CHECK_( tree->root->color == RBT_BLACK, "RB properties: root is not black" );

    TEST_CHECK_( RBT_has_even_black_height(tree->root), "RB properties: every path from root does not have equal black height" );

    TEST_CHECK_( RBT_red_has_black_children(tree->root), "RB properties: every red node does not have only black children" );
}

int RBT_has_even_black_height(struct RBT_NODE *node) {
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

int RBT_red_has_black_children(struct RBT_NODE *node) {

    if ( node == NULL ) {
        return 1;
    }

    int has_black_children = RBT_IS_RED(node) ? RBT_IS_BLACK(node->right) && 
        RBT_IS_BLACK(node->left) : 1;

    return has_black_children && RBT_red_has_black_children(node->left) 
        && RBT_red_has_black_children(node->left);
}


void RBT_test_insert() {
    struct RBT_TREE *tree = RBT_init_tree();
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

    RBT_destroy_tree(tree, nofree);
}

void RBT_test_find() {
    struct RBT_TREE *tree = RBT_test_tree_default();

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
    struct RBT_TREE *tree = RBT_test_tree_default();
    
    RBT_test_is_RB_tree(tree);

    long int *value;
    struct RBT_PAIR *pair = NULL;

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
    struct RBT_TREE *tree = RBT_init_tree();

    TEST_CHECK( RBT_get_minimum(tree) == NULL );
    TEST_CHECK( RBT_get_maximum(tree) == NULL );

    RBT_destroy_tree(tree, nofree);
}

void RBT_test_remove() {
    struct RBT_TREE *tree = RBT_test_tree_default();
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

