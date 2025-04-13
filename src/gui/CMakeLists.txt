set(TARGET_NAME match_maker_gui)

set(MOC_HEADERS mainwindow.h menubar.h)
list(TRANSFORM MOC_HEADERS PREPEND ${ROOT_INCLUDE_DIR}/gui/)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(SRCS mainwindow.cpp menubar.cpp ${MOC_HEADERS})

add_library(${TARGET_NAME} INTERFACE)
target_sources(${TARGET_NAME} INTERFACE ${SRCS})
target_include_directories(${TARGET_NAME} INTERFACE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} INTERFACE Qt6::Widgets)
target_compile_definitions(${TARGET_NAME} INTERFACE QT_DISABLE_DEPRECATED_BEFORE=0x050F00)
