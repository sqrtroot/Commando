cmake_minimum_required(VERSION 3.15)
project(CommandLib)

set(CMAKE_CXX_STANDARD 11)

option(COVERAGE "build with coverage enabled" NO)
if(COVERAGE)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
  set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
endif()

add_library(Commando
        include/Commando/ArgSpan.h
        include/Commando/Command.h
        include/Commando/CommandHandler.h
        include/Commando/Commando.h
        include/Commando/CommandStatus.h
        include/Commando/Utils/ToNumber.hpp
        src/CommandHandler.cpp)

target_include_directories(Commando PUBLIC include)
target_include_directories(Commando PUBLIC external/span-lite/include)
target_include_directories(Commando PUBLIC external/string-view-lite/include)

include(tests/CMakeLists.txt)

