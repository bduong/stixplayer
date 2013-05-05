/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief Capture and process TCP/IP requests from the remote client.
*/
#ifndef _TRANSCEIVER_H
#define _TRANSCEIVER_H

/**
* Handles for the TCP sockets we open.
*/
extern int socket_fd[3];

void * sendInfo(void *);
void * receiveInfo(void *);
void * sendPlaylist(void * arg);
#endif
