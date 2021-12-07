#include "test_functions.h"
#include "cutest/cutest.h"

TEST_LIST = {
       { "inserting into tree", RBT_test_insert },
       { "finding nodes in tree", RBT_test_find },
       { "getting minimum and maximum from tree", RBT_test_min_max },
       { "getting minimum and maximum from empty tree", RBT_test_min_max_null },
       { "deleting nodes in tree", RBT_test_remove },
       { "static allocation", RBT_test_static_allocate_nodes},
       { 0 }
};
