cmake_minimum_required(VERSION 3.5)
set(CMAKE_BUILD_TYPE Debug)
add_definitions(-std=c++17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(GST REQUIRED gstreamer-1.0)

set(PLAYER_SOURCES src/HTTPPlayer.cpp src/FilePlayer.cpp src/MediaPlayer.cpp src/main.cpp src/Controller.cpp)
add_executable(play ${PLAYER_SOURCES})
target_compile_options(play PRIVATE -Wall -Werror  ${GST_CFLAGS} -Wno-switch)
target_include_directories(play PUBLIC ${GLIB_INCLUDE_DIRS} ${GST_INCLUDE_DIRS}  inc/)

target_link_libraries(play
    ${GLIB_LIBRARIES}
    ${GST_LIBRARIES}
    pthread)
