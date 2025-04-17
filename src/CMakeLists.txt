set(TARGET_NAME match_maker)

add_executable(${TARGET_NAME} main.cpp)

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME match-maker
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

target_include_directories(${TARGET_NAME} PRIVATE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE match_maker_app)

if (NOT SANITIZE STREQUAL "none")
    target_compile_options(${TARGET_NAME} PUBLIC -fsanitize=${SANITIZE})
    target_link_options(${TARGET_NAME} PUBLIC -fsanitize=${SANITIZE})
endif ()

add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(games)
add_subdirectory(gui)
add_subdirectory(misc)
