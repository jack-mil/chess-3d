# chess example
# https://disservin.github.io/chess-library/pages/examples.html
add_executable(chess_example src/chess.cpp)
target_link_libraries(chess_example PRIVATE chess)

# external process examples
# https://github.com/DaanDeMeyer/reproc/blob/main/reproc++/examples/forward.cpp
add_executable(reproc_example src/reproc_forward.cpp)
target_link_libraries(reproc_example PRIVATE fmt::fmt reproc++)
target_compile_features(reproc_example PRIVATE cxx_std_17)

# ogre 3d scene basic examples
# https://github.com/OGRECave/ogre/blob/master/Samples/Tutorials/Bootstrap.cpp

## [discover_ogre]
# The COMPONENTS part checks that OGRE was built the way we need it
# The CONFIG flag makes sure we get OGRE instead of OGRE-next
find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)

# add the source files as usual
add_executable(ogre_bootstrap src/ogre_bootstrap.cpp)
# this also sets the includes and pulls third party dependencies
target_link_libraries(ogre_bootstrap OgreBites)

target_compile_features(ogre_bootstrap PRIVATE cxx_std_14)
## [discover_ogre]

add_executable(1_FirstScene src/ogre_tutorial1.cpp)
target_link_libraries(1_FirstScene OgreBites)

add_executable(2_CamerasAndShadows src/ogre_tutorial2.cpp)
target_link_libraries(2_CamerasAndShadows OgreBites)
