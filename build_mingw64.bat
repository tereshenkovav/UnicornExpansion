REM Для работы скрипта сборки - CMake и MinGW/bin должны быть в PATH

cmake -G "MinGW Makefiles" ^
 -D CMAKE_CXX_COMPILER=g++.exe ^
 -D CMAKE_MAKE_PROGRAM=mingw32-make ^
 -B build-mingw64

cmake --build build-mingw64
