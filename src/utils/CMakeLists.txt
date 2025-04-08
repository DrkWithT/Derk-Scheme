add_library(ds_utils "")
target_include_directories(ds_utils PUBLIC ${DERK_SCHEME_INC_DIR})
target_sources(ds_utils PRIVATE file_io.cpp)
