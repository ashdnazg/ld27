del test.exe
gcc -pedantic -std=c99 -g main.c stb_image.c int_list.c video.c game.c assets.c mem_wrap.c sound.c tween.c actor.c ai.c font.c projectile.c -lmingw32 -lSDL2main -lSDL2 -o test.exe