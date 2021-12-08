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

set(redblack_library_sources
  ${CMAKE_CURRENT_LIST_DIR}/src/RBTree.c
  ${CMAKE_CURRENT_LIST_DIR}/src/RBTreePrinter.c
)

add_library(redblacktree SHARED "")
target_sources(redblacktree
  PRIVATE
    ${redblack_library_sources}
)
target_include_directories(redblacktree
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/
)


add_library(redblacktree_static STATIC "")
target_sources(redblacktree_static
  PRIVATE
    ${redblack_library_sources}
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
    ${CMAKE_CURRENT_LIST_DIR}/tests/Main.c
    ${CMAKE_CURRENT_LIST_DIR}/tests/RBTreeTest.c
)
target_link_libraries(redblacktree_test
  PRIVATE
    redblacktree_static
)

target_include_directories(redblacktree_test
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/
)
