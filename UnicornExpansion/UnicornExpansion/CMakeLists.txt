add_executable(UnicornExpansion
main.cpp
ClickerCounter.cpp
FinderByBestDistance.cpp
FogBuilder.cpp
Game.cpp
GameUnit.cpp
MiniMap.cpp
MushroomNet.cpp
SeedStore.cpp
SubTerrainBuilder.cpp
UnitComponent.cpp
UnitFactory.cpp
UnitMover.cpp
UnitSelector.cpp
WayFinder.cpp
UserProfile.cpp
)

# Добавление каталога с классами компонент, сцен и установка текущего каталога
# как включаемого, чтобы компоненты видели заголовочные файлы классов игры
target_include_directories(UnicornExpansion PUBLIC .)
add_subdirectory(components)
add_subdirectory(scenes)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
target_link_options(UnicornExpansion PRIVATE -mwindows)
