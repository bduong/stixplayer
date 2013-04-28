IDIR =$(CURDIR)
CC=arm-linux-gcc
CFLAGS=-I$(IDIR)
LDFLAGS=-L$(IDIR)
OPTS =-lm -lmad -lasound
LDIR =../lib
DEPS = player.h
OBJ = player.o main.o

%.o: %.c $(DEPS)
	$(CC) $(OPTS) -c -o $@ $< $(CFLAGS)

play: $(OBJ) $(DEPS)
	$(CC) $(OPTS) -o player $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(IDIR)/*.o $(IDIR)/*~ $(IDIR)core $(IDIR)/player
