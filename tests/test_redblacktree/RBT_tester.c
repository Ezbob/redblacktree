#include "RBTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

void RBT_test_insert(void);
int main(void);

RBT_TREE *tree = NULL;

void RBT_test_insert() {
	RBT_add(tree,10);
	RBT_add(tree,12);
	RBT_add(tree,20);
}

int main () {
	tree = RBT_init_tree();

	
	RBT_destroy_tree(tree);
	return EXIT_SUCCESS;
}
