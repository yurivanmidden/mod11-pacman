cmake_minimum_required(VERSION 3.9)
project(pacman)

set(CMAKE_CXX_STANDARD 14)

add_executable(pacman src/main.cpp src/UI.cpp include/GameObjectStruct.h include/UI.h include/example.h src/Game.cpp include/Game.h src/Character.cpp include/Character.h)