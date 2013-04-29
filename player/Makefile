IDIR =$(CURDIR)
EC535=/ad/eng/courses/ec/ec535/mixed
CC=${EC535}/gumstix/oe/cross/bin/arm-linux-gcc
CFLAGS=-I$(IDIR)
LDFLAGS=-L$(IDIR)
OPTS =-lm -lmad -lasound -lid3tag
LDIR =../lib
DEPS = player.h tags.h
OBJ = player.o tags.o main.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) 

play: $(OBJ) $(DEPS)
	$(CC) $(OPTS) -o player $^ $(CFLAGS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(IDIR)/*.o $(IDIR)/*~ $(IDIR)core $(IDIR)/player
