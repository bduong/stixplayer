/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief Extract the ID3 tag information from an MP3 file.
*/
#ifndef _TAG_H_
#define _TAG_H_
int read_tag( int fd, unsigned char *data, unsigned long length);

/**
* The title of the currently playing song.
*/

/**
*  The artist of the currently playing song.
*/
extern char song_title[40];
extern char song_artist[40];
#endif

