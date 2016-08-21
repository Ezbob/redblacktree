#ifndef RB_TREE_
#define RB_TREE_

#include <stdlib.h>

enum RBT_COLOR { 
	RBT_BLACK, RBT_RED 
};

typedef struct RBT_NODE {
	enum RBT_COLOR color;
	int key;
	void *data;
	struct RBT_NODE *left;
	struct RBT_NODE *right;
	struct RBT_NODE *parent;
} RBT_NODE;

typedef struct RBT_TREE {
	struct RBT_NODE *root;
	size_t node_count;
} RBT_TREE;

typedef struct RBT_PAIR {
	int key;
	void *value;	
} RBT_PAIR;

RBT_TREE *RBT_init_tree(void);

void RBT_destroy_tree(RBT_TREE *);

void RBT_destroy_pair(RBT_PAIR *);

void *RBT_add(RBT_TREE *, int, void *);

int RBT_delete(RBT_TREE *, int);

void *RBT_find(RBT_TREE *, int);

RBT_PAIR *RBT_get_maximum(RBT_TREE *);

RBT_PAIR *RBT_get_minimum(RBT_TREE *);

void RBT_pretty_printer(RBT_NODE *);

#define RBT_PRETTY_PRINT(tree) RBT_pretty_printer(tree->root)
#define RBT_NODE_COUNT(tree) ( tree->node_count )
#define RBT_PAIR_KEY(pair) ( pair->key )
#define RBT_PAIR_VALUE(pair) ( pair->value )

#endif
