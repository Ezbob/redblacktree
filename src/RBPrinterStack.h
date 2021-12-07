
#ifndef _HEADER_FILE_RBPrinterStack_20211207192617_
#define _HEADER_FILE_RBPrinterStack_20211207192617_

#include <stdlib.h>

// Linear bounded stack for printing purposes
struct RBT_Printer_Stack {
    size_t size;
    size_t next_index;
    char *buffer;
};

struct RBT_Printer_Stack *RBT_new_stack( size_t );

void RBT_destroy_stack( struct RBT_Printer_Stack * stack );
void RBT_pretty_push( struct RBT_Printer_Stack * stack, char character );
void RBT_pretty_pop( struct RBT_Printer_Stack * stack );

#endif