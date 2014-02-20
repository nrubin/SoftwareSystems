/* Example code for Software Systems at Olin College.

Copyright 2014 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Returns a heap-allocated string with length n, with
// the given string in the middle, surrounded by fillchar.
//
// For example, if s is allen, n is 10 and fillchar is .
// the result is ..allen...

char *center(char *s, int n, char fillchar)
{
    int middleword, middleresult, wordbegin, wordlen, i;
    char *result = (char *) malloc(n + 1);
    wordlen = strlen(s);
    if (wordlen % 2 == 0)
    {
        middleword = wordlen / 2 - 1;
    }
    else
    {
        middleword = wordlen / 2;
    }
    if (wordlen % 2 == 0)
    {
        middleresult = n / 2 - 1;
    }
    else
    {
        middleresult = n / 2;
    }

    wordbegin = middleresult - middleword;

    for (i = 0; i < wordbegin; ++i)
    {
        result[i] = fillchar;
    }
    for (i = wordbegin; i < wordbegin + wordlen; ++i)
    {
        result[i] = s[i - wordbegin];
    }
    for (i = wordbegin + wordlen; i < n; ++i)
    {
        result[i] = fillchar;
    }
    
    return result;
}


int main (int argc, char *argv[])
{
    //test case
    // char *s0 = center("allen", 9, '.');
    // printf("%s\n", s0);

    char *s = center("Software", 30, '.');
    printf("%s\n", s);

    char *s2 = center("Systems", 31, '-');
    printf("%s\n", s2);

    char *s3 = center("Spring 2014", 32, '*');
    printf("%s\n", s3);

    char *s4 = center("Exam 1", 33, '^');
    printf("%s\n", s4);

    return 0;
}
