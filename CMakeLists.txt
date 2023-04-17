# CMake for windows builds only
cmake_minimum_required(VERSION 3.20)
project(libnewton)
set(CMAKE_CXX_STANDARD 17)


find_package(pylon REQUIRED)
find_package(OpenCV REQUIRED)

message( ${OpenCV_LIB_PATH} )
message( ${OpenCV_INCLUDE_DIRS} )
message( ${OpenCV_LIBS} )

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(_WINSOCK_DEPRECATED_NO_WARNINGS)
#add_compile_definitions(_DEBUG)
add_compile_definitions(_CSHARP)
add_compile_definitions(CV_IGNORE_DEBUG_BUILD_GUARD)

add_library(libnewton SHARED    src/ethercat.cpp
                                src/maxon.cpp
                                src/bits.cpp
                                src/galil.cpp
                                src/basler-camera.cpp
                                src/heap.cpp)

target_link_directories( libnewton PRIVATE
        ${OpenCV_LIB_PATH}
        ${PROJECT_SOURCE_DIR}/3rd/SOEM/build/Release
        "C:\\pcap\\Lib\\x64"
        "C:\\Program Files (x86)\\Galil\\gclib\\lib\\dynamic\\x64"
        "C:\\Program Files (x86)\\Galil\\gclib\\dll\\x64"
        "C:\\Program Files\\Basler\\pylon 7\\Development\\lib\\x64")
                
target_include_directories(libnewton PRIVATE
        ${PROJECT_SOURCE_DIR}/3rd/SOEM/
        ${PROJECT_SOURCE_DIR}/3rd/SOEM/oshw
        ${PROJECT_SOURCE_DIR}/3rd/SOEM/oshw/win32
        ${PROJECT_SOURCE_DIR}/3rd/SOEM/osal
        ${PROJECT_SOURCE_DIR}/3rd/SOEM/osal/win32
        ${OpenCV_INCLUDE_DIRS}
        "C:\\pcap\\Include"
        "C:\\Program Files (x86)\\Galil\\gclib\\include"
        "C:\\Program Files\\Basler\\pylon 7\\Development\\include")

# opencv_imgproc470d opencv_imgcodecs470d
target_link_libraries(libnewton soem Packet wpcap wsock32 ws2_32 Winmm gclib gclibo ${OpenCV_LIBS})


add_library(libnewton-managed SHARED 
    src/basler-camera-cli.cpp
)

target_link_directories( libnewton-managed PRIVATE
                ${OpenCV_LIB_PATH}
                ${PROJECT_SOURCE_DIR}/3rd/SOEM/build/Release
                "C:\\pcap\\Lib\\x64"
                "C:\\Program Files (x86)\\Galil\\gclib\\lib\\dynamic\\x64"
                "C:\\Program Files (x86)\\Galil\\gclib\\dll\\x64"
                "C:\\Program Files\\Basler\\pylon 7\\Development\\lib\\x64"
                ${PROJECT_SOURCE_DIR}/build/Release)

target_include_directories(libnewton-managed PRIVATE
                ${PROJECT_SOURCE_DIR}/3rd/SOEM/
                ${PROJECT_SOURCE_DIR}/3rd/SOEM/oshw
                ${PROJECT_SOURCE_DIR}/3rd/SOEM/oshw/win32
                ${PROJECT_SOURCE_DIR}/3rd/SOEM/osal
                ${PROJECT_SOURCE_DIR}/3rd/SOEM/osal/win32
                ${OpenCV_INCLUDE_DIRS}
                "C:\\pcap\\Include"
                "C:\\Program Files (x86)\\Galil\\gclib\\include"
                "C:\\Program Files\\Basler\\pylon 7\\Development\\include"
                ${PROJECT_SOURCE_DIR}/src/newton)

set_target_properties(libnewton-managed PROPERTIES COMMON_LANGUAGE_RUNTIME "")
target_compile_definitions(libnewton-managed PUBLIC _CRT_SECURE_NO_WARNINGS=0)
target_compile_definitions(libnewton-managed PUBLIC _WINSOCK_DEPRECATED_NO_WARNINGS=0)

target_link_libraries(libnewton-managed soem Packet wpcap wsock32 ws2_32 Winmm gclib gclibo ${OpenCV_LIBS} libnewton)

