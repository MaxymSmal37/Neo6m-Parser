cmake_minimum_required(VERSION 3.24)
project(Neo6_Parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(Neo6_Parser main.cpp GPS_Module/GPS_Module.cpp GPS_Module/GPS_Module.h)
