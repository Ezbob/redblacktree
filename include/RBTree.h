#ifndef RB_TREE_
#define RB_TREE_

typedef struct RBT_NODE {
	enum { BLACK, RED } color;
	int key;
	struct RBT_NODE *left;
	struct RBT_NODE *right;
	struct RBT_NODE *parent;
} RBT_NODE;

typedef struct RBT_TREE {
	struct RBT_NODE *root;
	int node_count;
} RBT_TREE;

RBT_TREE *RBT_init_tree(void);

void RBT_destroy_tree(RBT_TREE *);

RBT_NODE *RBT_add(RBT_TREE *, int);

void RBT_remove(RBT_TREE *, RBT_NODE *);

RBT_NODE *RBT_find(RBT_TREE *, int);

RBT_NODE *RBT_get_maximum(RBT_TREE *);

RBT_NODE *RBT_get_minimum(RBT_TREE *);

#endif
