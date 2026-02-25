cmake_minimum_required(VERSION 3.21)

project(SfmlGameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SfmlGameEngine)

set(sources
    include/SfmlGameEngine/Animation.h
    include/SfmlGameEngine/SfmlTools.h
    src/Animation.cpp
    src/SfmlTools.cpp)

target_sources(SfmlGameEngine PRIVATE ${sources})
target_include_directories(SfmlGameEngine PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Здесь мы добавляем путь к каталогу SFML в том же каталоге библиотек, что и 
# сама библиотека SfmlGameEngine, предполагая, что библитека SFML уже
# подключена в главном проекте, указывая её линковку без добавления
# подкаталога SFML с его CMakeLists

target_include_directories(SfmlGameEngine PUBLIC ${PROJECT_SOURCE_DIR}/../SFML/include)
target_link_libraries(SfmlGameEngine PRIVATE SFML::Graphics)
target_link_libraries(SfmlGameEngine PRIVATE SFML::Audio)
target_link_libraries(SfmlGameEngine PRIVATE SFML::System)
target_link_libraries(SfmlGameEngine PRIVATE SFML::Window)
