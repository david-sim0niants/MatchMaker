set(TARGET_NAME match_maker_app)
set(SRCS app.cpp mediator.cpp console_user_rating_observer.cpp console_player_observer.cpp)

add_library(${TARGET_NAME} INTERFACE)
target_sources(${TARGET_NAME} INTERFACE ${SRCS})
target_include_directories(${TARGET_NAME} INTERFACE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} INTERFACE match_maker_core match_maker_gui)
