#define Version "{bubbleSort (v1.0a)}"
/***************************************************************************************************
bubbleSort.c                                                                          18 August 2021

    This program is a demonstration of the classic bubble sort.  It will sort a list of 100 random
    words. An ASCII text file of 100 unique random words (wordsdata.txt) is located in the GitHub
    where this program was found.

    gcc bubbleSort.c
    ./a.out

    by Clyde Hoadley
****************************************************************************************************
    Procedure:

    Given a list of N words.
    1. for words 0 through N compare the N and N+1 words.
    2. If word N+1 is smaller than word N then swap the two words.
    3 repeat the above steps until no words are swapped.


****************************************************************************************************
                                GNU General Public License v3.0
    This program is free software: you can redistribute it and/or modify it under the terms of the
    GNU General Public License as published by the Free Software Foundation, either version 3 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
    without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
    the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with this program.
    If not, see <https://www.gnu.org/licenses/>.
***************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAXWORDS 100
#define MAXWORDLENGTH 20
#define FILENAME "wordsdata.txt"


/***********************************  Forward declarations  ***************************************/
void errorExit(char *string);
void readWordFile(char words[][MAXWORDLENGTH], int *listSize);
void bubbleSort(char words[][MAXWORDLENGTH],int listSize);


/**************************************************************************************************/
void errorExit(char *string) {
    fprintf(stderr,"\nERROR: %s\n%s\n",string,Version);
    exit(EXIT_FAILURE);
}


/**************************************************************************************************/
void readWordFile(char words[][MAXWORDLENGTH], int *listSize) {
    FILE *fp=NULL;
    int i=0, l=0;

    fp=fopen(FILENAME, "r");
    if ( !fp ) errorExit("No such file");

    while (1){
        bzero(words[i],MAXWORDLENGTH);
        fgets(words[i],MAXWORDLENGTH,fp);
        if (feof(fp)) break;
        l=strlen(words[i]);
        if ( !l ) break;
        if ( (words[i][l-1] < ' ') || (words[i][l-1] > '~') ) words[i][l-1]=0; // remove LF/CR
        if ( (words[i][l-2] < ' ') || (words[i][l-2] > '~') ) words[i][l-2]=0;
        i++;
    }//end-while

    *listSize = i;
    fclose(fp);
    return;
}


/**************************************************************************************************/
void bubbleSort(char words[][MAXWORDLENGTH],int listSize) {
    char temp[MAXWORDLENGTH];
    bool switched = false;
    int i=0;

    while ( 1 ) {
        switched = false;
        for (i=0; i<listSize-1; i++) {
            if (strncmp(words[i],words[i+1], MAXWORDLENGTH) > 0) { //compare words N and N+1
                strncpy(temp,words[i+1],MAXWORDLENGTH);            //word N+1 is smaller than word N
                strncpy(words[i+1],words[i],MAXWORDLENGTH);
                strncpy(words[i],temp,MAXWORDLENGTH);
                switched = true;
            }
        }
        if ( ! switched ) break;  //if no words were swapped then the list is fully sorted.
    }//end-while
    return;
}//end-bubbleSort()


/*************************************************************************************************/
int main(int argc, char **argv) {
    char words[MAXWORDS][MAXWORDLENGTH];
    int listSize = -1;

    readWordFile(words, &listSize);
    bubbleSort(words,listSize);
    for(int i=0; i<listSize; i++) {
        printf("%s\n",words[i]);
    }
    exit(EXIT_SUCCESS);
}