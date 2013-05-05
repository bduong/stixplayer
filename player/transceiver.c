/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief Capture and process TCP/IP requests from the remote client.
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>

#include "transceiver.h"
#include "player.h"
#include "tags.h"
#include "playlist.h"

#define RECEIVE_PORT 5001
#define TRANSMIT_PORT 5000
#define PLAYLIST_PORT 5002

int pause_play_flag;
int stop_flag;
char song_title[40];
char song_artist[40];
int song_choice;
int number_of_songs;
char * songs[20];
int socket_fd[3];

/**
* @brief Sends the current song information when requested by a client.
*/
void * sendInfo(void * arg) {
	int * run = (int *) arg;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	socket_fd[0] = listenfd;
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(TRANSMIT_PORT); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(run)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

		printf("song title: %s artist:%s\n", song_title, song_artist);
		snprintf(sendBuff, sizeof(sendBuff), "%s\n%s\n", song_title, song_artist); 		
        write(connfd, sendBuff, strlen(sendBuff)); 
        printf("Sent packet\n");

        close(connfd);
        sleep(1);
     }
}

/**
* @brief Receives and processes all user commands.
*/
void * receiveInfo(void * arg) {
	int * run = (int *) arg;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
	socket_fd[1] = listenfd;
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(RECEIVE_PORT); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(run)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

        read(connfd, sendBuff, 1024); 
        if (strncmp(sendBuff, "Pause", 5) == 0) {
        	pause_play_flag = !pause_play_flag;
        } else if (strncmp(sendBuff, "Next", 4) == 0) {
	        printf("Go Next\n");
        	 stop_flag = STOP;
        	 pause_play_flag = PLAY;
        	 song_choice = (song_choice + 1) % number_of_songs;
        } else if (strncmp(sendBuff, "Back", 4) == 0) {
        	printf("Go Back\n");
        	stop_flag = STOP;
        	pause_play_flag = PLAY;
        	song_choice = (song_choice <= 0) ? number_of_songs - 1 : song_choice -1;        	
        } else if (strncmp(sendBuff, "Stop", 4) == 0) {
        	stop_flag = STOP;
  	        pause_play_flag = PAUSE;        	
        } else if (strncmp(sendBuff, "Play", 4) == 0) {
        	int num = atoi(sendBuff + 4);
        	if (num < number_of_songs && num != song_choice) {
        		song_choice = num;
        		stop_flag = STOP;
        		pause_play_flag = PLAY;
        	}
        }
        printf("Got Packet: %s\n", sendBuff);

        close(connfd);
        sleep(1);
     }
}

/**
* @brief Sends the songs names in the playlist to the client when requested.
*/
void * sendPlaylist(void * arg) {
	int * run = (int *) arg;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 
    int i;

    char sendBuff[2049];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);	
	socket_fd[2] = listenfd;
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(sendBuff, '0', sizeof(sendBuff)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(PLAYLIST_PORT); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10); 

    while(run)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

		for (i = 0; i < number_of_songs; i++) {
			snprintf(sendBuff, sizeof(sendBuff), "%s\n", songs[i]); 
		
        	write(connfd, sendBuff, strlen(sendBuff)); 
        }
        sprintf(sendBuff, "done\n");
      	write(connfd, sendBuff, strlen(sendBuff));         
        printf("Sent packet\n");

        close(connfd);
     }
}
