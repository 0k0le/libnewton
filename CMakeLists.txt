cmake_minimum_required(VERSION 3.10)

project(libnewton VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

#set(pylon_DIR "C:\\Program Files\\Basler\\pylon 7\\Development\\CMake\\pylon")
#set(OpenCV_DIR "C:\\opencv\\build\\x64\\vc16\\lib")

#find_package(pylon 7.2.1 REQUIRED)
#find_package(OpenCV REQUIRED)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
add_compile_definitions(_DEBUG)

link_directories(${PROJECT_SOURCE_DIR}/3rd/SOEM/build/Release "C:\\pcap\\Lib\\x64")

include_directories(${PROJECT_SOURCE_DIR}/3rd/SOEM/
                    ${PROJECT_SOURCE_DIR}/3rd/SOEM/oshw
                    ${PROJECT_SOURCE_DIR}/3rd/SOEM/oshw/win32
                    ${PROJECT_SOURCE_DIR}/3rd/SOEM/osal
                    ${PROJECT_SOURCE_DIR}/3rd/SOEM/osal/win32
                    "C:\\pcap\\Include")

add_library(libnewton SHARED    src/ethercat.cpp
                                src/maxon.cpp
                                src/bits.cpp)

target_link_libraries(libnewton soem Packet wpcap wsock32 ws2_32 Winmm)
