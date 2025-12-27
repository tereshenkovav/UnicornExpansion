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
UnitSelector.cpp
WayFinder.cpp
)

# Добавление каталога с классами компонент и установка текущего каталога
# как включаемого, чтобы компоненты видели заголовочные файлы классов игры
target_include_directories(UnicornExpansion PUBLIC .)
add_subdirectory(components)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
