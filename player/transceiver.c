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

#define RECEIVE_PORT 5001
#define TRANSMIT_PORT 5000

void * sendInfo(void *);
void * receiveInfo(void *);

int main(int argc, char *argv[])
{
	pthread_t id[2];
	int runSend = 1;
	int runRec = 1;
	pthread_create(&id[0], NULL, sendInfo, (void *) &runSend);
	pthread_create(&id[1], NULL, receiveInfo, (void *) &runRec);
	
	while (1) {
	
		sleep(1000);
	}
	
}

void * sendInfo(void * arg) {
	int * run = (int *) arg;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
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

        ticks = time(NULL);
        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        write(connfd, sendBuff, strlen(sendBuff)); 
        printf("Sent packet\n");

        close(connfd);
        sleep(1);
     }
}

void * receiveInfo(void * arg) {
	int * run = (int *) arg;
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    char sendBuff[1025];
//    time_t ticks; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
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

  //      ticks = time(NULL);
//        snprintf(sendBuff, sizeof(sendBuff), "%.24s\r\n", ctime(&ticks));
        read(connfd, sendBuff, strlen(sendBuff)); 
        printf("Got Packet: %s\n", sendBuff);

        close(connfd);
        sleep(1);
     }
}


