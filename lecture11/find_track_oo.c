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


typedef struct
{
    regex_t inner_struct[1];
} Regex;


// Returns a new Regex that matches the given pattern.
// pattern: string regex
// flags: flags passed to regcomp
// returns: new Regex
Regex *make_regex(char *pattern, int flags)
{
    Regex *my_regex = malloc(sizeof(Regex));
    if (my_regex == NULL)
    {
        fprintf(stderr, "Malloc failed\n");
        exit(1);
    }
    int err = regcomp(my_regex->inner_struct, pattern, flags);
    if (err)
    {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }
    return my_regex;
}

// Checks whether a regex matches a string.
// regex: Regex
// s: string
// returns: 1 if there's a match, 0 otherwise
int regex_match(Regex *regex, char *s)
{
    char msgbuf[100];
    int return_code = regexec(regex->inner_struct, s, 0, NULL, 0);
    if (!return_code)
    {
        return 1;
    }
    else if (return_code == REG_NOMATCH)
    {
        return 0;
    }
    else
    {
        regerror(return_code, regex->inner_struct, msgbuf, sizeof(msgbuf));
        fprintf(stderr, "Regex match failed: %s\n", msgbuf);
        exit(1);
    }
    return 0;
}

// Frees a Regex.
// regex: Regex
void regex_free(Regex *regex)
{
    regfree(regex->inner_struct);
    free(regex);
}


// Finds all tracks that match the given pattern.
//
// Prints track number and title.
void find_track_regex(char pattern[])
{
    int i;

    Regex *regex = make_regex(pattern, REG_EXTENDED | REG_NOSUB);

    for (i = 0; i < NUM_TRACKS; i++)
    {
        if (regex_match(regex, tracks[i]))
        {
            printf("Track %i: '%s'\n", i, tracks[i]);
        }
    }

    regex_free(regex);
}


int main (int argc, char *argv[])
{
    char *pattern = "F.*F.*";
    find_track_regex(pattern);

    return 0;
}
