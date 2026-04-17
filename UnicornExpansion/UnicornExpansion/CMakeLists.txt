add_executable(UnicornExpansion
main.cpp
ClickerCounter.cpp
FinderByBestDistance.cpp
FogBuilder.cpp
Game.cpp
GameUnit.cpp
MiniMap.cpp
MushroomNet.cpp
SceneMainMenu.cpp
SceneGameMenu.cpp
SceneStartMenu.cpp
SceneEndGame.cpp
SceneOptions.cpp
SceneHelp.cpp
SceneTask.cpp
SceneGame.cpp
SeedStore.cpp
SubTerrainBuilder.cpp
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
target_link_options(UnicornExpansion PRIVATE -mwindows)
