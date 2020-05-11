add_executable(tests
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/CommandHandler.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/Utils.cpp)

target_include_directories(tests PUBLIC ${CMAKE_CURRENT_LIST_DIR}/../external/Catch2/single_include ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(tests Commando)

if(COVERAGE)

  add_custom_command(OUTPUT coverage.info
    COMMAND lcov --directory ${CMAKE_BINARY_DIR} --zerocounters
    COMMAND ${CMAKE_BINARY_DIR}/tests
    COMMAND lcov -c --directory ${CMAKE_BINARY_DIR} -o coverage.info --include '*Commando*' --exclude '*nonstd*' --exclude '*test*'
    DEPENDS tests)

  add_custom_target(coverage_summary
    DEPENDS coverage.info
    COMMAND lcov --summary ${CMAKE_BINARY_DIR}/coverage.info)

  add_custom_target(coverage_html
    DEPENDS coverage.info
    COMMAND genhtml --highlight --output-directory ${CMAKE_BINARY_DIR}/coverage_html ${CMAKE_BINARY_DIR}/coverage.info)
  set_target_properties(coverage_html PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_BINARY_DIR}/coverage_html/)

endif()

