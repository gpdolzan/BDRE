# Makefile padrao

#Variaveis
CFLAGS = -std=c99 -Wall -g
LDLIBS = -lm
PACKAGEFLAGS = pkg-config
ALLEGRO5FLAGS = allegro-5 allegro_font-5 allegro_ttf-5  allegro_image-5  allegro_audio-5 allegro_acodec-5 --libs --cflags

#object-files
objects = main.o map_reader.o game_manager.o allegro_manager.o

#default execution
all: boulderdash

boulderdash: $(objects)
	gcc $(CFLAGS) $(objects) $(LDLIBS) -o boulderdash $$($(PACKAGEFLAGS) $(ALLEGRO5FLAGS))

main.o: main.c
	cc -c main.c $(CFLAGS)

allegro_manager.o: allegro_manager.c
	cc -c allegro_manager.c $(CFLAGS)

map_reader.o: map_reader.c
	cc -c map_reader.c $(CFLAGS)

game_manager.o: game_manager.c
	cc -c game_manager.c $(CFLAGS)

clean:
	rm -f *.o

purge: clean
	rm -f boulderdash