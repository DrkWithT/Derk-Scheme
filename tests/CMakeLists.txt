add_executable(test_lexer)
target_link_directories(test_lexer PRIVATE ${DERK_SCHEME_LIB_DIR})
target_include_directories(test_lexer PUBLIC ${DERK_SCHEME_INC_DIR})
target_sources(test_lexer PRIVATE test_lexer.cpp)
add_test(NAME test_lexer COMMAND "$<TARGET_FILE:test_lexer>")
target_link_libraries(test_lexer PRIVATE ds_frontend)

# todo
