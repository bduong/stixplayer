/*
    * This program displays the names of all files in the current directory.
     */

#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
int main(int argc, char * argv[])
{
      DIR           *d;
      struct dirent *dir;
      d = opendir(argv[1]);
      if (d)
      {
          while ((dir = readdir(d)) != NULL)
          {
          	if (strstr(dir->d_name, ".mp3") != NULL) {
            	printf("%s\n", dir->d_name);
            }
          }

      closedir(d);
      }

      return(0);
}
