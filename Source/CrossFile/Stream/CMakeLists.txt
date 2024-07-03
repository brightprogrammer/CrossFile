file(GLOB_RECURSE CrossFile_Stream_SRCS ${CMAKE_CURRENT_SOURCE_DIR} *.c)
add_library(xf_stream ${CrossFile_Stream_SRCS})
