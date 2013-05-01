#ifndef STDLIB
#define STDLIB
#include <stdlib.h>
#include <stdio.h>
#endif

#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "transceiver.h"
#include "player.h"
#include "tags.h"
#include "playlist.h"
#include "keyboard.h"

int pause_play_flag;
int stop_flag;
char * songs[20];
int number_of_songs;
int break_input_loop;
int song_choice;
int socket_fd[3];

int total_stop;
void catch_control_c(int sig);

int main(int argc, char* argv[])
{
    pthread_t decode_thread_t, listener_t, id3_t, id4_t, keyboard_t;
	int run = 1;
	int runOuter = 1;
	int i;
	total_stop = 0;
    
    void* decode_thread_exit_p;
    void *keyboard(void * arg);
	
    decode_thread_data_t* decode_thread_data_p = (decode_thread_data_t*)malloc(sizeof(decode_thread_data_t));
    
    if (argc > 1)
    {
		getMP3Files(argv[1]);
		pause_play_flag = PAUSE;
		stop_flag = RUN;

		signal(SIGINT, catch_control_c);
		pthread_create(&listener_t, NULL, receiveInfo,(void *) &run);
		pthread_create(&id3_t, NULL, sendInfo,(void *) &run);
		pthread_create(&id4_t, NULL, sendPlaylist,(void *) &run);
		pthread_create(&keyboard_t,NULL,keyboard,0);

		song_choice = 0;
		printf("Press q to quit\n");
		while (total_stop == 0)
		{
			break_input_loop = 0;
			stop_flag = 0;
			printf("Press p to play\n");
			while (pause_play_flag == PAUSE && total_stop==0) {
				sleep(1);
			}
			printf("Select song\n");
			for (i = 0; i < number_of_songs; i++) {
			printf("%d %s\n", i, songs[i]);
			}

			char buffer[50];
			if (argv[1][strlen(argv[1]) - 1] == '/') { 
				sprintf(buffer, "%s%s", argv[1], songs[song_choice]);
			} else {
				sprintf(buffer, "%s/%s", argv[1], songs[song_choice]);				
			}
			printf("%s\n", buffer);

			decode_thread_data_p->filename = buffer;//argv[1];
			/* init of decode thread */
			if (pthread_create(&decode_thread_t,NULL,mad_decode,decode_thread_data_p) != 0)
			{
				fprintf(stderr, "Error creating decode thread!\n");
				return 1;
			}
			
			/* main loop */
			while (stop_flag == RUN)
				sleep(1);
			
			printf( "waiting for Decode thread to exit...\n");
			pthread_join(decode_thread_t, &decode_thread_exit_p);
			if (decode_thread_exit_p != PTHREAD_CANCELED)
			{
					printf("Decode return value:\n");
			}
			else
			{
					printf("Decode thread was cancelled.\n");
			}
		}
    }
    else
    {
		fprintf(stderr, "Usage: ./madtest mp3file\n");
    }       
        
    free(decode_thread_data_p);
    for (i = 0; i < number_of_songs; i++ ) {
		free(songs[i]);
    }
	for (i = 0; i < 3; i++) {
		if(socket_fd[i] != 0)
			close(socket_fd[i]);
	}
        
    return 0;
}

void catch_control_c(int sig) {
	printf("Press q to quit!!\n");	
}
