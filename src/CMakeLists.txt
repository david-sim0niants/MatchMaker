set(TARGET_NAME match_maker)

set(MOC_HEADERS mainwindow.h)
list(TRANSFORM MOC_HEADERS PREPEND ${ROOT_INCLUDE_DIR}/)

add_executable(${TARGET_NAME} main.cpp mainwindow.cpp mainwindow.ui ${MOC_HEADERS})
set_target_properties(${TARGET_NAME} PROPERTIES
    OUTPUT_NAME match-maker
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
target_include_directories(${TARGET_NAME} PUBLIC ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} Qt6::Widgets)
