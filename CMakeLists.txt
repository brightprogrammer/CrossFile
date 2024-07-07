cmake_minimum_required(VERSION 3.5)

project(CrossFile VERSION 0 LANGUAGES C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -fsanitize=address")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

# This is where I keep all my CMake helper scripts
set(CROSSFILE_CMAKE_SCRIPTS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/CMake)
include(${CROSSFILE_CMAKE_SCRIPTS_DIR}/TreeSitterXfileGenerate.cmake)

include_directories(Include)
add_subdirectory(Source)
