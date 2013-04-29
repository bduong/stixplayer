#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <id3tag.h>
#include <linux/limits.h>
#include <assert.h>

int read_tag( int fd, unsigned char *data, unsigned long length);
