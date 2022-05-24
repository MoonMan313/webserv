cmake_minimum_required(VERSION 3.21)
project(parser)

set(CMAKE_CXX_STANDARD 98)

add_executable(parser main.cpp ParserConfig.cpp ParserConfig.hpp ConfigUtils.hpp)
