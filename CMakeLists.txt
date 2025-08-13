cmake_minimum_required(VERSION 3.16)
project(SnakeGame LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(SnakeGame
    main.cpp
    snake.cpp
    snake.h
)

qt_add_resources(SnakeGame "assets"
    PREFIX "/"
    FILES
        img/apple.png
        img/dot.png
        img/head.png
)

target_link_libraries(SnakeGame PRIVATE Qt6::Widgets)