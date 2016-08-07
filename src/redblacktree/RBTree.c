#include "RBTree.h"
#include <stdlib.h>

RBT_NODE *RBT_new_node(int key) {
	RBT_NODE *new_node;
	new_node = malloc( sizeof(RBT_NODE) );
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	new_node->key = key;
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
	new_tree->node_count = 0;
	return new_tree;
}

void RBT_recursive_destroy(RBT_NODE **node) {
	if ( *node != NULL ) {
		RBT_NODE *n = *node;
		if (n->left != NULL) {
			RBT_recursive_destroy(&n->left);		
		}
		if (n->right != NULL) {	
			RBT_recursive_destroy(&n->right);
		}
		free(n);
	}
}

void RBT_destroy_tree(RBT_TREE *tree) {
	RBT_recursive_destroy(&tree->root);
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

void RBT_insert_fixup(RBT_TREE *tree, RBT_NODE *node) {
	while ( node->parent->color == RED ) {
		if ( node->parent == node->parent->parent->left ) {
			RBT_NODE *right_node = node->parent->parent->right;
			if ( right_node->color == RED ) {
				// uncle and parent of node is red -> color them black
				node->parent->color = BLACK;
				right_node->color = BLACK;
				node->parent->parent->color = RED;
				node = node->parent->parent;
				continue;
			} else if ( node == node->parent->right ) {
				// uncle is black
				node = node->parent;
				RBT_left_rotate(tree, node);
			}

			node->parent->color = BLACK;
			node->parent->parent->color = RED;
			RBT_right_rotate(tree, node->parent->parent);
		} else {
			RBT_NODE *left_node = node->parent->parent->left;
			if ( left_node->color == RED ) {
                                node->parent->color = BLACK;
                                left_node->color = BLACK;
                                node->parent->parent->color = RED;
                                node = node->parent->parent;
				continue;
                        } else if ( node == node->parent->left ) {
                                node = node->parent;
                                RBT_left_rotate(tree, node);
                        }
			node->parent->color = BLACK;
                        node->parent->parent->color = RED;
                        RBT_right_rotate(tree, node->parent->parent);
		}
	}
	tree->root->color = BLACK;
}

RBT_NODE *RBT_insert(RBT_TREE *tree, RBT_NODE *node) {
	RBT_NODE *parent = NULL;
	RBT_NODE *iterator = tree->root;
	
	while ( iterator != NULL ) { // traversal of the tree finding parent of node
		parent = iterator;
		if ( node->key < iterator->key ) {
			iterator = iterator->left;
		} else {
			iterator = iterator->right;
		}
	}

	node->parent = parent; // setting parent node and fixing forward pointers
	if ( iterator == NULL ) {
		tree->root = node;
	} else if ( node->key < iterator->key  ) {
		iterator->left = node;
	} else {
		iterator->right = node;
	}
	
	node->left = NULL;
	node->right = NULL;
	node->color = RED;
	RBT_insert_fixup(tree, node);
	return node;
}

RBT_NODE *RBT_add(RBT_TREE *tree, int key) {
	tree->node_count += 1;
	return RBT_insert(tree, RBT_new_node(key));
}
