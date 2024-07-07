# File: TreeSitterMakeAvailable.cmake
# Author: Siddharth Mishra (admin@brightprogrammer.in)
# Copyright: Copyright (c) 2024, All Rights Reserved.
# Description:
#
# This file is a part of CrossFile
#
# This CMake script when included will automatically download the offical
# tree-sitter project and build it to make it available for use throughout
# the project.
# --------------------------------------------------------------------------------------------------

# Allows us to download remote projects and include them within our project
include(ExternalProject)

set(TREE_SITTER_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/tree-sitter)
set(TREE_SITTER_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/tree-sitter/src/tree-sitter-gh)

message(STATUS "Making tree-sitter available")

# Fetch and build the official tree-sitter package
ExternalProject_Add(
    tree-sitter-gh
    PREFIX ${TREE_SITTER_SOURCE_DIR}
    GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter
    GIT_SUBMODULES_RECURSE 1
    GIT_PROGRESS 1 
    LOG_DOWNLOAD 1
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make -C ${TREE_SITTER_BUILD_DIR}
    INSTALL_COMMAND ""
)

message(STATUS "Making tree-sitter available - done")

set(TREE_SITTER_INCLUDE_DIR ${TREE_SITTER_BUILD_DIR}/lib/include CACHE PATH "Include directory for Tree-sitter Xfile parser")
set(TREE_SITTER_LIBRARY_DIR ${TREE_SITTER_XFILE_BUILD_DIR} CACHE PATH "Library directory for Tree-sitter Xfile parser")
set(TREE_SITTER_LIBRARIES tree-sitter CACHE STRING "Libraries to link with for Tree-sitter Xfile parser")