set(BUILD_SHARED_LIBS OFF)

# disable gtest installation
set(INSTALL_GTEST OFF)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(TEST_TARGETS core/player core/timeline core/match_arranger)
set(TEST_SRC_FILES ${TEST_TARGETS})
list(TRANSFORM TEST_SRC_FILES APPEND .cpp)
list(APPEND TEST_SRC_FILES test_config.cpp)

set(TARGET_NAME test_all)
add_executable(${TARGET_NAME} ${TEST_SRC_FILES})
target_include_directories(${TARGET_NAME} PRIVATE ${TEST_INCLUDE_DIR})
target_link_libraries(${TARGET_NAME} PRIVATE match_maker_core gtest_main gmock)
