#include "test/test_functions.h"
#include "test/cutest.h"

TEST_LIST = {
       { "inserting into tree", RBT_test_insert },
       { "finding nodes in tree", RBT_test_find },
       { "getting minimum and maximum from tree", RBT_test_min_max },
       { "getting minimum and maximum from empty tree", RBT_test_min_max_null },
       { "deleting nodes in tree", RBT_test_remove },
       { "zero-sized allocation of stack", RBT_test_creation_deletion_stack_fail },
       { "creation and deletion of print stack", RBT_test_creation_deletion_stack },
       { "legal stack push", RBT_test_push_stack },
       { "stack push overflow try", RBT_test_push_stack_fail },
       { 0 }
};