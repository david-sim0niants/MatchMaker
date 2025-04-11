set(TARGET_NAME match_maker_core)
set(SRCS
    user.cpp player.cpp timeline_bound.cpp timeline.cpp waiter.cpp match_arranger.cpp
    executable_game.cpp)

add_library(${TARGET_NAME} INTERFACE)
target_sources(${TARGET_NAME} INTERFACE ${SRCS})
target_include_directories(${TARGET_NAME} INTERFACE ${ROOT_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} INTERFACE match_maker_misc)
