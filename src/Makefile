CC=g++
LINKER_FLAGS= -mwindows -lmingw32 -lSDL2main -lsqlite3 -lSDL2 -lSDL2_image -lSDL2_ttf -lzip
CFLAGS= -Iinclude/SDL2
OBJETOS= display.o vector.o mesh.o
../renderer: main.c $(OBJETOS)
	$(CC) main.c $(CFLAGS) -g -o ../renderer $(OBJETOS) $(LINKER_FLAGS)

build:
	g++ $(objs) main.c -Iinclude/SDL2 -o ../renderer -lSDL2 -lSDL2main -lmingw32 -mwindows 

display.o:	display.h display.c
	g++ display.h display.c -Iinclude/SDL2 -c -lSDL2 -lSDL2main -lmingw32 -mwindows $(objs)

vector.o:	vector.h vector.c
	g++ vector.h vector.c -c

mesh.o:		mesh.h mesh.c
	g++ mesh.h mesh.c -c

run:
	./renderer
clean:
	rm renderer

