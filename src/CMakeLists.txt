set(TARGET_NAME match_maker)

add_executable(${TARGET_NAME} main.cpp)

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME match-maker
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

target_include_directories(${TARGET_NAME} PRIVATE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE match_maker_core match_maker_misc match_maker_gui)

add_subdirectory(core)
add_subdirectory(misc)
add_subdirectory(gui)
add_subdirectory(games)
