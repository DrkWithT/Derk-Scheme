add_library(ds_syntax)
target_include_directories(ds_syntax PUBLIC ${DERK_SCHEME_INC_DIR})
target_sources(ds_syntax PRIVATE exprs.cpp)
