# redblacktree
A red black tree written in C. This is my implementation of the classic red-black tree found in the Cormen et. al. textbook.

## How-to build

This project uses CMake to build the libraries and tests associated with this project.

To build everything, the first step is to setup and configure a build directory. To do this,
position your current working directory in the root of this project and run this command:
```
cmake -S . -B build
```
Then to build tests and libraries, run the following command from the same directory:
```
cmake --build build
```
This will build all of the target mentions in the `CMakeLists.txt` file (Windows: in debug mode).
