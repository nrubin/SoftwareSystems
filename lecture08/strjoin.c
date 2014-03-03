/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *tracks[] = {
    "So What",
    "Freddie Freeloader",
    "Blue in Green",
    "All Blues",
    "Flamenco Sketches"
};

/* Returns a heap-allocated string that contains the strings 
   from the given array, joined up with no spaces between.
*/
char *strjoin(char *array[], int n)
{
    // TODO: fill this in
    // first get the size of the array
    // then strcat the whole way down!
    int i = 0;
    int size = 0;
    int offset = 0;
    char *prev_buff = malloc(1);
    strcpy(prev_buff,"");
    // printf("The array is this long: %i\n", num_strs );
    for (i = 1; i < n; i++)
    {
        size = size + strlen(array[i]);
    }
    char *result = (char *) malloc(size + 1);
    result[0] = '\0';
    for (i = 0; i < n; i++)
    {
        strcpy(result + offset,tracks[i]);
        offset += strlen(tracks[i]) * sizeof(char);
    }
    return result;
}

int main (int argc, char *argv[])
{
    char *s = strjoin(tracks, 5);
    printf("%s\n", s);
    return 0;
}
