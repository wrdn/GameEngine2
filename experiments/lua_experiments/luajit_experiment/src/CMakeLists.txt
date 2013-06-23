cmake_minimum_required (VERSION 2.6)

# root of the game engine tree
set(GE2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../../../..")

include_directories (${GE2_ROOT}/external/common/bitsquid_foundation/)
include_directories (${GE2_ROOT}/external/common/luajit-2.0/src/)
link_directories (${GE2_ROOT}/external/common/luajit-2.0/lib/)

add_executable(luajit_experiment main.cpp)
target_link_libraries(luajit_experiment luajit)
