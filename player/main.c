#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "transceiver.h"
#include "player.h"
#include "tags.h"
#include "playlist.h"

int pause_play_flag;
int stop_flag;
char * songs[20];
int number_of_songs;
int break_input_loop;
int song_choice;

int main(int argc, char* argv[])
{
    pthread_t decode_thread_t, listener_t, id3_t, id4_t;
	int run = 1;
	int runOuter = 1;
	int i;
    
    void* decode_thread_exit_p;
    
    decode_thread_data_t* decode_thread_data_p = (decode_thread_data_t*)malloc(sizeof(decode_thread_data_t));
    
    if (argc > 1)
    {
    			getMP3Files(argv[1]);
				pause_play_flag = PAUSE;
				stop_flag = RUN;
//                decode_thread_data_p->data = NULL;
				pthread_create(&listener_t, NULL, receiveInfo,(void *) &run);
				pthread_create(&id3_t, NULL, sendInfo,(void *) &run);
				pthread_create(&id4_t, NULL, sendPlaylist,(void *) &run);				
   				song_choice = 0;
                printf("Press q to quit\n");
				while (1)
				{
					break_input_loop = 0;
					stop_flag = 0;
					printf("Press p to play\n");
					while (pause_play_flag == PAUSE) {
						printf("%d\n", pause_play_flag);
						sleep(1);
/*						int res = fgetc(stdin);
						if (res == 0x70) {
							break;
						}*/
					}
					printf("Select song\n");
	    			for (i = 0; i < number_of_songs; i++) {
    				printf("%d %s\n", i, songs[i]);
    				}
//    				int result = scanf("%d", &choice);
//              	  decode_thread_data_p->fp = 0;
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
                {
                		printf("%d\n", stop_flag);
                		sleep(1);
/*                        int res = fgetc(stdin);
                        if ((res == EOF) || (res == 0x71))
                        {
                                pthread_cancel(decode_thread_t);
								free(decode_thread_data_p);
								return;
                        }
						if (res == 0x70)
						{
							pause_play_flag = !pause_play_flag;
						}
						else if (res == 'b') {
							stop_flag = 1;
							pause_play_flag = PLAY;
							choice = (choice - 1);
							if (choice < 0) choice = number_of_songs - 1;
							break;
						}
						else if (res == 'n') {
							stop_flag = 1;
							pause_play_flag = PLAY;
							choice = (choice + 1) % number_of_songs;
							break;
						}
						else if (res == 0x73)
						{
							stop_flag = 1;
							pause_play_flag = PAUSE;
							break;
						}
						*/
                }
                
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
        
    return 0;
}

/*
int main(int argc, char *argv[]){

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
  

  data = mmap(NULL, s.st_size, PROT_READ, MAP_PRIVATE|MAP_FILE, fd, 0);
  if ((int)data == -1) {
    perror("mmap");
    close(fd);
    return 1;
  }

  read_tag(fd, data, s.st_size);
 
  bt_init();
  mad_decode(data, s.st_size);

  munmap(data, s.st_size);
  close(fd);

  close_handle();

  return 0;
}
*/