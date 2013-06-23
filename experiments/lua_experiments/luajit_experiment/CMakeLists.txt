cmake_minimum_required (VERSION 2.6)
project(lua_experiment)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(src)

execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink
    /home/wrdn/Desktop/GameEngine2/experiments/lua_experiments/luajit_experiment/data
    /home/wrdn/Desktop/GameEngine2/experiments/lua_experiments/luajit_experiment/build/data)
