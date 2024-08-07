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

include(TreeSitterMakeAvailable)
include(ExternalProject)

if(TARGET GenerateTreeSitterXfileParser)
    return()
endif()

message(STATUS "Generating tree-sitter-xfile parser")

# Define the source and build directories
set(TREE_SITTER_XFILE_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/Generated/Xfile)
set(TREE_SITTER_XFILE_GRAMMAR_JS ${CMAKE_CURRENT_SOURCE_DIR}/grammar.js)

set(TREE_SITTER_XFILE_INCLUDE_DIR ${TREE_SITTER_INCLUDE_DIR} ${TREE_SITTER_XFILE_BUILD_DIR}/bindings/c
    CACHE PATH "Include directory for Tree-sitter Xfile parser")
set(TREE_SITTER_XFILE_LIBRARY_DIR ${TREE_SITTER_LIBRARY_DIR} ${TREE_SITTER_XFILE_BUILD_DIR}
    CACHE PATH "Library directory for Tree-sitter Xfile parser")
set(TREE_SITTER_XFILE_LIBRARIES ${TREE_SITTER_LIBRARIES} tree-sitter-xfile
    CACHE STRING "Libraries to link with for Tree-sitter Xfile parser")
set(TREE_SITTER_XFILE_DEPENDENCIES GenerateTreeSitterXfileParser
    CACHE STRING "Custom targets on which other targets that use tree-sitter-xfile must depend on")

# Create the build directory if it doesn't exist
file(MAKE_DIRECTORY ${TREE_SITTER_XFILE_BUILD_DIR})

# Copy grammar.js to parser build directroy (Build/Xfile)
file(COPY ${TREE_SITTER_XFILE_GRAMMAR_JS} DESTINATION ${TREE_SITTER_XFILE_BUILD_DIR})

# Run tree-sitter generate command during cmake configure step
add_custom_command(
    OUTPUT ${TREE_SITTER_XFILE_BUILD_DIR}/tree-sitter-xfile-parser-gen-output
    COMMAND ${TREE_SITTER_CLI_EXE} generate
    WORKING_DIRECTORY ${TREE_SITTER_XFILE_BUILD_DIR}
    DEPENDS ${TREE_SITTER_DEPENDENCIES}
    COMMENT "Generating tree-sitter-xfile parser..."
)

# Add a custom target for the parser generation step
add_custom_target(GenerateTreeSitterXfileParser ALL
    DEPENDS ${TREE_SITTER_XFILE_BUILD_DIR}/tree-sitter-xfile-parser-gen-output
)

# Directories/Files to be cleaned during clean operation
set_target_properties(
    GenerateTreeSitterXfileParser PROPERTIES ADDITIONAL_CLEAN_FILES
    ${TREE_SITTER_XFILE_BUILD_DIR}
)

# Use ExternalProject to include the generated Makefile project
ExternalProject_Add(
    TreeSitterXfile
    PREFIX ${TREE_SITTER_XFILE_BUILD_DIR}
    SOURCE_DIR ${TREE_SITTER_XFILE_BUILD_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND make -C ${TREE_SITTER_XFILE_BUILD_DIR}
    INSTALL_COMMAND ""
    DEPENDS GenerateTreeSitterXfileParser
)

# Directories/Files to be cleaned during clean operation
set_target_properties(
    GenerateTreeSitterXfileParser PROPERTIES ADDITIONAL_CLEAN_FILES
    ${TREE_SITTER_XFILE_BUILD_DIR}/test/corpus
)

# Add the test to CTest
add_test(NAME TreeSitterXfileTest
    COMMAND "${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Test/XfileParser ${TREE_SITTER_XFILE_BUILD_DIR}/test/corpus && ${TREE_SITTER_CLI_EXE} test"
    WORKING_DIRECTORY ${TREE_SITTER_XFILE_BUILD_DIR}
)
