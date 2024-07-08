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

set(TREE_SITTER_SOURCE_DIR ${CMAKE_CURRENT_BINARY_DIR}/Dependencies/tree-sitter)
set(TREE_SITTER_BUILD_DIR ${TREE_SITTER_SOURCE_DIR}/src/tree-sitter-gh)

set(TREE_SITTER_CLI_EXE ${TREE_SITTER_BUILD_DIR}/target/release/tree-sitter CACHE PATH "tree-sitter cli executable")
set(TREE_SITTER_INCLUDE_DIR ${TREE_SITTER_BUILD_DIR}/lib/include CACHE PATH "Include directory for Tree-sitter Xfile parser")
set(TREE_SITTER_LIBRARY_DIR ${TREE_SITTER_XFILE_BUILD_DIR} CACHE PATH "Library directory for Tree-sitter Xfile parser")
set(TREE_SITTER_LIBRARIES tree-sitter CACHE STRING "Libraries to link with for Tree-sitter Xfile parser")
set(TREE_SITTER_DEPENDENCIES BuildTreeSitter CACHE STRING "Dependencies to depend on if tree-sitter is to be used")

if(TARGET tree-sitter-gh)
    return()
endif()

# Allows us to download remote projects and include them within our project
include(ExternalProject)

message(STATUS "Making tree-sitter available")

# Fetch and build the official tree-sitter package
ExternalProject_Add(
    tree-sitter-gh
    PREFIX ${TREE_SITTER_SOURCE_DIR}
    GIT_REPOSITORY https://github.com/tree-sitter/tree-sitter
    GIT_SUBMODULES_RECURSE 1
    GIT_PROGRESS 1 
    LOG_DOWNLOAD 1
    UPDATE_DISCONNECTED 1
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make -C ${TREE_SITTER_BUILD_DIR} && cargo build --release --jobs 4 --manifest-path ${TREE_SITTER_BUILD_DIR}/Cargo.toml
    INSTALL_COMMAND ""
)

add_custom_target(BuildTreeSitter ALL
    DEPENDS tree-sitter-gh
)

message(STATUS "Making tree-sitter available - done")