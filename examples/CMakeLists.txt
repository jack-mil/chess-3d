# chess example
# https://disservin.github.io/chess-library/pages/examples.html
add_executable(chess_example src/chess.cpp)
target_link_libraries(chess_example PRIVATE chess)

# ogre 3d scene basic examples
# https://github.com/OGRECave/ogre/blob/master/Samples/Tutorials/Bootstrap.cpp

add_executable(ogre_bootstrap src/ogre_bootstrap.cpp)
target_link_libraries(ogre_bootstrap OgreBites)
target_compile_features(ogre_bootstrap PRIVATE cxx_std_14)

add_executable(1_FirstScene src/ogre_tutorial1.cpp)
target_link_libraries(1_FirstScene OgreBites)

# I added the camera movement and overlays from the Python example
add_executable(2_CamerasAndShadows src/ogre_tutorial2.cpp)
target_link_libraries(2_CamerasAndShadows OgreBites)
