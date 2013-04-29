IDIR =$(CURDIR)
CC=arm-linux-gcc
CFLAGS=-I$(IDIR)
LDFLAGS=-L$(IDIR)
OPTS =-lm -lmad -lasound -lid3tag
LDIR =../lib
DEPS = player.h tags.h
OBJ = player.o tags.o main.o

%.o: %.c $(DEPS)
	$(CC) $(OPTS) -c -o $@ $< $(CFLAGS)

play: $(OBJ) $(DEPS)
	$(CC) $(OPTS) -o player $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(IDIR)/*.o $(IDIR)/*~ $(IDIR)core $(IDIR)/player
