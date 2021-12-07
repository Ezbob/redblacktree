/**
 * Red-black tree
 *
 * Based on the design from Cormen et. al.
 *
 * Implemented by Anders Busch (2016-2021)
 **/
#include "RBTree/RBTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define RBT_ERROR(message) fprintf(stderr, "Error: %s\n", message)
#define RBT_ERROR_STOP(code, message)  do { fprintf(stderr, "Error: %s\n", message); exit(code); } while ( 0 )

// Linear bounded stack for printing purposes
struct RBT_Stack {
    size_t size;
    size_t next_index;
    char *buffer;
};

#define RBT_COLOR_CHAR(node) RBT_IS_RED(node) ? 'r' : 'b'
#define RBT_TAB_SIZE 4

#define RBT_COLOR_BITMASK (UINTMAX_C(1) << (8 * sizeof(uintmax_t) - 1))

#define RBT_IS_RED(node) (node && ((node->key & RBT_COLOR_BITMASK) != 0))
#define RBT_IS_BLACK(node)  (! RBT_IS_RED(node))

// highest significat bit set = RED
#define RBT_SET_BLACK(node) ( (node)->key &= ~(RBT_COLOR_BITMASK) )
#define RBT_SET_RED(node) ( (node)->key |= RBT_COLOR_BITMASK )

#define RBT_COPY_COLOR(dest, source) (RBT_IS_RED(source) ? RBT_SET_RED(dest) : RBT_SET_BLACK(dest))

#define RBT_IS_KEY_BLACK(key) ((key & RBT_COLOR_BITMASK) == 0)

#define RBT_KEYVALUE(key) (key & (~RBT_COLOR_BITMASK))


/* ---- PRIVATE FUNCTIONS ---- */


static inline struct RBT_Node *RBT_new_node(struct RBT_Tree *tree, uintmax_t key, void *data ) {
    struct RBT_Node *new_node = RBT_MALLOC( sizeof(struct RBT_Node) );
    if ( !new_node ) {
        return NULL;
    }
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->key = key;
    new_node->data = data;
    return new_node;
}

static inline void RBT_destroy_node(struct RBT_Tree *tree, struct RBT_Node *node, void (*freedata)(void *)) {
    if ( !node || !tree ) {
        return;
    }
    if (freedata) {
        freedata(node->data);
    }
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    RBT_FREE(node);
}

static void RBT_recursive_destroy(struct RBT_Tree *tree, struct RBT_Node *node, void (*freedata)(void *)) {
    if ( !node || !tree ) {
        return;
    }
    RBT_recursive_destroy(tree, node->left, freedata );
    RBT_recursive_destroy(tree, node->right, freedata );

    RBT_destroy_node(tree, node, freedata);
}

static inline struct RBT_Node *RBT_minimum(struct RBT_Node *node) {
    if ( node == NULL ) {
        return NULL;
    } else if ( node->left == NULL ) {
        return node;
    }
    struct RBT_Node *iterator = node->left;
    while (iterator->left != NULL) {
        iterator = iterator->left;
    }
    return iterator;
}

static inline struct RBT_Node *RBT_maximum(struct RBT_Node *node) {
    if ( node == NULL ) {
        return NULL;
    } else if ( node->right == NULL ) {
        return node;
    }
    struct RBT_Node *iterator = node->right;
    while ( iterator->right != NULL ) {
        iterator = iterator->right;
    }
    return iterator;
}

static inline void RBT_left_rotate(struct RBT_Tree *tree, struct RBT_Node *node) {
    if ( node->right != NULL ) {
        struct RBT_Node *right_node = node->right;
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

static inline void RBT_right_rotate(struct RBT_Tree *tree, struct RBT_Node *node) {
    if ( node->left != NULL ) {
        struct RBT_Node *left_node = node->left;
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

static inline struct RBT_Node *RBT_find_parent(struct RBT_Tree *tree, struct RBT_Node *node) {
    struct RBT_Node *parent = NULL;
    struct RBT_Node *iterator = tree->root;

    while ( iterator != NULL ) { // traversal of the tree finding parent of node
        parent = iterator;

        if ( RBT_KEYVALUE(node->key) < RBT_KEYVALUE(iterator->key) ) {
            iterator = iterator->left;
        } else {
            iterator = iterator->right;
        }
    }
    return parent;
}

static inline void RBT_insert_fixup(struct RBT_Tree *tree, struct RBT_Node *node ) {
    if ( tree == NULL || node == NULL ) {
        return;
    }
    while ( RBT_IS_RED( node->parent ) ) {
        if ( node->parent == node->parent->parent->left ) {
            struct RBT_Node *right_node = node->parent->parent->right;
            if ( RBT_IS_RED( right_node ) ) {
                // uncle and parent of node is red -> color them black
                RBT_SET_BLACK( node->parent );
                RBT_SET_BLACK( right_node );
                RBT_SET_RED( node->parent->parent );
                node = node->parent->parent;
                continue;
            } else if ( node == node->parent->right ) {
                // uncle is black
                node = node->parent;
                RBT_left_rotate(tree, node);
            }

            RBT_SET_BLACK(node->parent);
            RBT_SET_RED(node->parent->parent);
            RBT_right_rotate(tree, node->parent->parent);
        } else {
            struct RBT_Node *left_node = node->parent->parent->left;
            if ( RBT_IS_RED( left_node ) ) {
                RBT_SET_BLACK( node->parent );
                RBT_SET_BLACK( left_node );
                RBT_SET_RED( node->parent->parent );
                node = node->parent->parent;
                continue;
            } else if ( node == node->parent->left ) {
                node = node->parent;
                RBT_right_rotate(tree, node);
            }
            RBT_SET_BLACK(node->parent);
            RBT_SET_RED(node->parent->parent);
            RBT_left_rotate(tree, node->parent->parent);
        }
    }
    RBT_SET_BLACK(tree->root);
}

static inline void RBT_remove_fixup(struct RBT_Tree *tree, struct RBT_Node *node) {
    while ( RBT_IS_BLACK( node ) && node != tree->root ) {
        if ( node == node->parent->left ) {
            struct RBT_Node *sibling = node->parent->right;
            if ( RBT_IS_RED( sibling ) ) {
                RBT_SET_BLACK( sibling );
                RBT_SET_RED( node->parent );
                RBT_left_rotate( tree, node->parent );
                sibling = node->parent->right;
            }
            if ( RBT_IS_BLACK( sibling->left ) && RBT_IS_BLACK( sibling->right ) ) {
                RBT_SET_RED( sibling );
                node = node->parent;
            } else if ( RBT_IS_BLACK( sibling->right ) ) {
                RBT_SET_BLACK(sibling->left);
                RBT_SET_RED(sibling);
                RBT_right_rotate( tree, sibling );
                sibling = node->parent->right;
            }
            RBT_COPY_COLOR(sibling, node->parent);
            RBT_SET_BLACK(node->parent);
            RBT_SET_BLACK(sibling->right);
            RBT_left_rotate( tree, node->parent );
            node = tree->root;
        } else {
            struct RBT_Node *sibling = node->parent->left;
            if ( RBT_IS_RED( sibling ) ) {
                RBT_SET_BLACK( sibling );
                RBT_SET_RED( node->parent );
                RBT_right_rotate( tree, node->parent );
                sibling = node->parent->left;
            }
            if ( RBT_IS_BLACK( sibling->right ) && RBT_IS_BLACK( sibling->left ) ) {
                RBT_SET_RED( sibling );
                node = node->parent;
            } else if ( RBT_IS_BLACK( sibling->left ) ) {
                RBT_SET_BLACK(sibling->right);
                RBT_SET_RED(sibling);
                RBT_left_rotate( tree, sibling );
                sibling = node->parent->left;
            }
            RBT_COPY_COLOR(sibling, node->parent);
            RBT_SET_BLACK(node->parent);
            RBT_SET_BLACK(sibling->left);
            RBT_right_rotate( tree, node->parent );
            node = tree->root;
        }
    }
    RBT_SET_BLACK(node);
}

static inline struct RBT_Node *RBT_insert(struct RBT_Tree *tree, struct RBT_Node *node) {
    struct RBT_Node *parent = RBT_find_parent(tree, node);

    node->parent = parent; // setting parent node and fixing forward pointers

    if ( parent == NULL ) {
        tree->root = node;
    } else if ( RBT_KEYVALUE(node->key) < RBT_KEYVALUE(parent->key) ) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    node->left = NULL;
    node->right = NULL;
    RBT_SET_RED(node);
    RBT_insert_fixup( tree, node );
    return node;
}

static inline void RBT_transplant_tree(struct RBT_Tree *tree, struct RBT_Node *old, struct RBT_Node *transplant) {

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

static inline struct RBT_Node *RBT_iterative_find(struct RBT_Node *node, size_t key) {
    struct RBT_Node *iterator = node;

    while ( iterator != NULL && RBT_KEYVALUE(iterator->key) != RBT_KEYVALUE(key) ) {
        if ( RBT_KEYVALUE(key) < RBT_KEYVALUE(iterator->key) ) {
            iterator = iterator->left;
        } else {
            iterator = iterator->right;
        }
    }
    return iterator;
}

static inline int RBT_remove(struct RBT_Tree *tree, struct RBT_Node *node ) {
    if ( tree == NULL || node == NULL ) {
        return 0;
    }

    struct RBT_Node *point;
    struct RBT_Node *old = node;
    uintmax_t old_color = node->key;

    if ( node->left == NULL ) {
        point = node->right;
        RBT_transplant_tree(tree, node, node->right);
    } else if ( node->right == NULL ) {
        point = node->left;
        RBT_transplant_tree(tree, node, node->left);
    } else if ( node->right == NULL && node->left == NULL ) {
        // this step is needed since the book uses the T.nill definition
        // which is always black and is a allocated object (in contrast to our NULL)
        RBT_transplant_tree(tree, node, NULL);
    } else {
        old = RBT_minimum( node->right );
        old_color = old->key;
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
        RBT_COPY_COLOR(old, node);
    }
    if ( RBT_IS_KEY_BLACK(old_color) ) {
        RBT_remove_fixup(tree, point);
    }
    RBT_FREE(node);
    tree->node_count--;

    return 1;
}

static inline struct RBT_Stack *RBT_new_stack( size_t size ) {
    if ( size == 0 ) {
        return NULL;
    }

    struct RBT_Stack *new_stack = malloc( sizeof(struct RBT_Stack) );
    if ( new_stack == NULL ) {
        RBT_ERROR_STOP( EXIT_FAILURE, "Cannot allocate more memory." );
        return NULL;
    }
    new_stack->buffer = calloc( sizeof(char), size );

    new_stack->size = size;
    new_stack->next_index = 0;

    return new_stack;
}

static inline void RBT_destroy_stack( struct RBT_Stack * stack ) {
    if ( stack != NULL ) {
        free(stack->buffer);
        free(stack);
    }
}

static inline void RBT_pretty_push( struct RBT_Stack * stack, char character ) {
    if ( stack->size > 0 && stack->next_index + 5 <= stack->size ) {
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index++ ] = character;
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index ] = '\0';
    }
}

static inline void RBT_pretty_pop( struct RBT_Stack * stack ) {
    if ( stack->size > 0 && stack->next_index > 0 ) {
        stack->buffer[ stack->next_index -= 4 ] = '\0';
    }
}

static void RBT_pretty_printer_helper( struct RBT_Node *node, struct RBT_Stack *stack ) {

    if ( stack == NULL ) {
        return;
    }

    if ( node == NULL ) {
        printf("(NULL, b)\n");
        return;
    } else {
        printf("(k:%zu, c:%c, d:%p)\n", RBT_KEYVALUE(node->key), RBT_COLOR_CHAR(node), node->data);
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


/* --- PUBLIC FUNCTIONS --- */


int RBT_init_tree(struct RBT_Tree *tree) {
    if ( !tree ) {
        return 0;
    }
    tree->root = NULL;
    tree->node_count = 0;
    return 1;
}

void RBT_deinit_tree(struct RBT_Tree *tree, void (*freedata)(void *)) {
    RBT_recursive_destroy(tree, tree->root, freedata);
}

void *RBT_add( struct RBT_Tree *tree, size_t key, void *data ) {
    tree->node_count++;
    struct RBT_Node *inserted = RBT_insert(tree, RBT_new_node(tree, key, data));
    return (inserted == NULL) ? inserted : inserted->data;
}

void *RBT_find(struct RBT_Tree *tree, size_t key) {
    if ( tree == NULL ) {
        return NULL;
    }
    struct RBT_Node *node = RBT_iterative_find(tree->root, key);
    return node == NULL ? node : node->data;
}

int RBT_delete(struct RBT_Tree *tree, size_t key) {
    struct RBT_Node *find_node = RBT_iterative_find( tree->root, key );

    if ( find_node != NULL ) {
        return RBT_remove( tree, find_node );
    }
    return 0;
}

int RBT_get_maximum(struct RBT_Tree *tree, size_t *key, void **value) {
    struct RBT_Node *node = RBT_maximum(tree->root);
    if (!node) {
        return 0;
    }
    if (key) {
        *key = RBT_KEYVALUE(node->key);
    }
    if (value) {
        *value = node->data;
    }

    return 1;
}

int RBT_get_minimum(struct RBT_Tree *tree, size_t *key, void **value) {
    struct RBT_Node *node = RBT_minimum(tree->root);
    if (!node) {
        return 0;
    }
    if (key) {
        *key = RBT_KEYVALUE(node->key);
    }
    if (value) {
        *value = node->data;
    }

    return 1;
}

void RBT_pretty_printer(struct RBT_Tree *tree) {
    if ( tree == NULL ) {
        return;
    }
    struct RBT_Stack *stack = RBT_new_stack(2046);
    RBT_pretty_printer_helper(tree->root, stack);
    RBT_destroy_stack(stack);
}