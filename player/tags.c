/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief Extract the ID3 tag information from an MP3 file.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <id3tag.h>
#include <linux/limits.h>
#include <assert.h>

#include "tags.h"

char *id3_get_tag (struct id3_tag const *tag, char const *what, unsigned int maxlen);
char song_title[40];
char song_artist[40];

static int show_id3(struct id3_tag const *tag)
{
    unsigned int i;
    int print = 0;
    char emptystring[31];
    char *names[6];
	
    struct {
        int index;
        char const *id;
        char const *name;
    } 
	const info[] = {
        { 0,    ID3_FRAME_TITLE,  "Title  : "   },
        { 1,    ID3_FRAME_ARTIST, "  Artist: "  },
        { 2,    ID3_FRAME_ALBUM,  "Album  : "   },
        { 3,    ID3_FRAME_YEAR,   "  Year  : "  },
        { 4,    ID3_FRAME_COMMENT,"Comment: "   },
        { 5,    ID3_FRAME_GENRE,  "  Genre : "  }
    };

    memset (emptystring, ' ', 30);
    emptystring[30] = '\0';
    /*  Get ID3 tag if available  */
    for (i=0; i<=5; i++)    {
        names[i] = NULL;
        names[i] = id3_get_tag(tag, info[i].id, (i==3) ? 4 : 30);
    }
    for (i=0; i<=5; i++)    {
        if (names[i])   {
            print = 1;
            break;
        }
    }
    if (!print) {
        return 0;
    }

	if (names[0] == NULL)		
		strcpy( song_title, " ");
	else		
		strncpy( song_title, names[0], 20);
	if (names[1] == NULL)
		strcpy( song_artist, " ");
	else		
		strncpy( song_artist, names[1], 20);
	
	for (i=0; i<=5; i++)    {
		fprintf (stderr, "%s", info[i].name);
		if (!names[i])  {
			fprintf (stderr, emptystring);
		}   else    {
			fprintf (stderr, "%s", names[i]);
			free (names[i]);
		}
		if (i%2) fprintf (stderr, "\n");
	}		

    return 1;
}

int read_tag( int fd, unsigned char *data, unsigned long length){
	struct id3_file *id3struct = NULL;
	struct id3_tag *id3tag = NULL;
	struct id3_tag *id3tag2 = NULL;
	signed long id3len, id3len2;
	unsigned long id3off = 0;

	id3struct = id3_file_fdopen (fd, ID3_FILE_MODE_READONLY);

	id3len = id3_tag_query(data, length);

	id3off = length - 128;
	id3len2 = id3_tag_query(data + id3off, 128);

	if (id3struct)
	{
		id3tag = id3_tag_parse (data, id3len);
		id3tag2 = id3_tag_parse (data + id3off, id3len2); 
		  
		if (id3tag2) //print out id3v1 if it exist
		{
		show_id3 (id3tag2);
		}
		else if (id3tag)  //print out id3v2 if it exist
		{
			show_id3 (id3tag);
		}
		id3_file_close (id3struct);
	}

	close(fd);
}

char *id3_get_tag (struct id3_tag const *tag, char const *what, unsigned int maxlen)
{
    struct id3_frame const *frame = NULL;
    union id3_field const *field = NULL;
    int nstrings;
    int avail;
    int j;
    int tocopy;
    int len;
    char printable [1024];
    char *retval = NULL;
    id3_ucs4_t const *ucs4 = NULL;
    id3_latin1_t *latin1 = NULL;

    memset (printable, '\0', 1024);
    avail = 1024;
    if (strcmp (what, ID3_FRAME_COMMENT) == 0)
    {
        j=0;
        frame = id3_tag_findframe(tag, ID3_FRAME_COMMENT, j++);
        if (!frame) return (NULL);
        ucs4 = id3_field_getfullstring (&frame->fields[3]);
        if (!ucs4) return (NULL);
        latin1 = id3_ucs4_latin1duplicate (ucs4);
        if (!latin1 || strlen(latin1) == 0) return (NULL);
        len = strlen(latin1);
        if (avail > len)
            tocopy = len;
        else
            tocopy = 0;
        if (!tocopy) return (NULL);
		avail-=tocopy;
        strncat (printable, latin1, tocopy);
        free (latin1);
    }
    
    else
    {
        frame = id3_tag_findframe (tag, what, 0);
        if (!frame) return (NULL);
        field = &frame->fields[1];
        nstrings = id3_field_getnstrings(field);
        for (j=0; j<nstrings; ++j)
        {
            ucs4 = id3_field_getstrings(field, j);
            if (!ucs4) return (NULL);
            if (strcmp (what, ID3_FRAME_GENRE) == 0)
                ucs4 = id3_genre_name(ucs4);
            latin1 = id3_ucs4_latin1duplicate(ucs4);
            if (!latin1) break;
            len = strlen(latin1);
            if (avail > len)
                tocopy = len;
            else
                tocopy = 0;
            if (!tocopy) break;
            avail-=tocopy;
            strncat (printable, latin1, tocopy);
            free (latin1);
        }
    }
    retval = malloc (maxlen + 1);
    if (!retval) return (NULL);

    strncpy (retval, printable, maxlen);
    retval[maxlen] = '\0';

    len = strlen(printable);
    if (maxlen > len)
    {
        memset (retval + len, ' ', maxlen - len);
    }

    return (retval);
}
