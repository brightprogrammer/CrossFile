add_subdirectory(CrossFile)

add_executable(main Main.c)
target_include_directories(main PUBLIC ${TREE_SITTER_XFILE_INCLUDE_DIR})
target_link_directories(main PUBLIC ${TREE_SITTER_XFILE_LIBRARY_DIR})
target_link_libraries(main xf_stream ${TREE_SITTER_XFILE_LIBRARIES})

