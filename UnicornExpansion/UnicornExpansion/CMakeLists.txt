add_executable(UnicornExpansion
main.cpp
Animation.cpp
ClickerCounter.cpp
CppTools.cpp
FinderByBestDistance.cpp
FogBuilder.cpp
Game.cpp
GameUnit.cpp
MiniMap.cpp
MushroomNet.cpp
SeedStore.cpp
SfmlTools.cpp
SubTerrainBuilder.cpp
Texts.cpp
UnitComponent.cpp
UnitFactory.cpp
UnitMover.cpp
WayFinder.cpp
)

# Добавление каталога с классами компонент и установка текущего каталога
# как включаемого, чтобы компоненты видели заголовочные файлы классов игры
target_include_directories(UnicornExpansion PUBLIC .)
add_subdirectory(components)

include_directories(SFML.mingw/include)

find_library(SFML_LIB_Window sfml-window "SFML.mingw/lib")
find_library(SFML_LIB_Graphics sfml-graphics "SFML.mingw/lib")
find_library(SFML_LIB_System sfml-system "SFML.mingw/lib")
find_library(SFML_LIB_Audio sfml-audio "SFML.mingw/lib")

target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Window})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Graphics})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_System})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Audio})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
