cmake_minimum_required(VERSION 3.24)
project(MatchMaker CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(ROOT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(src)

option(BUILD_TESTS "Builds tests?" OFF)

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()
