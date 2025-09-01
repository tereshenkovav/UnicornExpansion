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
include_directories(JsonCPP/include)

find_library(SFML_LIB_Window sfml-window "SFML.mingw/lib")
find_library(SFML_LIB_Graphics sfml-graphics "SFML.mingw/lib")
find_library(SFML_LIB_System sfml-system "SFML.mingw/lib")
find_library(SFML_LIB_Audio sfml-audio "SFML.mingw/lib")
find_library(JSON_LIB jsoncpp "JsonCPP/lib.mingw")
find_library(ZetScript_LIB zetscript-2-1-0-static "ZetScript/lib.mingw")

target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Window})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Graphics})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_System})
target_link_libraries(UnicornExpansion PRIVATE ${SFML_LIB_Audio})
target_link_libraries(UnicornExpansion PRIVATE ${JSON_LIB})
target_link_libraries(UnicornExpansion PRIVATE ${ZetScript_LIB})

# Команда копирует файл из каталога сборки в каталог bin.
# Это нужно потому, что по неясной причине, в выходном каталоге появляется
# также файл .dll.a, который не нужен.
# Поэтому решение через set(EXECUTABLE_OUTPUT_PATH не подходит.

add_custom_command(
        TARGET UnicornExpansion POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_BINARY_DIR}/UnicornExpansion.exe
        ${PROJECT_SOURCE_DIR}/bin)
