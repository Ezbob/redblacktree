/**
 * Red-black tree
 * This is a red-black tree based on the description from the Cormen
 * textbook.
 *
 * Unlike Cormens version, this version uses NULL pointers to mark
 * end nodes instead of keeping a allocated nill node that is black
 * but can take on any value.
 *
 * Implemented by Anders Busch (2016-2021)
 *
 **/
#ifndef _HEADER_FILE_RBTree_20211208190717_
#define _HEADER_FILE_RBTree_20211208190717_

#include <stdint.h>

/**
 * Internal RBT tree node, carrying a key and a data reference,
 * as well as reference to sub and parent nodes.
 *
 * Coloring is determinted by the most significate bit of the key with
 * a set bit marking the node as red, and a cleared bit marking the node as black.
 * On 64 bit systems this gives a 2^63 number of unique keys.
 */
struct RBT_Node {
    uintmax_t key;
    void *data;
    struct RBT_Node *left;
    struct RBT_Node *right;
    struct RBT_Node *parent;
};

/**
 * Front facade for the RBT tree carrying the root node,
 * as well as some meta data.
 */
struct RBT_Tree {
    struct RBT_Node *root;
    uintmax_t node_count;
};

/**
 *  RBT tree initialization.
 *  The memory allocation of the RBT_Tree is owned by the caller.
 *  @returns a non-zero value on success, and zero on failure.
 */
int RBT_init_tree(struct RBT_Tree *tree);

/**
 * RBT tree de-initialization.
 * Deallocates any internal nodes of a initialized tree. If a data_deallocator is provided, it is
 * called for every value stored in the tree just before the node is destroyed.
 * As the memory for the RBT_Tree is considered owned by the caller, it is NOT freed by this function.
 */
void RBT_deinit_tree(struct RBT_Tree *tree, void (*)(void *data_deallocator));

/**
 * Adds a new node to the tree with the given key and value.
 * @returns The added value, if any, NULL otherwise.
 */
void *RBT_add(struct RBT_Tree *, uintmax_t, void *);

/**
 * Delete a node with the given key from the given RBT tree.
 * @returns a non-zero value on successful deletion, zero otherwise.
 */
int RBT_delete(struct RBT_Tree *, uintmax_t);

/**
 * Finds a value in the RBT tree given a key.
 * @returns the found value, if any, NULL otherwise.
 */
void *RBT_find(struct RBT_Tree *, uintmax_t);

/**
 * Finds the key and value in the tree of the element with the trees' maximum key value.
 * "key" and "value" are both optional (they can be NULL) output variables that gets written to,
 * if any element was found.
 * @returns a non-zero value on successful deletion, zero otherwise.
 */
int RBT_get_maximum(struct RBT_Tree *tree, uintmax_t *key, void **value);

/**
 * Finds the key and value in the tree of the element with the trees' minimum key value.
 * "key" and "value" are both optional (they can be NULL) output variables that gets written to,
 * if any element was found.
 * @returns a non-zero value on successful deletion, zero otherwise.
 */
int RBT_get_minimum(struct RBT_Tree *tree, uintmax_t *key, void **value);

/**
 * Convience macro for getting the node count of a RBT tree
 */
#define RBT_NODE_COUNT(tree_ptr) ((tree_ptr)->node_count)

/*
 * memory allocation function. This function is given a size_t of RBT_Node as the
 * first argument to allocate a tree node of sizeof(RBT_Node)
 */
#ifndef RBT_MALLOC
#define RBT_MALLOC malloc
#endif

/*
 * memory deallocation function. This function is given a pointer to an allocated
 * node as the first argument, which it is expected to free.
 */
#ifndef RBT_FREE
#define RBT_FREE free
#endif

#endif
