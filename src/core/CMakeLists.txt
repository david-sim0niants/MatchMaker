set(TARGET_NAME match_maker_core)
set(SRCS main_activity.cpp resource_manager.cpp
    player.cpp timeline.cpp cv_waiter.cpp timeline_object.cpp rating_map.cpp
    match.cpp match_arranger.cpp match_mediator.cpp match_engine.cpp
    user.cpp user_registry.cpp executable_game.cpp game_registry.cpp)

add_library(${TARGET_NAME} INTERFACE)
target_sources(${TARGET_NAME} INTERFACE ${SRCS})
target_include_directories(${TARGET_NAME} INTERFACE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} INTERFACE match_maker_misc)
