#include "RBTree.h"
#include <stdlib.h>
#include <stdio.h>

#define RBT_ERROR(message) fprintf(stderr, "Error: %s\n", message)
#define RBT_ERROR_STOP(code, message) fprintf(stderr, "Error: %s\n", message); exit(code)

// NULL values are black per difinition, so node has  
// to be non-NULL for it to be RBT_RED
#define RBT_IS_RED(node) (node != NULL && node->color == RBT_RED)
#define RBT_IS_BLACK(node) (node == NULL || node->color == RBT_BLACK)
#define RBT_COLOR_CHAR(node) RBT_IS_RED(node) ? 'r' : 'b' 
#define RBT_TAB_SIZE 4

RBT_NODE *RBT_new_node(int);
RBT_TREE *RBT_init_tree(void);
void RBT_destroy_node(RBT_NODE *);
static void RBT_left_rotate(RBT_TREE *, RBT_NODE *);
static void RBT_right_rotate(RBT_TREE *, RBT_NODE *);
static void RBT_insert_fixup(RBT_TREE *, RBT_NODE *);
static RBT_NODE *RBT_insert(RBT_TREE *, RBT_NODE *);
static void RBT_recursive_destroy(RBT_NODE *);
static void RBT_pretty_printer_helper(RBT_NODE *node, int indent);
static RBT_NODE *RBT_find_parent(RBT_TREE *, RBT_NODE *);
static void RBT_remove(RBT_TREE *, RBT_NODE *);
static void RBT_remove_fixup(RBT_TREE *, RBT_NODE *);
static void RBT_transplant_tree(RBT_TREE *, RBT_NODE *, RBT_NODE *);
static RBT_NODE *RBT_minimum(const RBT_NODE *);
static RBT_NODE *RBT_maximum(const RBT_NODE *);
static RBT_NODE *RBT_iterative_find( const RBT_NODE *node, const int key );

RBT_NODE *RBT_new_node( int key ) {
	RBT_NODE *new_node;
	new_node = malloc( sizeof( RBT_NODE ) );
	if ( new_node == NULL ) {
		RBT_ERROR_STOP( EXIT_FAILURE, "Cannot allocate more memory." );
		return NULL;
	}
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->parent = NULL;
	new_node->key = key;
	new_node->color = RBT_BLACK;
	return new_node;
}

void RBT_destroy_node( RBT_NODE *node ) {
	if ( node != NULL ) {
		node->left = NULL;
		node->right = NULL;
		node->parent = NULL;
		free(node);	
	}
}

RBT_TREE *RBT_init_tree() {
	RBT_TREE *new_tree;
	new_tree = malloc( sizeof( RBT_TREE ) );
	if ( new_tree == NULL ) {
		RBT_ERROR_STOP( EXIT_FAILURE, "Cannot allocate more memory." );
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

void RBT_destroy_tree( RBT_TREE *tree ) {
	RBT_recursive_destroy( tree->root );
	free( tree );
}

static void RBT_left_rotate( RBT_TREE *tree, RBT_NODE *node ) {
	if ( node->right != NULL ) {
		RBT_NODE *right_node = node->right;
		node->right = right_node->left;

		if ( right_node->left != NULL ) {
			right_node->left->parent = node;
		}
		right_node->parent = node->parent;

		if ( node->parent == NULL ) {
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

static void RBT_right_rotate( RBT_TREE *tree, RBT_NODE *node ) {
	if ( node->left != NULL ) {
		RBT_NODE *left_node = node->left;
		node->left = left_node->right;
		
		if ( left_node->right != NULL ) {
			left_node->right->parent = node;
		}
		left_node->parent = node->parent;
	
		if ( node->parent == NULL ) {
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

static void RBT_insert_fixup( RBT_TREE *tree, RBT_NODE *node ) {
	if ( tree == NULL || node == NULL ) {
		return;
	} 
	while ( RBT_IS_RED( node->parent ) ) {
		if ( node->parent == node->parent->parent->left ) {
			RBT_NODE *right_node = node->parent->parent->right;
			if ( RBT_IS_RED( right_node ) ) {
				// uncle and parent of node is red -> color them black
				node->parent->color = RBT_BLACK;
				right_node->color = RBT_BLACK;
				node->parent->parent->color = RBT_RED;
				node = node->parent->parent;
				continue;
			} else if ( node == node->parent->right ) {
				// uncle is black
				node = node->parent;
				RBT_left_rotate(tree, node);
			}

			node->parent->color = RBT_BLACK;
			node->parent->parent->color = RBT_RED;
			RBT_right_rotate(tree, node->parent->parent);
		} else {
			RBT_NODE *left_node = node->parent->parent->left;
			if ( RBT_IS_RED( left_node ) ) {
                node->parent->color = RBT_BLACK;
                left_node->color = RBT_BLACK;
                node->parent->parent->color = RBT_RED;
                node = node->parent->parent;
				continue;
	        } else if ( node == node->parent->left ) {
                node = node->parent;
                RBT_right_rotate(tree, node);
	        }
			node->parent->color = RBT_BLACK;
            node->parent->parent->color = RBT_RED;
            RBT_left_rotate(tree, node->parent->parent);
		}
	}
	tree->root->color = RBT_BLACK;
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

static RBT_NODE *RBT_insert( RBT_TREE *tree, RBT_NODE *node ) {
	
	RBT_NODE *parent = RBT_find_parent( tree, node );

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
	node->color = RBT_RED;
	RBT_insert_fixup( tree, node );
	return node;
}

RBT_NODE *RBT_add( RBT_TREE *tree, int key ) {
	tree->node_count++;
	return RBT_insert( tree, RBT_new_node(key) );
}

static RBT_NODE *RBT_minimum( const RBT_NODE *node ) {
	if ( node == NULL ) {
		return NULL;
	} else if ( node->left == NULL ) {
		return node;
	}
	RBT_NODE *iterator = node->left;
	while (iterator->left != NULL) {
		iterator = iterator->left;
	}
	return iterator;
}

static RBT_NODE *RBT_maximum( const RBT_NODE *node ) {
	if ( node == NULL ) {
		return NULL;
	} else if ( node->right == NULL ) {
		return node;
	}
	RBT_NODE *iterator = node->right;
	while ( iterator->right != NULL ) {
		iterator = iterator->right;
	}
	return iterator;
}

static void RBT_transplant_tree( RBT_TREE *tree, RBT_NODE *old, RBT_NODE *transplant ) {

	if ( old->parent == NULL ) {
		tree->root = transplant;
	} else if ( old == old->parent->left ) {
		old->parent->left = transplant;
	} else {
		old->parent->right = transplant;
	}
	if ( transplant != NULL ) {
		transplant->parent = old->parent;	
	} 
}

static void RBT_remove_fixup( RBT_TREE *tree, RBT_NODE *node ) {
	while ( RBT_IS_BLACK( node ) && node != tree->root ) {
		if ( node == node->parent->left ) {
			RBT_NODE *sibling = node->parent->right;
			if ( RBT_IS_RED( sibling ) ) {
				sibling->color = RBT_BLACK;
				node->parent->color = RBT_RED;
				RBT_left_rotate( tree, node->parent );
				sibling = node->parent->right;
			}
			if ( RBT_IS_BLACK( sibling->left ) && RBT_IS_BLACK( sibling->right ) ) {
				sibling->color = RBT_RED;
				node = node->parent;
			} else if ( RBT_IS_BLACK( sibling->right ) ) {
				sibling->left->color = RBT_BLACK;
				sibling->color = RBT_RED;
				RBT_right_rotate( tree, sibling );
				sibling = node->parent->right;
			}
			sibling->color = node->parent->color;
			node->parent->color = RBT_BLACK;
			sibling->right->color = RBT_BLACK;
			RBT_left_rotate( tree, node->parent );
			node = tree->root;
		} else {
			RBT_NODE *sibling = node->parent->left;
			if ( RBT_IS_RED( sibling ) ) {
				sibling->color = RBT_BLACK;
				node->parent->color = RBT_RED;
				RBT_right_rotate( tree, node->parent );
				sibling = node->parent->left;
			}
			if ( RBT_IS_BLACK( sibling->right ) && RBT_IS_BLACK( sibling->left ) ) {
				sibling->color = RBT_RED;
				node = node->parent;
			} else if ( RBT_IS_BLACK( sibling->left ) ) {
				sibling->right->color = RBT_BLACK;
				sibling->color = RBT_RED;
				RBT_left_rotate( tree, sibling );
				sibling = node->parent->left;
			}
			sibling->color = node->parent->color;
			node->parent->color = RBT_BLACK;
			sibling->left->color = RBT_BLACK;
			RBT_right_rotate( tree, node->parent );
			node = tree->root;
		}
	}
	node->color = RBT_BLACK;
}

static RBT_NODE *RBT_iterative_find( const RBT_NODE *node, const int key ) {
	RBT_NODE *iterator = node;

	while ( iterator != NULL && iterator->key != key ) {
		if ( key < iterator->key ) {
			iterator = iterator->left;
		} else {
			iterator = iterator->right;
		}
	}
	return iterator;
}

RBT_NODE *RBT_find( RBT_TREE *tree, int key ) {

	if ( tree == NULL ) {
		return NULL;
	}
	return RBT_iterative_find(tree->root, key);
}

static void RBT_remove( RBT_TREE *tree, RBT_NODE *node ) {

	RBT_NODE *point;
	RBT_NODE *old = node;
	enum RBT_color old_color = node->color;

	if ( node->left == NULL ) {
		point = node->right;
		RBT_transplant_tree(tree, node, node->right);		
	} else if ( node->right == NULL ) {
		point = node->left;
		RBT_transplant_tree(tree, node, node->left);
	} else if ( node->right == NULL && node->left == NULL ) {
		// this step is needed since the book uses the T.nill difinition
		// which is always black and is a allocated object (in contrast to our NULL) 
		RBT_transplant_tree(tree, node, NULL);
	} else {
		old = RBT_minimum( node->right );
		old_color = old->color;
		point = old->right;

		if ( old->parent == node ) {
			if ( point != NULL ) {
				point->parent = old;	
			}
		} else {
			RBT_transplant_tree(tree, old, old->right);
			old->right = node->right;
			old->right->parent = old;
		}
		RBT_transplant_tree(tree, node, old);
		old->left = node->left;
		old->left->parent = old;
		old->color = node->color;
	}
	if ( old_color == RBT_BLACK ) {
		RBT_remove_fixup(tree, point);
	}
	free(node);
	tree->node_count--;
}

void RBT_delete( RBT_TREE *tree, int key ) {
	RBT_NODE *find_node = RBT_iterative_find( tree->root, key );

	if ( find_node != NULL ) {
		RBT_remove( tree, find_node );	
	} 
}

RBT_NODE *RBT_get_maximum(RBT_TREE *tree) {
	return RBT_maximum(tree->root);
}

RBT_NODE *RBT_get_minimum(RBT_TREE *tree) {
	return RBT_minimum(tree->root);
}

void RBT_pretty_printer(RBT_NODE *from_node) {
	if ( from_node == NULL ) {
		return;
	}
	RBT_pretty_printer_helper(from_node, 0);
}

static void RBT_pretty_printer_helper(RBT_NODE *node, int indent) {

	if ( node == NULL ) {
		printf("%*s%i: NULL,b\n", indent, "", (indent / RBT_TAB_SIZE) + 1);
		return;
	} else {

		RBT_pretty_printer_helper(node->right, indent + RBT_TAB_SIZE);

		printf("%*s%i: %i,%c\n", indent, " ", (indent / RBT_TAB_SIZE) + 1, node->key, RBT_COLOR_CHAR(node));
		
		RBT_pretty_printer_helper(node->left, indent + RBT_TAB_SIZE);

	}
}