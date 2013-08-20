del test.exe
gcc -pedantic -std=c99 -g -O3 main.c stb_image.c int_list.c video.c game.c assets.c mem_wrap.c sound.c tween.c -lmingw32 -lSDL2main -lSDL2 -o test.exe