cmake_minimum_required(VERSION 3.21)

project(SfmlGameEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SfmlGameEngine)

set(sources
    include/SfmlGameEngine/Animation.h
    include/SfmlGameEngine/SfmlTools.h
    include/SfmlGameEngine/Scene.h
    include/SfmlGameEngine/Engine.h
    include/SfmlGameEngine/Texts.h
    include/SfmlGameEngine/Colors.h
    include/SfmlGameEngine/Button.h
    include/SfmlGameEngine/Profile.h
    include/SfmlGameEngine/Checkbox.h
    src/Animation.cpp
    src/SfmlTools.cpp
    src/Scene.cpp
    src/Engine.cpp
    src/Texts.cpp
    src/Colors.cpp
    src/Button.cpp
    src/Profile.cpp
    src/Checkbox.cpp
)

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

# Здесь мы добавляем путь к каталогу HelperCppClasses в том же каталоге библиотек, что и 
# сама библиотека SfmlGameEngine, предполагая, что библитека уже
# подключена в главном проекте.
# В будущем она должна быть вынесена в отдельный подкаталог для основного проекта,
# и в подкаталог для движка
target_include_directories(SfmlGameEngine PUBLIC ${PROJECT_SOURCE_DIR}/../HelperCppClasses/include)
