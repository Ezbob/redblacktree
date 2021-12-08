
#include "RBTree/RBTreePrinter.h"
#include "RBMacros.h"
#include <stdio.h>
#include <stdlib.h>


/** ---- PRIVATE FUNCTIONS ---- **/

// Linear bounded stack for printing purposes
struct RBT_Printer_Stack {
    size_t size;
    size_t next_index;
    char *buffer;
};

inline static struct RBT_Printer_Stack *RBT_new_stack( size_t size ) {
    if ( size == 0 ) {
        return NULL;
    }

    struct RBT_Printer_Stack *new_stack = malloc( sizeof(struct RBT_Printer_Stack) );
    if ( new_stack == NULL ) {
        RBT_ERROR_STOP( EXIT_FAILURE, "Cannot allocate more memory." );
        return NULL;
    }
    new_stack->buffer = calloc( sizeof(char), size );

    new_stack->size = size;
    new_stack->next_index = 0;

    return new_stack;
}

inline static void RBT_destroy_stack( struct RBT_Printer_Stack * stack ) {
    if ( stack != NULL ) {
        free(stack->buffer);
        free(stack);
    }
}

inline static void RBT_pretty_push( struct RBT_Printer_Stack * stack, char character ) {
    if ( stack->size > 0 && stack->next_index + 5 <= stack->size ) {
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index++ ] = character;
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index ] = '\0';
    }
}

inline static void RBT_pretty_pop( struct RBT_Printer_Stack * stack ) {
    if ( stack->size > 0 && stack->next_index > 0 ) {
        stack->buffer[ stack->next_index -= 4 ] = '\0';
    }
}

inline static void RBT_pretty_printer_helper( struct RBT_Node *node, struct RBT_Printer_Stack *stack ) {

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


/** ---- PUBLIC FUNCTIONS ---- **/


void RBT_pretty_printer(struct RBT_Tree *tree) {
    if ( tree == NULL ) {
        return;
    }
    struct RBT_Printer_Stack *stack = RBT_new_stack(2046);
    RBT_pretty_printer_helper(tree->root, stack);
    RBT_destroy_stack(stack);
}