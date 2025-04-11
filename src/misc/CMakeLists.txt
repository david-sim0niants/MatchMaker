set(TARGET_NAME match_maker_misc)
set(SRCS subprocess.cpp subprocess_reaper.cpp)

add_library(${TARGET_NAME} INTERFACE)
target_sources(${TARGET_NAME} INTERFACE ${SRCS})
target_include_directories(${TARGET_NAME} INTERFACE ${ROOT_INCLUDE_DIR})
