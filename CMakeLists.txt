cmake_minimum_required(VERSION 3.10)

include(GNUInstallDirs)

file(GLOB __CC_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB __CC_HDRS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

set(__LIBDIR include/libninebot)

add_library(libninebot SHARED ${__CC_SRCS})

install(TARGETS libninebot
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
    FILES ${__CC_HDRS}
    DESTINATION ${__LIBDIR})