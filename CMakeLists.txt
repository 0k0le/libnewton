# CMake for windows builds only
cmake_minimum_required(VERSION 3.20)
project(libnewton)
set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)

add_library(libnewton SHARED    src/bits.cpp
                                src/heap.cpp
                                src/net.cpp
                                src/client.cpp)

# opencv_imgproc470d opencv_imgcodecs470d
target_link_libraries(libnewton wsock32 ws2_32 Winmm)
