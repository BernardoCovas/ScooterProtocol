cmake_minimum_required(VERSION 3.10)

file(GLOB __CC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(libninebot ${__CC_SRCS})
