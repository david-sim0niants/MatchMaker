function (make_tic_tac_toe CONFIG_SIZE)
    set(TARGET_NAME tic_tac_toe_${CONFIG_SIZE}x${CONFIG_SIZE})

    add_executable(${TARGET_NAME} tic_tac_toe.cpp)
    target_compile_definitions(${TARGET_NAME} PRIVATE -DCONFIG_SIZE=${CONFIG_SIZE})

    set_target_properties(${TARGET_NAME} PROPERTIES
        OUTPUT_NAME tic-tac-toe-${CONFIG_SIZE}x${CONFIG_SIZE}
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

endfunction ()

make_tic_tac_toe(3)
make_tic_tac_toe(4)
make_tic_tac_toe(5)

set(TARGET_NAME rock_paper_scissors)
add_executable(${TARGET_NAME} rock_paper_scissors.cpp)
set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME rock-paper-scissors
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
