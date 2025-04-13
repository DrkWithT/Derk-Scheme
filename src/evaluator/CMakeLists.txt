add_library(ds_evaluator "")
target_include_directories(ds_frontend PUBLIC ${DERK_SCHEME_INC_DIR})
target_sources(ds_evaluator PRIVATE values.cpp PRIVATE environment.cpp)
