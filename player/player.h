/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief
*
* @summary
*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

/**
* Control flag signaling whether the player should (un)pause.
*/
extern int pause_play_flag;

/**
* Control flag signalling whether the player should stop playing and reset.
*/
extern int stop_flag;

typedef struct {
        char* filename;
} decode_thread_data_t;

typedef struct buffer_s {
  unsigned char *buf;
  unsigned long len;
} buffer_t;

int bt_init();
int close_handle();
void *mad_decode(void * pthread_data);
#endif

#ifndef PAUSE
#define PAUSE 0
#endif

#ifndef PLAY
#define PLAY 1
#endif

#ifndef RUN
#define RUN 0
#endif

#ifndef STOP
#define STOP 1
#endif

