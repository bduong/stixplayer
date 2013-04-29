#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mad.h>
#include <alsa/asoundlib.h>


int bt_init();
int close_handle();
void mad_decode(unsigned char *, unsigned long);
