add_executable(tests
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/CommandHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Utils.cpp)
target_include_directories(tests PUBLIC ../external/Catch2/single_include ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(tests Commando)