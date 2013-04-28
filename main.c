#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "player.h"

int main(int argc, char *argv[]){

  init();
   if (argc != 2) {
    fprintf(stderr, "Usage: ./madtest mp3file\n");
    return 1;
  }

  char *filename = argv[1];
  int ret;

  struct stat s;
  ret = stat(filename, &s);
  if (ret < 0) {
    fprintf(stderr, "Could not stat %s\n", filename);
    return 1;
  }

  int fd = open(filename, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return 1;
  }
  
  unsigned char *data;
  data = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE|MAP_FILE, fd, 0);
  if ((int)data == -1) {
    perror("mmap");
    close(fd);
    return 1;
  }

  mad_decode(data, s.st_size);

  munmap(data, s.st_size);
  close(fd);

  close_handle();

  return 0;
}
