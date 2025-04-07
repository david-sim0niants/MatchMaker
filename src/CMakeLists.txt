set(TARGET_NAME match_maker)

add_executable(${TARGET_NAME} main.cpp)
set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME match-maker
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
target_link_libraries(${TARGET_NAME} Qt6::Widgets)
