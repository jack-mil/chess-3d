![screenshot_1](https://github.com/user-attachments/assets/7c224bdd-7974-451d-acdc-516e3b919753)

Quick and dirty 3D scene rendering and `stockfish` integration required project.

`$ cmake -B build`
`$ cmake --build build --target chess3d`

Output is `./build/chess3d`.

Runtime requires the `stockfish` chess engine binary available on $PATH.

Tested on Arch Linux with the `ogre` package. Other dependencies are cloned and compiled by cmake at build-time.

Targets in the `examples/` subfolder were just me getting used to the various libraries I was going to use.

The CMakePresets.json are options specific to my machine and toolchain (Ninja Multi-Config, Mold linker, ccache, parallel jobs, etc.)
