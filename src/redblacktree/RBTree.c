/**
 * Red-black tree
 *
 * Based on the design from Cormen et. al.
 *
 * Implemented by Anders Busch (2016)
 **/
#include "RBTree/RBTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define RBT_ERROR(message) fprintf(stderr, "Error: %s\n", message)
#define RBT_ERROR_STOP(code, message)  do { fprintf(stderr, "Error: %s\n", message); exit(code); } while ( 0 )

// NULL values are black per difinition, so node has
// to be non-NULL for it to be RBT_RED

#define RBT_COLOR_CHAR(node) RBT_IS_RED(node) ? 'r' : 'b'
#define RBT_TAB_SIZE 4

#define RBT_IS_RED(node) (node != NULL && node->color == RBT_RED)
#define RBT_IS_BLACK(node) (node == NULL || node->color == RBT_BLACK)

// Linear bounded stack for printing purposes
struct RBT_STACK {
    size_t size;
    size_t next_index;
    char *buffer;
};

struct RBT_NODE *RBT_new_node( int, void *data );
void RBT_destroy_node(struct RBT_NODE *, void (*f)(void *));
void RBT_left_rotate(struct RBT_TREE *, struct RBT_NODE *);
void RBT_right_rotate(struct RBT_TREE *, struct RBT_NODE *);
void RBT_insert_fixup(struct RBT_TREE *, struct RBT_NODE *);
struct RBT_NODE *RBT_insert(struct RBT_TREE *, struct RBT_NODE *);
void RBT_recursive_destroy(struct RBT_NODE *, void (*f)(void *));
struct RBT_STACK *RBT_new_stack( size_t size );
void RBT_destroy_stack(struct RBT_STACK * stack);
void RBT_pretty_printer_helper(struct RBT_NODE *node, struct RBT_STACK *);
void RBT_pretty_push(struct RBT_STACK * stack, char character);
void RBT_pretty_pop(struct RBT_STACK * stack );
struct RBT_NODE *RBT_find_parent(struct RBT_TREE *, struct RBT_NODE *);
int RBT_remove(struct RBT_TREE *, struct RBT_NODE *);
void RBT_remove_fixup(struct RBT_TREE *, struct RBT_NODE *);
void RBT_transplant_tree(struct RBT_TREE *, struct RBT_NODE *, struct RBT_NODE *);
struct RBT_NODE *RBT_minimum(struct RBT_NODE *);
struct RBT_NODE *RBT_maximum(struct RBT_NODE *);
struct RBT_NODE *RBT_iterative_find(struct RBT_NODE *node, size_t key );
struct RBT_PAIR *RBT_new_pair(size_t key, void *value);

struct RBT_PAIR *RBT_new_pair(size_t key, void *value) {
    struct RBT_PAIR *pair = malloc( sizeof(struct RBT_PAIR) );
    if ( pair == NULL ) {
        RBT_ERROR_STOP( EXIT_FAILURE, "Cannot allocate more memory." );
        return NULL;
    }

    pair->key = key;
    pair->value = value;

    return pair;
}

void RBT_destroy_pair(struct RBT_PAIR *pair) {
    pair->value = NULL;
    free(pair);
}

struct RBT_NODE *RBT_new_node( int key, void *data ) {
    struct RBT_NODE *new_node;
    new_node = malloc( sizeof(struct RBT_NODE) );
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

void RBT_destroy_node(struct RBT_NODE *node, void (*freedata)(void *)) {
    if ( node != NULL ) {
        freedata(node->data);
        node->left = NULL;
        node->right = NULL;
        node->parent = NULL;
        free(node);
    }
}

struct RBT_TREE *RBT_init_tree() {
    struct RBT_TREE *new_tree;
    new_tree = malloc( sizeof(struct RBT_TREE) );
    if ( new_tree == NULL ) {
        RBT_ERROR_STOP( EXIT_FAILURE, "Cannot allocate more memory." );
    }
    new_tree->root = NULL;
    new_tree->node_count = 0;
    return new_tree;
}

void RBT_recursive_destroy(struct RBT_NODE *node, void (*freedata)(void *)) {

    if ( node != NULL ) {
        RBT_recursive_destroy( node->left, freedata );
        RBT_recursive_destroy( node->right, freedata );

        RBT_destroy_node( node, freedata );
    }
}

void RBT_destroy_tree(struct RBT_TREE *tree, void (*freedata)(void *)) {
    RBT_recursive_destroy(tree->root, freedata);
    free(tree);
}

void RBT_left_rotate(struct RBT_TREE *tree, struct RBT_NODE *node) {
    if ( node->right != NULL ) {
        struct RBT_NODE *right_node = node->right;
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

void RBT_right_rotate(struct RBT_TREE *tree, struct RBT_NODE *node) {
    if ( node->left != NULL ) {
        struct RBT_NODE *left_node = node->left;
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

void RBT_insert_fixup(struct RBT_TREE *tree, struct RBT_NODE *node ) {
    if ( tree == NULL || node == NULL ) {
        return;
    }
    while ( RBT_IS_RED( node->parent ) ) {
        if ( node->parent == node->parent->parent->left ) {
            struct RBT_NODE *right_node = node->parent->parent->right;
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
            struct RBT_NODE *left_node = node->parent->parent->left;
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

struct RBT_NODE *RBT_find_parent(struct RBT_TREE *tree, struct RBT_NODE *node) {
    struct RBT_NODE *parent = NULL;
    struct RBT_NODE *iterator = tree->root;

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

struct RBT_NODE *RBT_insert(struct RBT_TREE *tree, struct RBT_NODE *node) {

    struct RBT_NODE *parent = RBT_find_parent(tree, node);

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

void *RBT_add( struct RBT_TREE *tree, size_t key, void *data ) {
    tree->node_count++;
    struct RBT_NODE *inserted = RBT_insert(tree, RBT_new_node( key, data ));
    return inserted == NULL ? inserted : inserted->data;
}

struct RBT_NODE *RBT_minimum(struct RBT_NODE *node) {
    if ( node == NULL ) {
        return NULL;
    } else if ( node->left == NULL ) {
        return node;
    }
    struct RBT_NODE *iterator = node->left;
    while (iterator->left != NULL) {
        iterator = iterator->left;
    }
    return iterator;
}

struct RBT_NODE *RBT_maximum(struct RBT_NODE *node) {
    if ( node == NULL ) {
        return NULL;
    } else if ( node->right == NULL ) {
        return node;
    }
    struct RBT_NODE *iterator = node->right;
    while ( iterator->right != NULL ) {
        iterator = iterator->right;
    }
    return iterator;
}

void RBT_transplant_tree(struct RBT_TREE *tree, struct RBT_NODE *old, struct RBT_NODE *transplant) {

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

void RBT_remove_fixup(struct RBT_TREE *tree, struct RBT_NODE *node) {
    while ( RBT_IS_BLACK( node ) && node != tree->root ) {
        if ( node == node->parent->left ) {
            struct RBT_NODE *sibling = node->parent->right;
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
            struct RBT_NODE *sibling = node->parent->left;
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

struct RBT_NODE *RBT_iterative_find(struct RBT_NODE *node, size_t key) {
    struct RBT_NODE *iterator = node;

    while ( iterator != NULL && iterator->key != key ) {
        if ( key < iterator->key ) {
            iterator = iterator->left;
        } else {
            iterator = iterator->right;
        }
    }
    return iterator;
}

void *RBT_find(struct RBT_TREE *tree, size_t key) {

    if ( tree == NULL ) {
        return NULL;
    }
    struct RBT_NODE *node = RBT_iterative_find(tree->root, key);
    return node == NULL ? node : node->data ;
}

int RBT_remove(struct RBT_TREE *tree, struct RBT_NODE *node ) {
    if ( tree == NULL || node == NULL ) {
        return 0;
    }

    struct RBT_NODE *point;
    struct RBT_NODE *old = node;
    enum RBT_COLOR old_color = node->color;

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

int RBT_delete(struct RBT_TREE *tree, size_t key) {
    struct RBT_NODE *find_node = RBT_iterative_find( tree->root, key );

    if ( find_node != NULL ) {
        return RBT_remove( tree, find_node );
    }
    return 0;
}

int RBT_get_maximum(struct RBT_TREE *tree, size_t *key, void **value) {
    struct RBT_NODE *node = RBT_maximum(tree->root);
    if (!node) {
        return 0;
    }
    if (key) {
        *key = node->key;
    }
    if (value) {
        *value = node->data;
    }

    return 1;
}


int RBT_get_minimum(struct RBT_TREE *tree, size_t *key, void **value) {
    struct RBT_NODE *node = RBT_minimum(tree->root);
    if (!node) {
        return 0;
    }
    if (key) {
        *key = node->key;
    }
    if (value) {
        *value = node->data;
    }

    return 1;
}


struct RBT_STACK *RBT_new_stack( size_t size ) {
    if ( size == 0 ) {
        return NULL;
    }

    struct RBT_STACK *new_stack = malloc( sizeof(struct RBT_STACK) );
    if ( new_stack == NULL ) {
        RBT_ERROR_STOP( EXIT_FAILURE, "Cannot allocate more memory." );
        return NULL;
    }
    new_stack->buffer = calloc( sizeof(char), size );

    new_stack->size = size;
    new_stack->next_index = 0;

    return new_stack;
}

void RBT_destroy_stack( struct RBT_STACK * stack ) {
    if ( stack != NULL ) {
        free(stack->buffer);
        free(stack);
    }
}

void RBT_pretty_push( struct RBT_STACK * stack, char character ) {
    if ( stack->size > 0 && stack->next_index + 5 <= stack->size ) {
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index++ ] = character;
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index ] = '\0';
    }
}

void RBT_pretty_pop( struct RBT_STACK * stack ) {
    if ( stack->size > 0 && stack->next_index > 0 ) {
        stack->buffer[ stack->next_index -= 4 ] = '\0';
    }
}

void RBT_pretty_printer( struct RBT_NODE *from_node ) {
    if ( from_node == NULL ) {
        return;
    }
    struct RBT_STACK *stack = RBT_new_stack(2046);
    RBT_pretty_printer_helper(from_node, stack);
    RBT_destroy_stack(stack);
}

void RBT_pretty_printer_helper( struct RBT_NODE *node, struct RBT_STACK *stack ) {

    if ( stack == NULL ) {
        return;
    }

    if ( node == NULL ) {
        printf("(NULL, b)\n");
        return;
    } else {
        printf("(k:%zu, c:%c, d:%p)\n", node->key, RBT_COLOR_CHAR(node), node->data);
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

