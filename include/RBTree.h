/**
 * Red-black tree
 * This is a red-black tree based on the description from the Cormen
 * textbook. 
 *
 * Unlike Cormens version, this version uses NULL pointers to mark
 * end nodes instead of keeping a allocated nill node that is black
 * but can take on any value.
 *
 * Implemented by Anders Busch (2016)
 *
 **/
#ifndef RB_TREE_H
#define RB_TREE_H

#include <stdlib.h>

enum RBT_COLOR { 
	RBT_BLACK, RBT_RED 
};

typedef struct RBT_NODE {
	enum RBT_COLOR color;
	size_t key;
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
	size_t key;
	void *value;	
} RBT_PAIR;


RBT_TREE *RBT_init_tree(void);

void RBT_destroy_tree(RBT_TREE *, void (*freedata)(void *));

void RBT_destroy_pair(RBT_PAIR *);

void *RBT_add(RBT_TREE *, size_t, void *);

int RBT_delete(RBT_TREE *, size_t);

void *RBT_find(RBT_TREE *, size_t);

RBT_PAIR *RBT_get_maximum(RBT_TREE *);

RBT_PAIR *RBT_get_minimum(RBT_TREE *);

void RBT_pretty_printer(RBT_NODE *);

#define RBT_PRETTY_PRINT(tree) RBT_pretty_printer(tree->root)
#define RBT_NODE_COUNT(tree) ( tree->node_count )
#define RBT_PAIR_KEY(pair) ( pair->key )
#define RBT_PAIR_VALUE(pair) ( pair->value )

#endif
