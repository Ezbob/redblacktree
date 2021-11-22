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

struct RBT_NODE {
	enum RBT_COLOR color;
	size_t key;
	void *data;
	struct RBT_NODE *left;
	struct RBT_NODE *right;
	struct RBT_NODE *parent;
};

struct RBT_TREE {
	struct RBT_NODE *root;
	size_t node_count;
};

struct RBT_PAIR {
	size_t key;
	void *value;
};

struct RBT_TREE *RBT_init_tree(void);

void RBT_destroy_tree(struct RBT_TREE *, void (*)(void *));

void *RBT_add(struct RBT_TREE *, size_t, void *);

int RBT_delete(struct RBT_TREE *, size_t);

void *RBT_find(struct RBT_TREE *, size_t);

int RBT_get_maximum(struct RBT_TREE *tree, size_t *key, void **value);

int RBT_get_minimum(struct RBT_TREE *tree, size_t *key, void **value);

void RBT_pretty_printer(struct RBT_NODE *);

#define RBT_PRETTY_PRINT(tree) RBT_pretty_printer(tree->root)
#define RBT_NODE_COUNT(tree) ( tree->node_count )

#endif
