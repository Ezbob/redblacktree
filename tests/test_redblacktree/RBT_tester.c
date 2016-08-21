#include "RBTree.h"
#include <stdlib.h>
#include "cutest.h"
#include <stdbool.h>
#include <time.h>

#define RBT_IS_RED(node) (node != NULL && node->color == RBT_RED)
#define RBT_IS_BLACK(node) (node == NULL || node->color == RBT_BLACK)

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
void RBT_test_tree_default_cleanup(RBT_TREE *);

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
	TEST_CHECK( RBT_is_RB_tree(tree) );
	int is_null = 1;
	long int *value;
	RBT_PAIR *pair = NULL;

	pair = RBT_get_minimum(tree);
	is_null = (pair == NULL);

	TEST_CHECK( !is_null );
	if ( !is_null ) {
		TEST_CHECK( RBT_PAIR_KEY(pair) == 3 );
		value = RBT_PAIR_VALUE(pair);
		TEST_CHECK( *value == 5L );
	}
	RBT_destroy_pair(pair);

	pair = RBT_get_maximum(tree);
	is_null = (pair == NULL);

	TEST_CHECK( !is_null );
	if ( !is_null ) {
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

	TEST_CHECK( RBT_is_RB_tree(tree) );
	TEST_CHECK( RBT_NODE_COUNT(tree) == 6 );

	RBT_delete( tree, 6 );

	RBT_PRETTY_PRINT(tree);

	TEST_CHECK( RBT_is_RB_tree(tree) );
	TEST_CHECK( tree->node_count == 5 );

	RBT_delete( tree, 34 );

	RBT_PRETTY_PRINT(tree);

	TEST_CHECK( RBT_is_RB_tree(tree) );

	TEST_CHECK( tree->node_count == 4 );

	RBT_test_tree_default_cleanup(tree);
}


TEST_LIST = {
       { "inserting into tree", RBT_test_insert },
       { "finding nodes in tree", RBT_test_find },
       { "getting minimum and maximum from tree", RBT_test_min_max },
       { "getting minimum and maximum from empty tree", RBT_test_min_max_null },
       { "deleting nodes in tree", RBT_test_remove },
       { 0 }
};