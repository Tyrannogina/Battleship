cmake_minimum_required(VERSION 3.10)
project(Battleship)

set(CMAKE_CXX_STANDARD 17)

set(BINARY ${CMAKE_PROJECT_NAME})

configure_file(adaship_config.ini ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
set(SOURCES src/main.cpp src/game.cpp src/game.h src/board.h src/board.cpp src/config_parser.cpp src/config_parser.h src/IOHelper.cpp src/player.h src/player.cc)
include_directories(.)

add_executable(${BINARY}_exec ${SOURCES})