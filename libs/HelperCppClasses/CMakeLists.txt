cmake_minimum_required(VERSION 3.21)

project(HelperCppClasses LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(HelperCppClasses)

set(sources
    include/HelperCppClasses/StringTools.h
    include/HelperCppClasses/Countdown.h
    include/HelperCppClasses/Vector2D.h
    src/StringTools.cpp
    src/Countdown.cpp
    src/Vector2D.cpp)

target_sources(HelperCppClasses PRIVATE ${sources})
target_include_directories(HelperCppClasses PUBLIC ${PROJECT_SOURCE_DIR}/include)

#set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
