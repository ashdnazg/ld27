rm streaker
gcc -O3 *.c `sdl2-config --cflags` -Wl,-Bstatic -lSDL2 -lm -Wl,-Bdynamic -ldl -lpthread -lrt -o streaker
