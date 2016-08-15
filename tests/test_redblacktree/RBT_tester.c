#include "RBTree.h"
#include <stdlib.h>
#include "cutest.h"

void RBT_test_insert(void);
void RBT_test_remove(void);
void RBT_test_find(void);
void RBT_test_min_max(void);
void RBT_test_min_max_null(void);

void RBT_test_insert() {
	RBT_TREE *tree = RBT_init_tree();
	printf("\n");

	RBT_add(tree, 10);
	RBT_pretty_printer(tree->root);

	RBT_add(tree, 12);
	
	RBT_pretty_printer(tree->root);

	RBT_add(tree, 20);

	RBT_pretty_printer(tree->root);

	RBT_add(tree, 9);

	RBT_pretty_printer(tree->root);

	RBT_add(tree, 7);

	RBT_pretty_printer(tree->root);

	TEST_CHECK( tree->node_count == 5 );

	RBT_destroy_tree(tree);
}

void RBT_test_find() {
	RBT_TREE *tree = RBT_init_tree();

	RBT_add(tree, 10);
	RBT_add(tree, 12);
	RBT_add(tree, 20);
	RBT_add(tree, 9);
	RBT_add(tree, 7);

	TEST_CHECK( tree->node_count == 5 );

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
	RBT_TREE *tree = RBT_init_tree();

	RBT_add(tree, 10);
	RBT_add(tree, 12);
	RBT_add(tree, 20);
	RBT_add(tree, 9);
	RBT_add(tree, 7);
	RBT_add(tree, 30);

	RBT_NODE *min = RBT_get_minimum(tree);
	TEST_CHECK( min != NULL );
	TEST_CHECK( min != NULL && min->key == 7 );

	RBT_NODE *max = RBT_get_minimum(tree);
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
	RBT_TREE *tree = RBT_init_tree();

	RBT_add(tree, 10);
	RBT_add(tree, 12);
	RBT_add(tree, 20);
	RBT_add(tree, 9);
	RBT_add(tree, 7);

	TEST_CHECK( tree->node_count == 5 );

	RBT_delete( tree, 9 );

	TEST_CHECK( tree->node_count == 4 );

	RBT_delete( tree, 7 );

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