
#ifndef _HEADER_FILE_RBMacros_20211207192450_
#define _HEADER_FILE_RBMacros_20211207192450_

#define RBT_ERROR(message) fprintf(stderr, "Error: %s\n", message)
#define RBT_ERROR_STOP(code, message)  do { fprintf(stderr, "Error: %s\n", message); exit(code); } while ( 0 )

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

#endif