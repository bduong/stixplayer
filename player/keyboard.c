/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief Capture user input from the keyboard
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "keyboard.h"
#include "player.h"
#include "playlist.h"


/**
* @brief Captures the user input from standard input.
*/
void *keyboard(void * arg)
{
	while (1)
	{
		int res = fgetc(stdin);
		if (res == 'p') //pause/play
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
		else if (res == 's') //stop
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
