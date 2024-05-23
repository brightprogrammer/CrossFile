file(GLOB_RECURSE CrossFile_SRCS ${CMAKE_CURRENT_SOURCE_DIR} *.c)
add_library(crossfile ${CrossFile_SRCS})
