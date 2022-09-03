add_executable(unit_test)

target_link_libraries(unit_test PRIVATE decimal3)

target_sources(unit_test PRIVATE
    main.cpp
    harness.cpp
    harness_extended.cpp
)

add_custom_target(run_test
    COMMAND unit_test
    DEPENDS unit_test
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)