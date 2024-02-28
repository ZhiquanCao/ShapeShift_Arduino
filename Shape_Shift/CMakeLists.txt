cmake_minimum_required(VERSION 3.10)
project(MyArduinoProject)

set(CMAKE_TOOLCHAIN_FILE /nyu/adv_mechatronics/arduino-cmake/cmake/ArduinoToolchain.cmake)

include_directories(src)
add_subdirectory(src)


# Include Arduino CMake module (if available)
set(BOARD uno)






