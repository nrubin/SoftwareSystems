/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define NUM_TRACKS 5

char tracks[][80] =
{
    "So What",
    "Freddie Freeloader",
    "Blue in Green",
    "All Blues",
    "Flamenco Sketches"
};


// Finds all tracks that contain the given string.
//
// Prints track number and title.
void find_track(char search_for[])
{
    int i;
    for (i = 0; i < NUM_TRACKS; i++)
    {
        if (strstr(tracks[i], search_for))
        {
            printf("Track %i: '%s'\n", i, tracks[i]);
        }
    }
}

// Finds all tracks that match the given pattern.
//
// Prints track number and title.
//
// Based on code found in http://pubs.opengroup.org/onlinepubs/7908799/xsh/regexec.html
void find_track_regex(char pattern[])
{
    int comp_status, exec_status, i;
    regex_t re;
    comp_status = regcomp(&re, pattern, 0); 
    if (comp_status != 0)
    {
        printf("Could not compile the regex!\n");
        return;
    }
    for (i = 0; i < NUM_TRACKS; i++)
    {
        exec_status = regexec(&re, tracks[i], (size_t) 0, NULL, 0);
        if (exec_status == 0)
        {
            printf("Track %i: '%s'\n", i, tracks[i]);
        }
    }

}

// Truncates the string at the first newline, if there is one.
void rstrip(char s[])
{
    //*ptr points to the location in s where the first \n  occurs
    char *ptr = strchr(s, '\n');
    //if \n occurs, replace it with a null terminator to end the string
    if (ptr)
    {
        *ptr = '\0';
    }
}

int main (int argc, char *argv[])
{
    char search_for[80];

    /* take input from the user and search */
    printf("Search for: ");
    fgets(search_for, 80, stdin); //read 80 characters from stdin and store them in search_for

    rstrip(search_for); //strip the trailing line break from the string

    // find_track(search_for);
    find_track_regex(search_for);

    return 0;
}

