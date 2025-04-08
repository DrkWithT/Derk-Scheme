add_library(ds_frontend)
target_include_directories(ds_frontend PUBLIC ${DERK_SCHEME_INC_DIR})
target_sources(ds_frontend PRIVATE token.cpp PRIVATE lexer.cpp)
