
cmake_minimum_required(VERSION 3.10)
project(lookcpu CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
    lookcpu
    main.cpp
    core/cpu.cpp
    core/memory.cpp
    core/reading.cpp
    core/block.cpp
    core/network.cpp
    core/display.cpp
    core/board.cpp
    core/power.cpp
    core/os.cpp
)