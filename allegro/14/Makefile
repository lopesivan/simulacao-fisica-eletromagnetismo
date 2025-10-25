PATH := /usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
export PATH

PKG_CONFIG_PATH:=/usr/lib/x86_64-linux-gnu/pkgconfig
export PKG_CONFIG_PATH

CC     = gcc
CFLAGS = -Wall -std=c99

SRC = main.c
BIN = app

ALLEGRO_LIBS_WITHOUT_VERSION = \
		allegro \
		allegro_font \
		allegro_ttf \
		allegro_main \
		allegro_primitives

ALLEGRO_LIBS_VERSION = $(addsuffix -5,$(ALLEGRO_LIBS_WITHOUT_VERSION))

ALLEGRO_LIBS         = $(ALLEGRO_LIBS_VERSION)
ALLEGRO_CFLAGS       = $(shell /usr/bin/pkg-config --cflags $(ALLEGRO_LIBS))
ALLEGRO_LDLIBS       = $(shell /usr/bin/pkg-config --libs $(ALLEGRO_LIBS))

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(ALLEGRO_CFLAGS) $^ -o $@ $(ALLEGRO_LDLIBS) -lm

indent:
	indent -kr -i4 -nut -l68 $(SRC)
run:
	./$(BIN)

clean:
	rm -f $(BIN) *~
