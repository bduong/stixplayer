#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "player.h"
#include "tags.h"

int main(int argc, char* argv[])
{
    pthread_t decode_thread_t;
    
    void* decode_thread_exit_p;
    
    decode_thread_data_t* decode_thread_data_p = (decode_thread_data_t*)malloc(sizeof(decode_thread_data_t));
    
    if (argc > 1)
    {
    
//                decode_thread_data_p->fp = 0;
                decode_thread_data_p->filename = argv[1];
//                decode_thread_data_p->data = NULL;

                printf("Press q to quit");
          
                /* init of decode thread */
                if (pthread_create(&decode_thread_t,NULL,mad_decode,decode_thread_data_p) != 0)
                {
                        fprintf(stderr, "Error creating decode thread!\n");
                        return 1;
                }

                /* main loop */
                while (1)
                {
                        int res = fgetc(stdin);
                        if ((res == EOF) || (res == 0x71))
                        {
                                /* perform clean-up */
                                pthread_cancel(decode_thread_t);
                                break;
                        }
                }
                
                printf( "waiting for Decode thread to exit...\n");
                pthread_join(decode_thread_t, &decode_thread_exit_p);
                if (decode_thread_exit_p != PTHREAD_CANCELED)
                {
                        //printf("Decode return value: %x\n", pDecodeThreadExit);
                }
                else
                {
                        printf("Decode thread was cancelled.\n");
                }
        
        }
        else
        {
    		fprintf(stderr, "Usage: ./madtest mp3file\n");
        }       
        
        free(decode_thread_data_p);
        
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
