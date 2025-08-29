cmake_minimum_required(VERSION 3.21)

project(UnicornExpansion LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/UnicornExpansion/UnicornExpansion)
