cmake_minimum_required(VERSION 3.16)

project(wpp-client LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wpp-client main.cpp
    service/wppconnectorservice.h service/wppconnectorservice.cpp)

include(GNUInstallDirs)
install(TARGETS wpp-client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
