
#include <stdio.h>
#include "RBPrinterStack.h"
#include "RBMacros.h"

struct RBT_Printer_Stack *RBT_new_stack( size_t size ) {
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

void RBT_destroy_stack( struct RBT_Printer_Stack * stack ) {
    if ( stack != NULL ) {
        free(stack->buffer);
        free(stack);
    }
}

void RBT_pretty_push( struct RBT_Printer_Stack * stack, char character ) {
    if ( stack->size > 0 && stack->next_index + 5 <= stack->size ) {
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index++ ] = character;
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index++ ] = ' ';
        stack->buffer[ stack->next_index ] = '\0';
    }
}

void RBT_pretty_pop( struct RBT_Printer_Stack * stack ) {
    if ( stack->size > 0 && stack->next_index > 0 ) {
        stack->buffer[ stack->next_index -= 4 ] = '\0';
    }
}
