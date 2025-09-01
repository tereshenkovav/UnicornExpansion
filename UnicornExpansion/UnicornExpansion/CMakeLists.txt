add_executable(UnicornExpansion
main.cpp
Animation.cpp
ClickerCounter.cpp
ComponentAttacker.cpp
ComponentDetoxer.cpp
ComponentEnemy.cpp
ComponentEnemyLair.cpp
ComponentEnemyTarget.cpp
ComponentHarvester.cpp
ComponentHealer.cpp
ComponentMeleeEnemy.cpp
ComponentPortal.cpp
ComponentRadar.cpp
ComponentResource.cpp
ComponentUnicorn.cpp
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

include_directories(SFML.mingw/include)
include_directories(ZetScript/include)

find_library(SFML_LIB_Window sfml-window "SFML.mingw/lib")
find_library(SFML_LIB_Graphics sfml-graphics "SFML.mingw/lib")
find_library(SFML_LIB_System sfml-system "SFML.mingw/lib")
find_library(SFML_LIB_Audio sfml-audio "SFML.mingw/lib")
find_library(ZetScript_LIB zetscript-2-1-0-static "ZetScript/lib.mingw")

target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Window})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Graphics})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_System})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Audio})
target_link_libraries(UnicornExpansion PRIVATE ${ZetScript_LIB})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
