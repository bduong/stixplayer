/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief Retrieve the file names of all mp3 files in the given directory.
*/

#include <dirent.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include "playlist.h"

char * songs[20];
int number_of_songs;

/**
* @ brief Scan a directory and pull out all file names that contain the .mp3 extension.
*/
int getMP3Files(char * directory)
{
    DIR           *d;
    struct dirent *dir;
    int count = 0;
    d = opendir(directory);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
          	if (strstr(dir->d_name, ".mp3") != NULL) {
    			char * str = malloc(40 * sizeof(char));    			        	
	          	strncpy(str,dir->d_name, 40);
	          	songs[count++] = str;	          	
            }
        }
        number_of_songs = count;

    closedir(d);
    }

    return(0);
}
