#include "RBTree.h"
#include <stdlib.h>
#include "cutest.h"

void RBT_test_insert(void);
void RBT_test_remove(void);

void RBT_test_insert() {
	RBT_TREE *tree = RBT_init_tree();

	RBT_add(tree, 10);
	RBT_add(tree, 12);
	RBT_add(tree, 20);

	TEST_CHECK( tree->node_count == 3 );

	RBT_destroy_tree(tree);
}

void RBT_test_remove() {
	RBT_TREE *tree = RBT_init_tree();

	RBT_add(tree, 10);
	RBT_add(tree, 12);
	RBT_add(tree, 20);

	TEST_CHECK( tree->node_count == 3 );

	RBT_destroy_tree(tree);
}


TEST_LIST = {
       { "inserting into tree", RBT_test_insert },
       { 0 }
};