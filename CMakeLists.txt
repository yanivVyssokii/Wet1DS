cmake_minimum_required(VERSION 3.20)
project(Wet1DS)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Wet1DS
        AVLTree.cpp
        AVLTree.h
        main23a1.cpp
        Node.h
        Node.cpp
        Player.cpp
        Player.h
        Team.cpp
        Team.h
        wet1util.h
        worldcup23a1.cpp
        worldcup23a1.h Node.cpp)
