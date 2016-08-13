#ifndef RB_TREE_
#define RB_TREE_

#include <stdlib.h>

typedef enum RBT_color { 
	BLACK, RED 
} RBT_color;

typedef struct RBT_NODE {
	RBT_color color;
	int key;
	struct RBT_NODE *left;
	struct RBT_NODE *right;
	struct RBT_NODE *parent;
} RBT_NODE;

typedef struct RBT_TREE {
	struct RBT_NODE *root;
	size_t node_count;
} RBT_TREE;

RBT_TREE *RBT_init_tree(void);

void RBT_destroy_tree(RBT_TREE *);

RBT_NODE *RBT_add(RBT_TREE *, int);

void RBT_delete(RBT_TREE *, int);

RBT_NODE *RBT_find(RBT_TREE *, int);

RBT_NODE *RBT_get_maximum(RBT_TREE *);

RBT_NODE *RBT_get_minimum(RBT_TREE *);

void RBT_pretty_printer(RBT_NODE *);

#define RBT_PRETTY_PRINT(tree) RBT_pretty_printer(tree->root)

#endif
