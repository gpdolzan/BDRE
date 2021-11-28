# GRR20209948 Gabriel Pimentel Dolzan
# Makefile BoulderDash Remake

# Flags used during compilation
CFLAGS = -std=c99 -Wall -g
LDLIBS = -lm
PACKAGEFLAGS = pkg-config
ALLEGRO5FLAGS = allegro-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_audio-5 allegro_acodec-5 --libs --cflags

# Object-files
objects = main.o scoreboard.o map_reader.o game_manager.o allegro_manager.o

# Compile all
all: boulderdash

# Game compilation
boulderdash: $(objects)
	gcc $(CFLAGS) $(objects) $(LDLIBS) -o boulderdash $$($(PACKAGEFLAGS) $(ALLEGRO5FLAGS))

# Creating object files based on all the existing .c files
main.o: main.c
	cc -c main.c $(CFLAGS)

scoreboard.o: scoreboard.c scoreboard.h
	cc -c scoreboard.c $(CFLAGS)

map_reader.o: map_reader.c map_reader.h
	cc -c map_reader.c $(CFLAGS)

game_manager.o: game_manager.c game_manager.h
	cc -c game_manager.c $(CFLAGS)

allegro_manager.o: allegro_manager.c allegro_manager.h allegro_structs.h
	cc -c allegro_manager.c $(CFLAGS)

# Removes all object files
clean:
	rm -f *.o

# Removes all object files and executable
purge: clean
	rm -f boulderdash