cmake_minimum_required(VERSION 3.0.0)
project(server_program VERSION 0.1.0)

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS})

add_executable(server_program main.cpp)

target_link_libraries( server_program ${OpenCV_LIBS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
