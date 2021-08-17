#define Version "{selectionSort (v1.0a)}"
/***************************************************************************************************
selectionSort.c                                                                       16 August 2021

    This program is a demonstration of the selection sort.  It will sort a list of 100 random words.
    Selection sort is considered to be a greedy algorithm; on a good day its performance is O(n^2).

    gcc selectionSort.c
    ./a.out

    A ASCII text file of 100 unique random words (wordsdata.txt) is located in the GitHub where this
    program was found.

    by Clyde Hoadley
****************************************************************************************************
    Procedure:

    Given a list of 100 random words (or numbers), begin at the head of the list and proceed as
    follows until the list is fully sorted.

    1) Remember the index of the current location within the list.
    2) scan down the list from the current location to the end of the list and select the
       lexically smallest word (or number.)
    3) shift all items in the list down from the current location (step 1 above) through to the
       location of the selected word.  This fills the hole left by the selected word and opens
       up the current location (step 1 above.)
    4) copy the selected word into the current location (step 1 above.) and advance the current
       location to the next item in the list.  The selected word is now in its correct location.

    Robert Sedgewick. Algorithms in C. Addison-Wesley/Pearson Educationy, 1990, page 96.

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


#define MAXWORDS 100
#define MAXWORDLENGTH 20
#define FILENAME "wordsdata.txt"


/***********************************  Forward declarations  ***************************************/
void errorExit(char *string);
void findSmallestWord(char words[][MAXWORDLENGTH], char *smallestWord, int *listSize,
                      int *indexSmallestWord, int *nextPosition);
void moveListDown(char words[][MAXWORDLENGTH], int *indexSmallestWord, int *nextPosition);
void putSelectedWord(char words[][MAXWORDLENGTH], char *smallestWord, int *indexSmallestWord, int *nextPosition);


/*************************************************************************************************/
void errorExit(char *string) {
    fprintf(stderr,"\nERROR: %s\n%s\n",string,Version);
    exit(EXIT_FAILURE);
}


/*************************************************************************************************/
// When passing a multidimensional array to a function (here it's an array of strings,) the
// compiler factors the first dimension down to a pointer to an array with fixed size of the
// second dimension.  The function declaration must specify every dimension of the array except
// for the fist dimension.
void findSmallestWord(char words[][MAXWORDLENGTH], char *smallestWord, int *listSize,
                      int *indexSmallestWord, int *nextPosition) {

    strcpy(smallestWord,"zzzzzzzzzzzzzzzzzz");
    for (int i=*nextPosition; i<*listSize; i++) {
      if ( strncmp(words[i],smallestWord,MAXWORDLENGTH) < 1) {
        *indexSmallestWord=i;
        strncpy(smallestWord,words[i],MAXWORDLENGTH);
      }
    }//end-for
}//end-isSmallestWord()

/*************************************************************************************************/
void moveListDown(char words[][MAXWORDLENGTH], int *indexSmallestWord, int *nextPosition) {

    for (int i=*indexSmallestWord; i>*nextPosition; i--) {
        strncpy(words[i],words[i-1],MAXWORDLENGTH);
    }
} //end-moveListDown()

/*************************************************************************************************/
void putSelectedWord(char words[][MAXWORDLENGTH], char *smallestWord, int *indexSmallestWord, int *nextPosition) {

    strncpy(words[*nextPosition],smallestWord,MAXWORDLENGTH);
    (*nextPosition)++;
    smallestWord[0]=0;
    *indexSmallestWord=-1;
}//end-putSelectedWord();


/*************************************************************************************************/
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


/*************************************************************************************************/
int main(int argc, char **argv) {
    char words[MAXWORDS][MAXWORDLENGTH];
    char smallestWord[MAXWORDLENGTH];
    int indexSmallestWord= -1;
    int listSize = -1;
    int nextPosition=0;

    //All arguments to the function calls are being passed by reference (pointers) rather than by
    //value.  This is necessary so the value of the argument retains the value set by the function
    //when it returns.
    readWordFile(words, &listSize);
    while (nextPosition < listSize) {
        findSmallestWord(words, smallestWord, &listSize, &indexSmallestWord, &nextPosition);
        moveListDown(words, &indexSmallestWord, &nextPosition);
        putSelectedWord(words, smallestWord, &indexSmallestWord, &nextPosition);
    }

    for(int i=0; i<listSize; i++) {
        printf("%s\n",words[i]);
    }
    exit(EXIT_SUCCESS);
}