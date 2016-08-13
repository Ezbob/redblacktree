#include "RBTree.h"
#include <stdlib.h>
#include "cutest.h"

void RBT_test_insert(void);
void RBT_test_remove(void);
void RBT_test_find(void);

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
	printf("\n");

	RBT_add(tree, 10);
	RBT_add(tree, 12);
	RBT_add(tree, 20);
	RBT_add(tree, 9);
	RBT_add(tree, 7);

	TEST_CHECK( tree->node_count == 5 );

	int node_key = 7;

	RBT_NODE *j = RBT_find(tree, node_key);

	TEST_CHECK( j != NULL && j->key == node_key );

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

	RBT_destroy_tree(tree);
}


TEST_LIST = {
       { "inserting into tree", RBT_test_insert },
       { "finding nodes in tree", RBT_test_find },
       { 0 }
};