SET PATH=%PATH%;C:\mingw64\bin

cd ..\UnicornExpansion\UnicornExpansion
g++ -o ..\..\bin\UnicornExpansion.exe -std=c++17 main.cpp ^
Animation.cpp ^
BusyMap.cpp ^
ComponentAttacker.cpp ^
ComponentEnemy.cpp ^
ComponentEnemyLair.cpp ^
ComponentEnemyTarget.cpp ^
ComponentPortal.cpp ^
ComponentHarvester.cpp ^
ComponentHealer.cpp ^
ComponentMeleeEnemy.cpp ^
ComponentRadar.cpp ^
ComponentResource.cpp ^
ComponentUnicorn.cpp ^
Countdown.cpp ^
CppTools.cpp ^
TreeBuilder.cpp ^
FinderByBestDistance.cpp ^
Game.cpp ^
GameUnit.cpp ^
MiniMap.cpp ^
SfmlTools.cpp ^
Texts.cpp ^
UnitComponent.cpp ^
UnitFactory.cpp ^
UnitMover.cpp ^
WayFinder.cpp ^
-I SFML.mingw\include ^
-I ZetScript\include ^
-I JsonCPP\include ^
-LSFML.mingw\lib -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system ^
-LZetScript\lib.mingw -lzetscript-2-1-0-static ^
-LJsonCPP\lib.mingw -ljsoncpp
