# redblacktree

A red black tree written in C. This is my implementation of the classic red-black tree found in the Cormen et. al. textbook.

## How-to build

This project uses CMake to build the libraries and tests associated with this project.

To build everything, the first step is to setup and configure a build directory. To do this,
position your current working directory in the root of this project and run this command:
```
cmake -S . -B ./build
```
Then to build tests and libraries, run the following command from the same directory:
```
cmake --build ./build
```
This will build all of the target mentions in the `CMakeLists.txt` file (Windows: in debug mode).
To build a specific target, the `--target` option can be used when starting the build with CMake:
```
cmake --build ./build --target redblacktree
```
Here we build the shared library target `redblacktree`.

## Requirements

 - CMake minimum version 3.15
 - C99 standard compliant C compiler, and C library
 - Makefile, MSBuild, Ninja or other CMake generable build system

## License

This software is licensed under the MIT license.
More information about this license can be found in the LICENSE file.