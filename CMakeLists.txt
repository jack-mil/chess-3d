# Minimum CMake version required
cmake_minimum_required(VERSION 3.26)

project(
    chess3d
    VERSION 0.1.0
    DESCRIPTION "Render a 3D chess board against the Stockfish chess engine"
    LANGUAGES CXX
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include(FetchContent)

# include fmt (from c++20)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
    GIT_TAG        0c9fce2ffefecfdce794e1859584e25877b7b592  # 11.0.2
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    SYSTEM ON
    EXCLUDE_FROM_ALL ON
)

# include reproc for process handling
set(REPROC++ ON)
FetchContent_Declare(
    reproc++
    GIT_REPOSITORY https://github.com/DaanDeMeyer/reproc
    GIT_TAG        3179928ae7b085e41dfb846d987519fa7c12ffb3  # 14.2.5
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    SYSTEM ON
    EXCLUDE_FROM_ALL ON
)

# include UCI chess moveset library (headers only)
FetchContent_Declare(
    chess
    GIT_REPOSITORY https://github.com/Disservin/chess-library
    GIT_TAG        7114fd5b26e913812055658f862971c4cd85f73b  # current master
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    SYSTEM ON
    EXCLUDE_FROM_ALL ON
    CONFIGURE_COMMAND ""    # disable configure and build (only headers to include)
    BUILD_COMMAND ""
)

# make all source deps findable
FetchContent_MakeAvailable(fmt reproc++ chess)

# add the chess header
add_library(chess INTERFACE)
target_include_directories(chess SYSTEM INTERFACE "${chess_SOURCE_DIR}/include")

# Add the header-only cli arg library
add_library(clipp INTERFACE)
# SYSTEM keyword prevents warnings from external libraries
target_include_directories(clipp SYSTEM INTERFACE "external/")


add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE chess clipp fmt::fmt reproc++)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

add_subdirectory(examples/)
