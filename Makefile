# Makefile

pkg_packages := ckpi

PKG_CFLAGS  := $(shell pkg-config --cflags $(pkg_packages))
PKG_LDFLAGS := $(shell pkg-config --libs $(pkg_packages))

INCLUDES := -I/usr/local/include
LIBS := -L/usr/local/lib -lckpi -lc
CC    := gcc

DEBUG := -g
EXTRA_FLAGS :=
EXTRA_OPTION := -std=c99 -Wl,--no-as-needed -Wall -Wextra -Wconversion -Wsign-conversion -Winit-self \
		  -Wunreachable-code -pedantic -Wformat=2 -Wswitch-default -Wtrigraphs \
		  -Wstrict-overflow=4 -Wfloat-equal -Wundef -Wshadow \
		  -Wbad-function-cast -Wcast-qual -Wcast-align -Wlogical-op

CFLAGS := -Wall -c $(DEBUG) $(INCLUDES) $(PKG_CFLAGS) $(EXTRA_FLAGS)
LFLAGS := -Wall $(DEBUG) $(PKG_LDFLAGS) $(LIBS)

PROG_NAME := sensor_insert

targets = main

ofiles :=  ./kp.o ./main.o

.PHONY: all
all : $(targets)

main: $(ofiles)
	$(CC) $(EXTRA_OPTION) $(LFLAGS) $(ofiles) -o $(PROG_NAME)

.c.o:
	$(CC) $(EXTRA_OPTION) $(CFLAGS) -o $@ $<


.PHONY: clean
clean:
	rm -f *.o
	rm -f $(PROG_NAME)
