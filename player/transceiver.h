#ifndef _TRANSCEIVER_H
#define _TRANSCEIVER_H
extern int socket_fd[3];

void * sendInfo(void *);
void * receiveInfo(void *);
void * sendPlaylist(void * arg);
#endif
