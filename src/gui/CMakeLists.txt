set(TARGET_NAME match_maker_gui)

set(MOC_HEADERS gui/mainwindow.h)
list(TRANSFORM MOC_HEADERS PREPEND ${ROOT_INCLUDE_DIR}/)

set(SRCS mainwindow.cpp mainwindow.ui ${MOC_HEADERS})

add_library(${TARGET_NAME} INTERFACE)
target_sources(${TARGET_NAME} INTERFACE ${SRCS})
target_include_directories(${TARGET_NAME} INTERFACE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} INTERFACE Qt6::Widgets)
