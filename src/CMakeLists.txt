set(TARGET_NAME match_maker)

set(MOC_HEADERS mainwindow.h)
list(TRANSFORM MOC_HEADERS PREPEND ${ROOT_INCLUDE_DIR}/)

set(UI_SRCS mainwindow.cpp mainwindow.ui ${MOC_HEADERS})

add_executable(${TARGET_NAME} main.cpp ${UI_SRCS})

set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME match-maker
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

target_include_directories(${TARGET_NAME} PRIVATE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE match_maker_core match_maker_misc Qt6::Widgets)

add_subdirectory(core)
add_subdirectory(misc)
