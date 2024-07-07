# File: TreeSitterXfileGenerate.cmake
# Author: Siddharth Mishra (admin@brightprogrammer.in)
# Copyright: Copyright (c) 2024, All Rights Reserved.
# Description:
#
# This file is a part of CrossFile
#
# This file contains CMake build system script to generate `tree-sitter-xfile` language
# and include the generated parser in CrossFile's build system. The script makes sure that                    
# the parser is generated inside the <Build>/Parser directory.                                                
#
# A new library is then imported and named as `TreeSitterXfileParserLib`. This requires importing             
# because the generated parser uses Makefile as it's build system.                                            
# --------------------------------------------------------------------------------------------------

# Required to include Makefile based projects
include(ExternalProject)

# Define the source and build directories
set(TREE_SITTER_XFILE_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/XfileParser)
set(TREE_SITTER_XFILE_GRAMMAR_JS ${CMAKE_CURRENT_SOURCE_DIR}/grammar.js)

# Create the build directory if it doesn't exist
file(MAKE_DIRECTORY ${TREE_SITTER_XFILE_BUILD_DIR})

# Run tree-sitter generate command during cmake configure step
# NOTE: This command assumes that tree-sitter-cli is already installed on host.
#       If not installed then install using cargo or other preferred methods provided
#       in official documentation.
execute_process(
    COMMAND tree-sitter generate "${TREE_SITTER_XFILE_GRAMMAR_JS}"
    WORKING_DIRECTORY ${TREE_SITTER_XFILE_BUILD_DIR}
    RESULT_VARIABLE TREE_SITTER_XFILE_GENERATE_RESULT
)

# If tree sitter generation result is not successful.
if(NOT TREE_SITTER_XFILE_GENERATE_RESULT EQUAL "0")
    message(FATAL_ERROR "tree-sitter-xfile parser generation failed.")
endif()

# Use ExternalProject to include the generated Makefile project
ExternalProject_Add(
    TreeSitterXfileParser
    PREFIX ${TREE_SITTER_XFILE_BUILD_DIR}
    SOURCE_DIR ${TREE_SITTER_XFILE_BUILD_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND tree-sitter build
    INSTALL_COMMAND ""
)

# Specify the generated library
add_library(TreeSitterXfileParserLib STATIC IMPORTED)
set_target_properties(TreeSitterXfileParserLib PROPERTIES
    IMPORTED_LOCATION ${TREE_SITTER_XFILE_BUILD_DIR}/path/to/libtree-sitter.a
)