
#include "RBTree/RBTreePrinter.h"
#include "RBPrinterStack.h"
#include "RBMacros.h"
#include <stdio.h>


/** ---- PRIVATE FUNCTIONS ---- **/


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