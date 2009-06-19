CC = gcc
LIBS = `sdl-config --libs --cflags` -lSDL_ttf -framework OpenGL -framework Cocoa 
OBJS = SDLMain.o graphics.o particle.o block.o grid.o  main.o element.o psys.o config.o gui.o
FLAGS = -Wall -O3 -std=c99 `sdl-config --cflags`
all: fluvium
fluvium: $(OBJS)
	$(CC) $(LIBS) -o $@ $(OBJS) $(FLAGS)
%.o:%.c
	$(CC) -c $+ -o $@ $(FLAGS)
clean:
	rm -f *.o *~
