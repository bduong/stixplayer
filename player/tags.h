#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <id3tag.h>
#include <linux/limits.h>
#include <assert.h>

int read_tag( int fd, unsigned char *data, unsigned long length);

//typedef struct {
extern char song_title[40];
extern char song_artist[40];
//} tag_info_t;
