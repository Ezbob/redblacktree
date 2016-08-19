#include "RBTree.h"
#include <stdlib.h>
#include "cutest.h"
#include <stdbool.h>
#include <time.h>

void RBT_test_insert(void);
void RBT_test_remove(void);
void RBT_test_find(void);
void RBT_test_min_max(void);
void RBT_test_min_max_null(void);

int RBT_is_RB_tree(RBT_TREE *);
bool RBT_has_all_black_leaves(RBT_NODE *node);
int RBT_has_even_black_height(RBT_NODE *);
int RBT_red_has_black_children(RBT_NODE *);
RBT_TREE *RBT_test_tree_default(void);

static int default_keys[] = { 10, 12, 20, 34, 6, 3 };
static long int *default_data[] = { NULL, NULL, NULL, NULL, NULL, NULL };
static int seed_set = 0;

RBT_TREE *RBT_test_tree_default() {
	RBT_TREE *tree = RBT_init_tree();

	if ( !seed_set ) {
		srand(time(NULL));
		seed_set = 1;
	}

	int n = sizeof(default_keys) / sizeof(default_keys[0]); 

	for ( int i = 0; i < n; ++i ) {
		long int data = rand();
		default_data[i] = &data;
	}

	for ( int i = 0; i < 6; ++i ) {
		RBT_add(tree, default_keys[i], default_data[i]);
	}

	TEST_CHECK( RBT_NODE_COUNT(tree) == 6 );

	return tree;
}

int RBT_is_RB_tree( RBT_TREE *tree ) {

	int is_RB;
	int current = ( tree->root->color == RBT_BLACK );

	is_RB = current;
	if ( !current ) {
		fprintf(stderr,"Error: Root is not black\n");
	} 
/*
	// every leaf is NULL which is defined to be black 
	current = RBT_has_all_black_leaves( tree->root );
	is_RB = is_RB && current;

	if ( !current ) {
		fprintf(stderr, "Error: Not all leaves are black\n");
	}
*/
	current = RBT_has_even_black_height(tree->root);
	is_RB = is_RB && current;

	if ( !current ) {
		fprintf(stderr, "Error: Tree does not have even black height\n");
	}

	current = RBT_red_has_black_children(tree->root);
	is_RB = is_RB && current;

	if ( !current ) {
		fprintf(stderr, "Error: Not all red nodes has black children\n");
	}

	return is_RB;
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

	int has_black_children = RBT_IS_RED(node) ? RBT_IS_BLACK(node->right) && RBT_IS_BLACK(node->left) : 1;

	return has_black_children && RBT_red_has_black_children(node->left) 
		&& RBT_red_has_black_children(node->left);
}


void RBT_test_insert() {
	RBT_TREE *tree = RBT_init_tree();
	printf("\n");

	RBT_add(tree, 10, NULL);
	RBT_pretty_printer(tree->root);

	TEST_CHECK( RBT_is_RB_tree(tree) );

	RBT_add(tree, 12, NULL);
	
	RBT_pretty_printer(tree->root);

	TEST_CHECK( RBT_is_RB_tree(tree) );

	RBT_add(tree, 20, NULL);

	RBT_pretty_printer(tree->root);

	TEST_CHECK( RBT_is_RB_tree(tree) );

	RBT_add(tree, 9, NULL);

	RBT_pretty_printer(tree->root);

	TEST_CHECK( RBT_is_RB_tree(tree) );

	RBT_add(tree, 7, NULL);

	RBT_pretty_printer(tree->root);

	TEST_CHECK( RBT_is_RB_tree(tree) );
	TEST_CHECK( tree->node_count == 5 );

	RBT_destroy_tree(tree);
}

void RBT_test_find() {
	RBT_TREE *tree = RBT_test_tree_default();

	TEST_CHECK( RBT_is_RB_tree(tree) );

	int node_key = 20;

	RBT_NODE *j = RBT_find(tree, node_key); // right subtree

	TEST_CHECK( j != NULL && j->key == node_key );

	node_key = 10;

	j = RBT_find(tree, node_key); // left subtree

	TEST_CHECK( j != NULL && j->key == node_key );

	node_key = tree->root->key;

	j = RBT_find(tree, node_key); // root

	TEST_CHECK( j != NULL && j->key == node_key );

	node_key = 15;

	j = RBT_find(tree, node_key); // non-existing subnode

	TEST_CHECK( j == NULL );

	RBT_destroy_tree(tree);
}

void RBT_test_min_max() {
	RBT_TREE *tree = RBT_test_tree_default();
	TEST_CHECK( RBT_is_RB_tree(tree) );

	RBT_NODE *min = RBT_get_minimum(tree);
	TEST_CHECK( min != NULL );
	TEST_CHECK( min != NULL && min->key == 7 );

	RBT_NODE *max = RBT_get_maximum(tree);
	TEST_CHECK( max != NULL );
	TEST_CHECK( max != NULL && max->key == 30 );

	RBT_destroy_tree(tree);
}

void RBT_test_min_max_null() {
	RBT_TREE *tree = RBT_init_tree();

	RBT_NODE *min = RBT_get_minimum(tree);
	TEST_CHECK( min == NULL );

	RBT_NODE *max = RBT_get_minimum(tree);
	TEST_CHECK( max == NULL );

	RBT_destroy_tree(tree);
}

void RBT_test_remove() {
	RBT_TREE *tree = RBT_test_tree_default();
	printf("\n");

	RBT_PRETTY_PRINT(tree);

	TEST_CHECK( RBT_is_RB_tree(tree) );
	TEST_CHECK( tree->node_count == 5 );

	RBT_delete( tree, 9 );

	RBT_PRETTY_PRINT(tree);

	TEST_CHECK( RBT_is_RB_tree(tree) );
	TEST_CHECK( tree->node_count == 4 );

	RBT_delete( tree, 7 );

	RBT_PRETTY_PRINT(tree);

	TEST_CHECK( RBT_is_RB_tree(tree) );

	TEST_CHECK( tree->node_count == 3 );

	RBT_destroy_tree(tree);
}


TEST_LIST = {
       { "inserting into tree", RBT_test_insert },
       { "finding nodes in tree", RBT_test_find },
       { "getting minimum and maximum from tree", RBT_test_min_max },
       { "getting minimum and maximum from empty tree", RBT_test_min_max_null },
       { "deleting nodes in tree", RBT_test_remove },
       { 0 }
};