cmake_minimum_required(VERSION 3.16)

project(wpp-client LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wpp-client main.cpp
    service/wppconnectorservice.h service/wppconnectorservice.cpp)

target_link_libraries(wpp-client PRIVATE
  nlohmann_json::nlohmann_json
  qrencode
)


include(GNUInstallDirs)
install(TARGETS wpp-client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
