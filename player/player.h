#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <mad.h>
#include <alsa/asoundlib.h>

typedef struct {
//        int fp;
        char* filename;
//  	unsigned char *data;
} decode_thread_data_t;

typedef struct buffer_s {
  unsigned char *buf;
  unsigned long len;
} buffer_t;

int bt_init();
int close_handle();
void *mad_decode(void * pthread_data);
