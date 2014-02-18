/* Example code for Think OS.

Copyright 2014 Allen Downey
License: GNU GPLv3

*/

#include <stdio.h>
#include <stdlib.h>

int global; //a global variable (in read-only mem)

void myfunc(void){
	int mylocal = 2;
	printf("Address of mylocal is %p\n", &mylocal);
}

void mymalloc(void){
	void *mypointer1 = malloc(1);
	void *mypointer2 = malloc(30);
	void *mypointer3 = malloc(5);
	printf("Address of mypointer1 is %p\n", mypointer1);
	printf("Address of mypointer2 is %p\n", mypointer2);
	printf("Address of mypointer3 is %p\n", mypointer3);
}

int main ()
{
    int local = 5; // a local variable
    void *p = malloc(128); //a pointer to the beginning of a 128 byte piece of memory
    printf ("Address of main is %p\n", main);
    printf ("Address of global is %p\n", &global);
    printf ("Address of local is %p\n", &local);
    myfunc();
    mymalloc();
    printf ("Address of p is %p\n", p);
    
    return 0;
}
