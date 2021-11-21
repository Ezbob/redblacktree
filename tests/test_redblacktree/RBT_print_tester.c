#include "RBTree/test/pub_cutest.h"
#include "RBTree/test/test_functions.h"

void RBT_test_creation_deletion_stack() {
    size_t size = 20;
    struct RBT_STACK *stack = RBT_new_stack( size );

    TEST_CHECK( stack != NULL );

    TEST_CHECK( stack->size == size );

    TEST_CHECK( stack->next_index == 0 );

    RBT_destroy_stack(stack);
}

void RBT_test_creation_deletion_stack_fail() {
    size_t size = 0;
    struct RBT_STACK *stack = RBT_new_stack( size );

    TEST_CHECK( stack == NULL );

    RBT_destroy_stack(stack);
}

void RBT_test_push_stack() {
    size_t size = 10;
    struct RBT_STACK *stack = RBT_new_stack( size );

    TEST_CHECK( stack != NULL );

    char push_char = 'l';

    RBT_pretty_push( stack, push_char );

    TEST_CHECK( stack->next_index == 4 );

    char c;
    int i = 0;
    for ( ; ( c = stack->buffer[i] ) != '\0'; ++i ) {
        if ( i == 1  ) {
            TEST_CHECK_( c == push_char, "stack push test: character %c found %c expected at index %i ", c, push_char, i );
        } else {
            TEST_CHECK_( c == ' ', "stack push test: character %c found %c expected at index %i ", c, ' ', i );
        }
    }

    TEST_CHECK( i == 4 );

    RBT_destroy_stack(stack);
}

void RBT_test_push_stack_fail() {
    size_t size = 2;
    struct RBT_STACK *stack = RBT_new_stack( size );

    TEST_CHECK( stack != NULL );

    size_t old_index = stack->next_index;

    char push_char = 'l';

    RBT_pretty_push( stack, push_char );

    TEST_CHECK_( stack->next_index == old_index, "stack push test: got index: %i expected: %i", stack->next_index, old_index );

    RBT_destroy_stack(stack);
}

