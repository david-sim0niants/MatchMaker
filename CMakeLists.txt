cmake_minimum_required(VERSION 3.24)
project(MatchMaker CXX)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(ROOT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SANITIZE "none" CACHE STRING "Enable sanitizer: none, address, thread")

string(TOLOWER ${SANITIZE} SANITIZE)

if (NOT SANITIZE MATCHES "(none|address|thread)")
    message(FATAL_ERROR "Invalid value for SANITIZE - " ${SANITIZE})
elseif (SANITIZE STREQUAL "none")
    message(STATUS "Not using any sanitizer")
else ()
    message(STATUS "Using ${SANITIZE} sanitizer")
endif ()


add_subdirectory(src)

option(BUILD_TESTS "Builds tests?" OFF)

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()
