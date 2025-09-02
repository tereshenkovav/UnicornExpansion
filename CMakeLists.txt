cmake_minimum_required(VERSION 3.21)

project(UnicornExpansion LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/UnicornExpansion/UnicornExpansion)

add_subdirectory(libs/HelperCppClasses)
target_link_libraries(UnicornExpansion PRIVATE HelperCppClasses)

# Настройка вложенного проекта jsonlib
set(JSONCPP_WITH_TESTS OFF)
set(BUILD_SHARED_LIBS OFF)
set(BUILD_OBJECT_LIBS OFF)
set(JSONCPP_WITH_EXAMPLE OFF)
# Добавляем каталог проекта
add_subdirectory(libs/jsoncpp)
# Указываем для него версию C++, переопределяя значения внутри
set_property(TARGET jsoncpp_static PROPERTY CXX_STANDARD 17)
# Добавляем библиотеку
target_link_libraries(UnicornExpansion PRIVATE jsoncpp_static)

# Настройка вложенного проекта zetscript
# Добавляем каталог проекта
add_subdirectory(libs/zetscript)
# Добавляем библиотеку
target_link_libraries(UnicornExpansion PRIVATE zetscript-2-1-0-static)
target_include_directories(UnicornExpansion PUBLIC ${PROJECT_SOURCE_DIR}/libs/zetscript/src)
