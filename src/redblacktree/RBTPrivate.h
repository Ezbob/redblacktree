
#ifndef _HEADER_FILE_RBTPrivate_20211205193305_
#define _HEADER_FILE_RBTPrivate_20211205193305_

struct RBT_Tree;
struct RBT_Node;

#define RBT_ERROR(message) fprintf(stderr, "Error: %s\n", message)
#define RBT_ERROR_STOP(code, message)  do { fprintf(stderr, "Error: %s\n", message); exit(code); } while ( 0 )

// NULL values are black per definition, so node has
// to be non-NULL for it to be RBT_RED

#define RBT_COLOR_CHAR(node) RBT_IS_RED(node) ? 'r' : 'b'
#define RBT_TAB_SIZE 4

#define RBT_IS_RED(node) (node != NULL && node->color == RBT_RED)
#define RBT_IS_BLACK(node) (node == NULL || node->color == RBT_BLACK)

// Linear bounded stack for printing purposes
struct RBT_Stack {
    size_t size;
    size_t next_index;
    char *buffer;
};

struct RBT_Node *RBT_new_node(struct RBT_Tree *, int, void *);
void RBT_destroy_node(struct RBT_Tree *, struct RBT_Node *, void (*f)(void *));
void RBT_left_rotate(struct RBT_Tree *, struct RBT_Node *);
void RBT_right_rotate(struct RBT_Tree *, struct RBT_Node *);
void RBT_insert_fixup(struct RBT_Tree *, struct RBT_Node *);
struct RBT_Node *RBT_insert(struct RBT_Tree *, struct RBT_Node *);
void RBT_recursive_destroy(struct RBT_Tree *tree, struct RBT_Node *node, void (*freedata)(void *));
struct RBT_Stack *RBT_new_stack( size_t size );
void RBT_destroy_stack(struct RBT_Stack * stack);
void RBT_pretty_printer_helper(struct RBT_Node *node, struct RBT_Stack *);
void RBT_pretty_push(struct RBT_Stack * stack, char character);
void RBT_pretty_pop(struct RBT_Stack * stack );
struct RBT_Node *RBT_find_parent(struct RBT_Tree *, struct RBT_Node *);
int RBT_remove(struct RBT_Tree *, struct RBT_Node *);
void RBT_remove_fixup(struct RBT_Tree *, struct RBT_Node *);
void RBT_transplant_tree(struct RBT_Tree *, struct RBT_Node *, struct RBT_Node *);
struct RBT_Node *RBT_minimum(struct RBT_Node *);
struct RBT_Node *RBT_maximum(struct RBT_Node *);
struct RBT_Node *RBT_iterative_find(struct RBT_Node *node, size_t key );

#endif