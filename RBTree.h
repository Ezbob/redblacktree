#ifndef RB_TREE_
#define RB_TREE_

#define BLACK 0
#define RED 1

typedef struct RBT_NODE {
	unsigned char color;
	int key;
	struct RBT_NODE *left;
	struct RBT_NODE *right;
	struct RBT_NODE *parent;
} RBT_NODE;

typedef struct RBT_TREE {
	struct RBT_NODE *root;
} RBT_TREE;

void RBT_left_rotate(RBT_TREE *,RBT_NODE *);
void RBT_right_rotate(RBT_TREE *,RBT_NODE *);

RBT_TREE *RBT_init_tree();
void RBT_destroy_tree(RBT_TREE *);

#endif
