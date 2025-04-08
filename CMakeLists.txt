cmake_minimum_required(VERSION 3.28)

project(derkscheme VERSION 0.1.0 LANGUAGES CXX)

set(DERK_SCHEME_INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/includes)
set(DERK_SCHEME_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(DERK_SCHEME_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

if (DEBUG_MODE)
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -Og)
else ()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -O3)
endif ()

add_subdirectory(${DERK_SCHEME_SRC_DIR})
# enable_testing()
# add_subdirectory(tests)
