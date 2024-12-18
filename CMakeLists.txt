# Minimum CMake version required
cmake_minimum_required(VERSION 3.28)

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


# include library for managing local chess board state (headers only)
# not a CMakeProject, see: 
# https://cmake.org/cmake/help/latest/module/ExternalProject.html#configure-step-options
FetchContent_Declare(
    chess
    GIT_REPOSITORY https://github.com/Disservin/chess-library
    GIT_TAG        7114fd5b26e913812055658f862971c4cd85f73b  # current master
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    SYSTEM ON
    EXCLUDE_FROM_ALL ON
    CONFIGURE_COMMAND ""    # disable configure and build (only headers to include)
    BUILD_COMMAND ""        # not a CMake project
)
 
# include library for interaction with the stockfish process over uci
FetchContent_Declare(
    uciadapter
    GIT_REPOSITORY https://gitlab.com/manzerbredes/uciadapter.git
    GIT_TAG        4c73c57b9ee0c12a6dc423b663753f4b812a8b10  # current master
    GIT_SHALLOW ON
    GIT_PROGRESS ON
    SYSTEM ON
    EXCLUDE_FROM_ALL ON
)

# make all library deps linkable
FetchContent_MakeAvailable(fmt chess uciadapter)

# add the chess header manully
# because it's not a CMake project
add_library(chess INTERFACE)
target_include_directories(chess SYSTEM INTERFACE "${chess_SOURCE_DIR}/include")

# Find the Ogre library installed on system through pacman
# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

add_executable(${PROJECT_NAME} src/main.cpp src/app.cpp src/overlay.cpp src/engine.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE uciadapter chess fmt::fmt OgreBites)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

add_subdirectory(examples/)
