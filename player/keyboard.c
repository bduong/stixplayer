#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "keyboard.h"
#include "player.h"
#include "playlist.h"

void *keyboard(void * arg)
{
	while (1)
	{
		int res = fgetc(stdin);
		if (res == 0x70) //pause/play
		{
			pause_play_flag = !pause_play_flag;
		}
		else if (res == 'b') { //back
			stop_flag = STOP;
			pause_play_flag = PLAY;
			song_choice = (song_choice - 1);
			if (song_choice < 0) song_choice = number_of_songs - 1;	
		}
		else if (res == 'n') { //next
			stop_flag = STOP;
			pause_play_flag = PLAY;
			song_choice = (song_choice + 1) % number_of_songs;		
		}
		else if (res == 0x73) //stop
		{
			stop_flag=STOP;
			pause_play_flag = PAUSE;
		} 
		else if (res == 'q') { //quit
			total_stop = 1;
			stop_flag = STOP;
			pause_play_flag;
		}
	}
}
