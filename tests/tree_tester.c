#include <stdlib.h>
#include <stdio.h>
#include "pub_cutest.h"
#include "test_functions.h"

#define RBT_IS_RED(node) (node && ((node->key & (UINTMAX_C(1) << (8 * sizeof(uintmax_t) - 1))) != 0))
#define RBT_IS_BLACK(node)  (! RBT_IS_RED(node))

static int default_keys[] = { 10, 12, 20, 34, 6, 3 };
static long int *default_values;

void nofree(void *);
void nofree(void *item) { ((void) item); }

struct RBT_Tree *RBT_test_tree_default() {
    static struct RBT_Tree tree;
    RBT_init_tree(&tree);

    int n = sizeof(default_keys) / sizeof(default_keys[0]);
    default_values = malloc(sizeof(long int) * n);

    for ( int i = 0; i < n; ++i ) {
        default_values[i] = ((long int) i);

        RBT_add(&tree, default_keys[i], default_values + i );
    }

    TEST_CHECK( RBT_NODE_COUNT(&tree) == 6 );

    return &tree;
}

void RBT_test_tree_default_cleanup(struct RBT_Tree *tree) {
    RBT_deinit_tree(tree, nofree);
    free(default_values);
}

void RBT_test_is_RB_tree(struct RBT_Tree *tree) {

    TEST_CHECK_( RBT_IS_BLACK(tree->root), "RB properties: root is not black" );

    TEST_CHECK_( RBT_has_even_black_height(tree->root), "RB properties: every path from root does not have equal black height" );

    TEST_CHECK_( RBT_red_has_black_children(tree->root), "RB properties: every red node does not have only black children" );
}

int RBT_has_even_black_height(struct RBT_Node *node) {
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

int RBT_red_has_black_children(struct RBT_Node *node) {
    if ( node == NULL ) {
        return 1;
    }

    int has_black_children = RBT_IS_RED(node) ? RBT_IS_BLACK(node->right) &&
        RBT_IS_BLACK(node->left) : 1;

    return has_black_children && RBT_red_has_black_children(node->left)
        && RBT_red_has_black_children(node->left);
}


void RBT_test_insert() {
    struct RBT_Tree tree;
    RBT_init_tree(&tree);

    RBT_add(&tree, 10, NULL);

    RBT_pretty_printer(tree.root);

    RBT_test_is_RB_tree(&tree);

    RBT_add(&tree, 12, NULL);

    RBT_pretty_printer(tree.root);

    RBT_test_is_RB_tree(&tree);

    RBT_add(&tree, 20, NULL);

    RBT_pretty_printer(tree.root);

    RBT_test_is_RB_tree(&tree);

    RBT_add(&tree, 9, NULL);

    RBT_pretty_printer(tree.root);

    RBT_test_is_RB_tree(&tree);

    RBT_add(&tree, 7, NULL);

    RBT_pretty_printer(tree.root);

    RBT_test_is_RB_tree(&tree);
    TEST_CHECK( tree.node_count == 5 );

    RBT_deinit_tree(&tree, nofree);
}

void RBT_test_find() {
    struct RBT_Tree *tree = RBT_test_tree_default();

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
    struct RBT_Tree *tree = RBT_test_tree_default();

    RBT_test_is_RB_tree(tree);

    RBT_pretty_printer(tree->root);

    long int *value;
    uintmax_t key;

    if ( TEST_CHECK( RBT_get_minimum(tree, &key, &value) ) ) {
        TEST_CHECK( key == UINTMAX_C(3) );
        TEST_CHECK( *value == 5L );
    }

    if ( TEST_CHECK( RBT_get_maximum(tree, &key, &value) ) ) {
        printf("--> %llu, %li\n", key, *value);
        TEST_CHECK( key == UINTMAX_C(34) );
        TEST_CHECK( *value == 3L );
    }

    RBT_test_tree_default_cleanup(tree);
}

void RBT_test_min_max_null() {
    struct RBT_Tree tree;
    RBT_init_tree(&tree);

    TEST_CHECK( !RBT_get_minimum(&tree, NULL, NULL) );
    TEST_CHECK( !RBT_get_maximum(&tree, NULL, NULL) );

    RBT_deinit_tree(&tree, nofree);
}

void RBT_test_remove() {
    struct RBT_Tree *tree = RBT_test_tree_default();
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

static struct RBT_Node nodes[8];
static size_t i = 0;

struct RBT_Node *my_malloc(size_t a) {
    (void)(a);
    if (i < 6) {
        return &nodes[i];
    } else {
        return NULL;
    }
}

void my_free(struct RBT_Node *node) {
    (void)(node);
}

void RBT_test_static_allocate_nodes() {
    struct RBT_Tree tree;
    struct RBT_Node nodes[6];
    #undef RBT_MALLOC
    #undef RBT_FREE
    #define RBT_MALLOC my_malloc
    #define RBT_FREE my_free

    RBT_init_tree(&tree);

    TEST_CHECK( RBT_NODE_COUNT(&tree) == 0 );

    int a = 23;
    int b = 13;
    RBT_add(&tree, 10, &b);
    RBT_add(&tree, 11, &a);

    int *j = RBT_find(&tree, 11);
    int *bj = RBT_find(&tree, 10);

    TEST_CHECK( j != NULL );
    TEST_CHECK( bj != NULL );

    TEST_CHECK( a == *j );
    TEST_CHECK( b == *bj );

    TEST_CHECK( RBT_NODE_COUNT(&tree) == 2 );

    RBT_deinit_tree(&tree, nofree);
}