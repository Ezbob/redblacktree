#include "RBTree.h"
#include <stdlib.h>
#include <stdio.h>

#define RBT_ERROR(message) fprintf(stderr, "Error: %s\n", message)
#define RBT_ERROR_STOP(code, message) fprintf(stderr, "Error: %s\n", message); exit(code)

// NULL values are black per difinition, so node has  
// to be non-NULL for it to be RED
#define RBT_IS_RED(node) node != NULL && node->color == RED
#define RBT_IS_BLACK(node) node == NULL || node->color == BLACK
#define RBT_COLOR_CHAR(node) RBT_IS_RED(node) ? 'r' : 'b' 
#define RBT_TAB_SIZE 4

RBT_NODE *RBT_new_node(int key);
RBT_TREE *RBT_init_tree(void);
void RBT_destroy_node(RBT_NODE *node);
static void RBT_left_rotate(RBT_TREE *tree, RBT_NODE *node);
static void RBT_right_rotate(RBT_TREE *tree, RBT_NODE *node);
static void RBT_insert_fixup(RBT_TREE *tree, RBT_NODE *node);
static RBT_NODE *RBT_insert(RBT_TREE *tree, RBT_NODE *node);
static void RBT_recursive_destroy(RBT_NODE *node);
static void pretty_printer_helper(RBT_NODE *node, int indent);
static RBT_NODE *RBT_find_parent(RBT_TREE *tree, RBT_NODE *node);
static void RBT_remove(RBT_TREE *, RBT_NODE *);


RBT_NODE *RBT_new_node(int key) {
	RBT_NODE *new_node;
	new_node = malloc( sizeof(RBT_NODE) );
	if ( new_node == NULL ) {
		RBT_ERROR_STOP(EXIT_FAILURE, "Cannot allocate more memory.");
		return NULL;
	}
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	new_node->key = key;
	new_node->color = BLACK;
	return new_node;
}

void RBT_destroy_node(RBT_NODE *node) {
	if ( node != NULL ) {
		node->left = NULL;
		node->right = NULL;
		node->parent = NULL;
		free(node);	
	}
}

RBT_TREE *RBT_init_tree() {
	RBT_TREE *new_tree;
	new_tree = malloc( sizeof(RBT_TREE) );
	if ( new_tree == NULL ) {
		RBT_ERROR_STOP(EXIT_FAILURE, "Cannot allocate more memory.");
	}
	new_tree->root = NULL;
	new_tree->node_count = 0;
	return new_tree;
}

static void RBT_recursive_destroy(RBT_NODE *node) {

	if ( node == NULL ) {
		return;
	} else {
		RBT_recursive_destroy( node->left );		
		RBT_recursive_destroy( node->right );

		RBT_destroy_node( node );
	}
}

void RBT_destroy_tree(RBT_TREE *tree) {
	RBT_recursive_destroy(tree->root);
	free(tree);
}

static void RBT_left_rotate(RBT_TREE *tree, RBT_NODE *node) {
	if ( node->right != NULL ) {
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

static void RBT_right_rotate(RBT_TREE *tree, RBT_NODE *node) {
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

static void RBT_insert_fixup(RBT_TREE *tree, RBT_NODE *node) {
	if ( tree == NULL || node == NULL ) {
		return;
	} 
	while ( RBT_IS_RED( node->parent ) ) {
		if ( node->parent == node->parent->parent->left ) {
			RBT_NODE *right_node = node->parent->parent->right;
			if ( RBT_IS_RED( right_node ) ) {
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
			if ( RBT_IS_RED( left_node ) ) {
                node->parent->color = BLACK;
                left_node->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
				continue;
	        } else if ( node == node->parent->left ) {
                node = node->parent;
                RBT_right_rotate(tree, node);
	        }
			node->parent->color = BLACK;
            node->parent->parent->color = RED;
            RBT_left_rotate(tree, node->parent->parent);
		}
	}
	tree->root->color = BLACK;
}

static RBT_NODE *RBT_find_parent( RBT_TREE *tree, RBT_NODE *node ) {
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
	return parent;
}

static RBT_NODE *RBT_insert(RBT_TREE *tree, RBT_NODE *node) {
	
	RBT_NODE *parent = RBT_find_parent(tree, node);

	node->parent = parent; // setting parent node and fixing forward pointers

	if ( parent == NULL ) {
		tree->root = node;
	} else if ( node->key < parent->key  ) {
		parent->left = node;
	} else {
		parent->right = node;
	}
	
	node->left = NULL;
	node->right = NULL;
	node->color = RED;
	RBT_insert_fixup(tree, node);
	return node;
}

RBT_NODE *RBT_add(RBT_TREE *tree, int key) {
	tree->node_count++;
	return RBT_insert(tree, RBT_new_node(key));
}

void pretty_printer(RBT_NODE *from_node) {
	if ( from_node == NULL ) {
		return;
	}
	pretty_printer_helper(from_node, 0);
}

static void pretty_printer_helper(RBT_NODE *node, int indent) {

	if ( node == NULL ) {
		//printf("%*s%i: NULL\n", indent, " ", (indent / RBT_TAB_SIZE) + 1);
		return;
	} else {

		printf("%*s%i: %i,%c\n", indent, " ", (indent / RBT_TAB_SIZE) + 1, node->key, RBT_COLOR_CHAR(node));

		pretty_printer_helper(node->left, indent + RBT_TAB_SIZE);		
		pretty_printer_helper(node->right, indent + RBT_TAB_SIZE);

	}
}