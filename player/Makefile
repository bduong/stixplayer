IDIR =$(CURDIR)
EC535=/ad/eng/courses/ec/ec535/mixed
CC=${EC535}/gumstix/oe/cross/bin/arm-linux-gcc
CFLAGS=-I$(IDIR)
LDFLAGS=-L$(IDIR)
OPTS =-lm -lmad -lasound -lid3tag
LDIR =../lib
DEPS = player.h tags.h transceiver.h playlist.h keyboard.h
OBJ = player.o tags.o main.o transceiver.o playlist.o keyboard.o

default: play

all: play network

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

play: $(OBJ) $(DEPS)
	$(CC) $(OPTS) -o player $^ $(CFLAGS) $(LDFLAGS)
	
network: 
	$(CC) transceiver.c -o network -lpthread

.PHONY: clean network play

clean:
	rm -f $(IDIR)/*.o $(IDIR)/*~ $(IDIR)core $(IDIR)/player $(IDIR)/network
