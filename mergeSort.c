#define Version "mergeSort (v1.0a)"
/******************************************************************************
mergeSort.c                                                         7-July-2021

    This program is a C implementation of the merge sort algorithm described on
    pages 82-83 in the text "Algorithms in a Nutshell" 2nd Edition by Heineman,
    Pollice, and Selkow.

    The program is limited to a maximum of 100 records.  No record can be
    longer than 9 characters (10 counting the ending null.)  This version of
    the program is also limited by the requirement that numeric data must be
    left padded with leading zeros.  Input data are ASCII characters.

    cc mergeSort.c
    ./a.out < datafile.txt

    by Clyde Hoadley July 7, 2021.
*******************************************************************************

    Merge sort algorithm pseudocode from page 82.

    Sort(A)
        Copy = Copy of A
        mergesSort(Copy, A, 0, n)
    end

    mergeSort (A, result, start, end)
        if end - start < 2 then return
        if end - start = 2 then
            swap end and start if they are out of order
            return

        mid = (start + end)/2
        mergeSort(result, A, start, mid)
        mergeSort(result, A, mid, end)

        merge left and right halves of A into result
    end
*******************************************************************************
                    GNU General Public License v3.0
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
******************************************************************************/

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>

#define MAXINPUT 100
#define MAXLENGTH 10



int readList(char A[][MAXLENGTH]) {
// everyone says to stop using gets and to use fgets instead.  Well ok, but fgets brings in the newline
// character if there is one so I have to null the newline out.
// readList() fills the array A[] with data sent in via stdin.  The function returns a count of how many
// records were read (how many slots of the array are filled.)

    int count=0, l=0;
    for (int i=0; i < MAXINPUT; i++,count++) {
        if ( fgets(A[i], MAXLENGTH+1, stdin) ) {  //fgets subtracts 1 from the middle parameter but I want all 10 chars
            l=strlen(A[i]) - 1;  // find the ending null and back off by 1.
            if (A[i][l] == '\n') A[i][l]='\0'; // if the last character is a newline replace it with a null.
        } else {
            return (count);  // fgets returns a null when at the end of file so we can exit the loop early.
        } //end-if fgets()
    } //end-for

    return (count);

} //end-readInput()



void outputList(char arrayA[][MAXLENGTH], int x) {
// function not really needed - just keeping main() nice and neat
    for (int i=0; i < x; i++)
        printf("%s\n", arrayA[i]);
}



void copyArray(char toList[][MAXLENGTH], char fromList[][MAXLENGTH],int max) {
// copy the right array into the left array.
// function not really needed - just keeping main() nice and neat
    for (int i=0; i < max; i++)
        strcpy(toList[i],fromList[i]);
} //end-copyArray()



void mergeHalves(char arrayA[][MAXLENGTH], char result[][MAXLENGTH],int start, int mid, int end) {
// arrayA[] has been divided into 2 sublists.  We will merge the 2 sublists in sorted order
// into the result[] array.

  int L = start;  // L is the head of the Left sublist
  int R = mid;    // R is the head of the Right sublist
  int B = start;  // B is the head of the combined sorted result list

  while (B < end) {  // don't let the result list go past the end of space/time.

    // if R has reached the end of its list than anything remaining will be in the left list.
    // mid is the head of the right list so don't let L run over mid.
    // if there is anything remaining in the right list then see if left string is < right string.
    // if the right list has exhausted but there is still data in the left list then copy it into result list.
    // if there is still data in both lists then compare and copy the smaller into result.
    if ( ( R >= end )   ||   ( (L < mid ) && ( strcmp(arrayA[L],arrayA[R]) < 1 ) ) ) {

      strcpy(result[B],arrayA[L]);  //the left string was smaller or equal so copy it into the result list
      ++L;  // advance the index of the left list to its next string

    } else {

        strcpy(result[B],arrayA[R]); //the right string was smaller so copy it into the result list
        ++R;  // advance the index of the right list to its next string
    }

    ++B;  // now advance the index of the result list to its next slot.

  } //end-while ... repeat until we run out of lists.

} // end-mergeHalves()



void mergeSort(char arrayA[][MAXLENGTH], char result[][MAXLENGTH], int start, int end) {
// merge sort works by dividing the list to be sorted in half then repeatedly calling itself (recursion)
// until the list can not be divided any further (the 2nd IF below.)  Each half list is sorted separately
// then merged back together

    if (end - start < 2) { // if the indices are less than 2 (i.e. 0 or 1) then stop the recursion and return.
        return;
    }
    if ( (end - start) == 2) { // if near the end then compare the last 2 strings
        if ( strcmp(result[start], result[start + 1]) > 0 ) {  // swap them if 1st string is bigger than the 2nd
            char temp[MAXLENGTH];
            strcpy(temp, result[start + 1]);
            strcpy(result[start + 1],result[start]);
            strcpy(result[start],temp);
            return;
        } //end-if strcmp
    }

    int mid = (end + start) / 2;  // divide the list in half
    mergeSort(result, arrayA, start, mid);  // sort the left sublist (a function calling itself is recursive.)
    mergeSort(result, arrayA, mid, end);  // sort the right sublist
    mergeHalves(arrayA,result,start,mid,end);  // combine the 2 sublists back together in sorted order.

    return; // mergeSort will return to its previous call, if any, until all sublists have been sorted and
            // recombined (mergeHalves.)  The final return sends us back to main().

}  //end-mergesort()


int main (int argc, char *argv[]) {

    time_t timeStart, timeFinish;
    double duration;
    int max=0;
    char Alist[MAXINPUT][MAXLENGTH];
    char Blist[MAXINPUT][MAXLENGTH];

    timeStart = time(0);
    max=readList(Alist);
    copyArray(Blist,Alist,max);

    mergeSort(Blist,Alist,0, max);  // Merge Sort, it's magic!

    outputList(Alist,max);
//for(int n=0; n < INT_MAX; n++) {};  // this delay has been commented out.
    timeFinish = time(0);
    duration = difftime(timeFinish, timeStart);
    printf("%s finished in %.0lf seconds.\n", Version, duration);
    return (0);

} //end-main()
