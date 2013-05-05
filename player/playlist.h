/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief Retrieve the file names of all mp3 files in the given directory.
*/
#ifndef _PLAYLIST_H_
#define _PLAYLIST_H_

/**
* Array holding the song file names
*/
extern char * songs[20];

/**
* Total number of songs found.
*/
extern int number_of_songs;

/**
* The index of the song to play.
*/
extern int song_choice;

int getMP3Files(char * directory);
#endif

