cmake_minimum_required(VERSION 3.15)
project(CommandLib)

set(CMAKE_CXX_STANDARD 11)

if(MSVC)
	add_compile_options(/W4)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

option(COVERAGE "build with coverage enabled" NO)
if(COVERAGE)
  if(MSVC)
    message(SEND_ERROR "MSVC doesnt support coverage flags")
  else()
    set(CMAKE_BUILD_TYPE Debug)
    add_compile_options("-g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
  endif() 
 endif()

 if (WIN32)
  set(CP COPY)
else()
  set(CP cp)
endif()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/include/nonstd")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/external/span-lite/include/nonstd/span.hpp" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/include/nonstd/span.hpp")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/external/string-view-lite/include/nonstd/string_view.hpp" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/include/nonstd/string_view.hpp")
file(COPY "${CMAKE_CURRENT_LIST_DIR}/external/optional-lite/include/nonstd/optional.hpp" DESTINATION "${CMAKE_CURRENT_LIST_DIR}/include/nonstd/optional.hpp")

add_library(Commando
        include/Commando/ArgSpan.h
        include/Commando/Command.h
        include/Commando/CommandHandler.h
        include/Commando/Commando.h
        include/Commando/CommandStatus.h
        include/Commando/Utils/ToNumber.hpp
        include/nonstd/span.hpp
        include/nonstd/string_view.hpp
        include/nonstd/optional.hpp
        src/CommandHandler.cpp)

target_include_directories(Commando PUBLIC "include")
target_include_directories(Commando PUBLIC "external/span-lite/include")
target_include_directories(Commando PUBLIC "external/string-view-lite/include")
target_include_directories(Commando PUBLIC "external/optional-lite/include")

include(tests/CMakeLists.txt)

