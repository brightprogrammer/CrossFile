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
# A new library is then imported and named as `TreeSitterXfileLib`. This requires importing             
# because the generated parser uses Makefile as it's build system.                                            
# --------------------------------------------------------------------------------------------------

# Make sure tree-sitter is available locally
include(${PROJECT_SOURCE_DIR}/CMake/TreeSitterMakeAvailable.cmake)

# Required to include Makefile based projects
include(ExternalProject)

message(STATUS "Generating tree-sitter-xfile parser")

# Define the source and build directories
set(TREE_SITTER_XFILE_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/Generated/Xfile)
set(TREE_SITTER_XFILE_GRAMMAR_JS ${CMAKE_CURRENT_SOURCE_DIR}/grammar.js)
set(TREE_SITTER_XFILE_BUILD_GRAMMAR_JS ${TREE_SITTER_XFILE_BUILD_DIR}/grammar.js)

# Create the build directory if it doesn't exist
file(MAKE_DIRECTORY ${TREE_SITTER_XFILE_BUILD_DIR})

# Copy grammar.js to parser build directroy (Build/Xfile)
file(COPY ${TREE_SITTER_XFILE_GRAMMR_JS} DESTINATION ${TREE_SITTER_XFILE_BUILD_GRAMMAR_JS})

# Run tree-sitter generate command during cmake configure step
# NOTE: This command assumes that tree-sitter-cli is already installed on host.
#       If not installed then install using cargo or other preferred methods provided
#       in official documentation.
execute_process(
    COMMAND tree-sitter generate
    WORKING_DIRECTORY ${TREE_SITTER_XFILE_BUILD_DIR}
    RESULT_VARIABLE TREE_SITTER_XFILE_GENERATE_RESULT
)

# If tree sitter generation result is not successful.
if(NOT TREE_SITTER_XFILE_GENERATE_RESULT EQUAL "0")
    message(FATAL_ERROR "tree-sitter-xfile parser generation failed.")
else()
    message(STATUS "Generating tree-sitter-xfile parser - done")
endif()

# Use ExternalProject to include the generated Makefile project
ExternalProject_Add(
    TreeSitterXfile
    PREFIX ${TREE_SITTER_XFILE_BUILD_DIR}
    SOURCE_DIR ${TREE_SITTER_XFILE_BUILD_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make -C ${TREE_SITTER_XFILE_BUILD_DIR}
    INSTALL_COMMAND ""
)

set(TREE_SITTER_XFILE_INCLUDE_DIR ${TREE_SITTER_INCLUDE_DIR} ${TREE_SITTER_XFILE_BUILD_DIR}/bindings/c
    CACHE PATH "Include directory for Tree-sitter Xfile parser")
set(TREE_SITTER_XFILE_LIBRARY_DIR ${TREE_SITTER_LIBRARY_DIR} ${TREE_SITTER_XFILE_BUILD_DIR}
    CACHE PATH "Library directory for Tree-sitter Xfile parser")
set(TREE_SITTER_XFILE_LIBRARIES ${TREE_SITTER_LIBRARIES} tree-sitter-xfile
    CACHE STRING "Libraries to link with for Tree-sitter Xfile parser")