PATH := /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
export PATH

PKG_CONFIG_PATH:=/usr/lib/x86_64-linux-gnu/pkgconfig
export PKG_CONFIG_PATH

CC = gcc
CFLAGS = -Wall -std=c99
SRC = mouse_pos.c
BIN = mouse_pos

ALLEGRO_LIBS = allegro-5 allegro_font-5 allegro_ttf-5 allegro_main-5 allegro_primitives-5
ALLEGRO_CFLAGS = $(shell /usr/bin/pkg-config --cflags $(ALLEGRO_LIBS))
ALLEGRO_LDLIBS = $(shell /usr/bin/pkg-config --libs $(ALLEGRO_LIBS))

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(ALLEGRO_CFLAGS) $^ -o $@ $(ALLEGRO_LDLIBS) -lm

run:
	./$(BIN)

clean:
	rm -f $(BIN)
