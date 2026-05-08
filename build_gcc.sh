mkdir bin

cmake -B build-gcc
cmake --build build-gcc --parallel $(nproc)
