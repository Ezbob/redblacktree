#include "RBTree.h"
#include <stdlib.h>

RBT_NODE *RBT_new_node() {
	RBT_NODE *new_node;
	new_node = malloc( sizeof(RBT_NODE) );
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	new_node->key = 0;
	new_node->color = BLACK;
	return new_node;
}

void RBT_destroy_node(RBT_NODE **node) {
	RBT_NODE *unpacked = *node;
	unpacked->left = NULL;
	unpacked->right = NULL;
	unpacked->parent = NULL;
	free(unpacked);
}

RBT_TREE *RBT_init_tree() {
	RBT_TREE *new_tree;
	new_tree = malloc( sizeof(RBT_TREE) );
	new_tree->root = NULL;
	return new_tree;
}

void RBT_destroy_tree(RBT_TREE *tree) {
	tree->root = NULL;
	free(tree);
}

void RBT_left_rotate(RBT_TREE *tree, RBT_NODE *node) {
	if (node->right != NULL) {
		RBT_NODE *right_node = node->right;
		node->right = right_node->left;

		if (right_node->left != NULL) {
			right_node->left->parent = node;
		}
		right_node->parent = node->parent;

		if (node->parent == NULL) {
			tree->root = right_node;
		} else if ( node == node->parent->left ) {
			node->parent->left = right_node;
		} else {
			node->parent->right = right_node;
		}
		right_node->left = node;
		node->parent = right_node;
	}
}

void RBT_right_rotate(RBT_TREE *tree, RBT_NODE *node) {
	if ( node->left != NULL ) {
		RBT_NODE *left_node = node->left;
		node->left = left_node->right;
		
		if (left_node->right != NULL) {
			left_node->right->parent = node;
		}
		left_node->parent = node->parent;
	
		if (node->parent == NULL) {
			tree->root = left_node;
		} else if ( node == node->parent->left ){
			node->parent->left = left_node;
		} else {
			node->parent->right = left_node;
		}
		left_node->right = node;
		node->parent = left_node;
	}
}
