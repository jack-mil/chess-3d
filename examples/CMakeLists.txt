add_executable(chess_example src/chess.cpp)
target_link_libraries(chess_example PRIVATE chess)

add_executable(reproc_example src/reproc_forward.cpp)
target_link_libraries(reproc_example PRIVATE fmt::fmt reproc++)
target_compile_features(reproc_example PRIVATE cxx_std_17)
