set(TARGET_NAME match_maker_gui)

set(COMPONENTS
    mainwindow menubar dashboard dashboard_model
    user_list user_list_model user_list_delegate user_list_filter)
set(HEADER_ONLY_COMPONENTS "")

set(SRCS ${COMPONENTS})
list(TRANSFORM SRCS APPEND .cpp)

set(HEADERS ${COMPONENTS} ${HEADER_ONLY_COMPONENTS})
list(TRANSFORM HEADERS PREPEND ${ROOT_INCLUDE_DIR}/gui/)
list(TRANSFORM HEADERS APPEND .h)

add_library(${TARGET_NAME} INTERFACE)
target_sources(${TARGET_NAME} INTERFACE ${SRCS} ${HEADERS})
target_include_directories(${TARGET_NAME} INTERFACE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} INTERFACE Qt6::Widgets)
target_compile_definitions(${TARGET_NAME} INTERFACE QT_DISABLE_DEPRECATED_BEFORE=0x050F00)
