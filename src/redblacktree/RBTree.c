#include "RBTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define RBT_ERROR(message) fprintf(stderr, "Error: %s\n", message)
#define RBT_ERROR_STOP(code, message) fprintf(stderr, "Error: %s\n", message); exit(code)

// NULL values are black per difinition, so node has  
// to be non-NULL for it to be RBT_RED

#define RBT_COLOR_CHAR(node) RBT_IS_RED(node) ? 'r' : 'b' 
#define RBT_TAB_SIZE 4

typedef struct RBT_STACK {
	size_t size;
	size_t next_index;
	char *buffer; 
} RBT_STACK;

static RBT_NODE *RBT_new_node( int, void *data );
static void RBT_destroy_node(RBT_NODE *);
static void RBT_left_rotate(RBT_TREE *, RBT_NODE *);
static void RBT_right_rotate(RBT_TREE *, RBT_NODE *);
static void RBT_insert_fixup(RBT_TREE *, RBT_NODE *);
static RBT_NODE *RBT_insert(RBT_TREE *, RBT_NODE *);
static void RBT_recursive_destroy(RBT_NODE *);
static RBT_STACK *RBT_new_stack( size_t size );
static void RBT_destroy_stack( RBT_STACK * stack );
static void RBT_pretty_printer_helper(RBT_NODE *node, RBT_STACK *);
static void RBT_pretty_push( RBT_STACK * stack, char character);
static void RBT_pretty_pop( RBT_STACK * stack );
static RBT_NODE *RBT_find_parent(RBT_TREE *, RBT_NODE *);
static int RBT_remove(RBT_TREE *, RBT_NODE *);
static void RBT_remove_fixup(RBT_TREE *, RBT_NODE *);
static void RBT_transplant_tree(RBT_TREE *, RBT_NODE *, RBT_NODE *);
static RBT_NODE *RBT_minimum(RBT_NODE *);
static RBT_NODE *RBT_maximum(RBT_NODE *);
static RBT_NODE *RBT_iterative_find( RBT_NODE *node, int key );


static RBT_NODE *RBT_new_node( int key, void *data ) {
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
	new_node->data = data;
	new_node->color = RBT_BLACK;
	return new_node;
}

static void RBT_destroy_node( RBT_NODE *node ) {
	if ( node != NULL ) {
		node->data = NULL;
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

void *RBT_add( RBT_TREE *tree, int key, void *data ) {
	tree->node_count++;
	RBT_NODE *inserted = RBT_insert( tree, RBT_new_node( key, data ) );
	return inserted == NULL ? inserted : inserted->data;
}

static RBT_NODE *RBT_minimum( RBT_NODE *node ) {
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

static RBT_NODE *RBT_maximum( RBT_NODE *node ) {
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

static RBT_NODE *RBT_iterative_find( RBT_NODE *node, int key ) {
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

void *RBT_find( RBT_TREE *tree, int key ) {

	if ( tree == NULL ) {
		return NULL;
	}
	RBT_NODE *node = RBT_iterative_find(tree->root, key);
	return node == NULL ? node : node->data ;
}

static int RBT_remove( RBT_TREE *tree, RBT_NODE *node ) {
	if ( tree == NULL || node == NULL ) {
		return 0;
	}

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

	return 1;
}

int RBT_delete( RBT_TREE *tree, int key ) {
	RBT_NODE *find_node = RBT_iterative_find( tree->root, key );

	if ( find_node != NULL ) {
		return RBT_remove( tree, find_node );	
	} 
	return 0;	
}

void *RBT_get_maximum(RBT_TREE *tree) {
	RBT_NODE *node = RBT_maximum(tree->root);
	return node == NULL ? node : node->data;
}

void *RBT_get_minimum(RBT_TREE *tree) {
	RBT_NODE *node = RBT_minimum(tree->root);
	return node == NULL ? node : node->data;
}


static RBT_STACK *RBT_new_stack( size_t size ) {
	RBT_STACK *new_stack = malloc( sizeof(RBT_STACK) );
	new_stack->buffer = calloc( sizeof(char), size );
	
	new_stack->size = size;
	new_stack->next_index = 0;

	return new_stack;
}

static void RBT_destroy_stack( RBT_STACK * stack ) {
	free(stack->buffer);
	free(stack);
}

static void RBT_pretty_push( RBT_STACK * stack, char character ) {
	assert(stack->next_index <= stack->size - 5);

	stack->buffer[ stack->next_index++ ] = ' ';
	stack->buffer[ stack->next_index++ ] = character;
	stack->buffer[ stack->next_index++ ] = ' ';
	stack->buffer[ stack->next_index++ ] = ' ';
	stack->buffer[ stack->next_index ] = '\0';
}

static void RBT_pretty_pop( RBT_STACK * stack ) {
	stack->buffer[ stack->next_index -= 4 ] = '\0';
}

void RBT_pretty_printer(RBT_NODE *from_node) {
	if ( from_node == NULL ) {
		return;
	}
	RBT_STACK *stack = RBT_new_stack(2046);
	RBT_pretty_printer_helper(from_node, stack);
	RBT_destroy_stack(stack);
}

static void RBT_pretty_printer_helper(RBT_NODE *node, RBT_STACK *stack) {

	if ( stack == NULL ) {
		return;
	}

	if (node == NULL) {
		printf("(NULL, b)\n");
		return;
	} else {
		printf("(k:%i,c:%c,d:%p)\n", node->key, RBT_COLOR_CHAR(node), node->data);	
	}

	printf( "%s |--", stack->buffer );
	RBT_pretty_push(stack, '|');
	RBT_pretty_printer_helper(node->left, stack);
	RBT_pretty_pop(stack);

	printf( "%s `--", stack->buffer );
	RBT_pretty_push(stack, ' ');
	RBT_pretty_printer_helper(node->right, stack);
	RBT_pretty_pop(stack);
	
}