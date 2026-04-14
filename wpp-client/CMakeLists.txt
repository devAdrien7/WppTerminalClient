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
    service/wppconnectorservice.h service/wppconnectorservice.cpp
    configuration/signalmanager.h configuration/signalmanager.cpp
    core/application.h core/application.cpp
    view/login.h view/login.cpp
    view/IView.h

    util/IObserver.h
    util/Observable.h
    util/OBSERVABLE_COMMAND.h
    util/utilalghoritms.h util/utilalghoritms.cpp
    core/ViewType.h
    view/chat.h view/chat.cpp)

target_link_libraries(wpp-client PRIVATE
  nlohmann_json::nlohmann_json
  qrencode
  ncursesw
)



include(GNUInstallDirs)
install(TARGETS wpp-client
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
