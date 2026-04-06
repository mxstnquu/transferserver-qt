cmake_minimum_required(VERSION 3.16)
project(transferserver-qt LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

qt_standard_project_setup()

qt_add_executable(transferserver-qt    
    src/FileHandler.cpp 
    src/Server.cpp 
    main.cpp
    include/FileHandler.hpp 
    include/Server.hpp 
)

target_include_directories(transferserver-qt  
    PRIVATE include
)

target_link_libraries(transferserver-qt 
    PRIVATE 
        Qt6::Core
        Qt6::Network
)