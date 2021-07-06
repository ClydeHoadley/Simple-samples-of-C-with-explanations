#define Version "(v1.1.0)"
/******************************************************************************************************
ptrAdd.c                                                                                    6-July-2021

    Use only pointers to do array stuff.  Demonstrates allocating memory and pointers,
    reading string data into memory pointed to by pointer, pass pointer to a function,
    printing strings pointed to by pointer, and pointer addition.  Input data is string data.

    cc ptrAdd.c
    ./a.out  < wordsdata.txt

    datafile.txt should contain ascii characters where each record (row) has no more than 5 characters.
    The datafile can have fewer than 100 rows but no more than 100 rows (0-99) will be read.
    Caution: this program does not do input checking.  A row with more than 9 characters has the
    potential to cause a buffer overflow and corrupt the data in memory (though not guaranteed.)

    by Clyde Hoadley July 5, 2021.
******************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#define MAXINPUT 100
#define MAXLENGTH 10

void output(char *ptr, int x, int amount);  //forward declaration.  Function will be defined after main.



int main(void) {
    char *ptr, *baseptr;  //create a pair of character pointers.
    int i=0, count=0;

    baseptr = NULL; //always NULL unused pointers so they can't be used to access freed memory.
    ptr = NULL;

            // allocate memory for a 2 dimensional array of size MAXINPUT X MAXLENGTH
            // store the pointer to the allocated memory in baseptr.
    baseptr = calloc((size_t)MAXINPUT, (size_t)MAXLENGTH);
    if (baseptr == NULL) {
        printf("\nMemory allocation error.\n");
        exit(1); //tell shell that program failed.
    } // end-if NULL

    ptr = baseptr;  // keep baseptr safe but ptr will change.

    for (i=0; i<MAXINPUT; i++,count++) {  //read in data from standard input
                //             Only using *ptr will bring in a single character.
                //             By using (char *)ptr the whole string will be brought in.
        if (scanf("%s",(char *)ptr) == EOF) break;  // read a string into 1 row of the array
        ptr = ptr + MAXLENGTH;  // advance the pointer by the size of 1 row.
    }//end-for

    ptr = baseptr;  // reset ptr because were going to use it again.
    output(ptr, MAXLENGTH, count);  //print the array to standard out.

    free(baseptr);  //always release the memory when no longer needed.
    baseptr = NULL;  //always NULL unused pointers so they can't be used to access freed memory.
    ptr = NULL;

    exit(0);  //signal success to the shell
} //end-main()



void output(char *xptr, int x, int amount) {
//  char *ptr points to the start of the allocated memory (i.e.  array[0][0] )
//  Could have defined char *ptr but want to demonstrate that this is a different pointer than in main.
//  Int x is the size (width) of 1 row of the array; his is how much needs to be
//  added to the pointer to advance it to the next row.

    for (int i=0; i<amount; i++) {    //print the array to standard out.
            //             *xptr is pointing to the first character in the string.
            //             (char *)xptr is pointing to the string.
       printf("row %d = %s\n",i,(char *)xptr);
       xptr += x; // advance the pointer by the size passed in via x.
                  // This syntax is the same as xptr = xptr + x;
    }//end-for
} //end-output()
