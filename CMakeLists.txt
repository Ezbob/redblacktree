cmake_minimum_required(VERSION 3.15)

project(redblacktree
  VERSION 1.0.0
  DESCRIPTION "A red-black tree written in C based on the Cormen et. al. book"
  LANGUAGES C
)

set(CMAKE_C_STANDARD 99)

# ----
# Library
# ----

add_library(redblacktree SHARED "")
target_sources(redblacktree
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/RBTree.c
)
target_include_directories(redblacktree
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/
)


add_library(redblacktree_static STATIC "")
target_sources(redblacktree_static
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/redblacktree/RBTree.c
)
target_include_directories(redblacktree_static
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/
)

set_target_properties(redblacktree redblacktree_static
  PROPERTIES
    OUTPUT_NAME libredblacktree
)

# ----
# Tests
# ----

add_executable(redblacktree_test "")
target_sources(redblacktree_test
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/tests/main_test.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/tree_tester.c
)
target_link_libraries(redblacktree_test
  PUBLIC
    redblacktree_static
)