#define Version "quicksort (v1.0a)"
/**************************************************************************************************
quicksort.c                                                                          31 August 2021

    This program demonstrates the quick sort algorithm.  The quick sort is nearly identical to the
    merge sort in that the list to be sorted is repeatedly divided into smaller and smaller lists
    until there is nothing left to sort.  This makes it ideal for recursion.  Unlike merge sort
    the smaller lists to not need to be merged back together since the records are swapped in-place
    and there is no copying of lists.

    A value is arbitrarily selected from the list to be used as a pivot point.  The list is
    rearranged so that everything to the left (or above) the pivot is smaller than the pivot
    and everything to the right (or below) the pivot is larger than the pivot.  In reality the
    last item in the list is frequently used for the pivot - this is fine since it is the value
    (numeric or string) the chosen pivot has not its location.  The pivot might get relocated
    during the sort, but the location where the pivot was when it was chosen will be the
    location where last element (record) of the sort will be placed.  For very large data sets
    it may be useful to refine the way the pivot is chosen.

    Once a pivot has been selected, scan the list from left to right until a value greater than the
    pivot is found.  Scan the list from right to left until a value smaller than the pivot is found.
    These two items must be swapped.  Then the scanning resumes from where it had stopped. Scanning
    stops when the left and right indices cross each other.  Once the indices pass each other the
    right most element and the value at left index (the index that started scanning from the left
    end) must be swapped.

    There are now 2 lists.  The left list extends from the left end of the list to the left scanning
    index (the index that started its can from the left end,) minus 1.  The right list extends from
    the left scanning index plus 1 to the right end of the list.  These 2 lists can be sorted
    recursively by passing each list into quick sort again.  The recursion stops when quick sort is
    given a left index that is greater than or equal to its right index.

    For further reading see: Robert Sedgewick. Algorithms in C. Addison-Wesley, 1990, pp. 115-120.

        cc quicksort.c
        ./a.out

    by Clyde Hoadley.
***************************************************************************************************
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
**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define listSize 20

// A macro is a rule or pattern that specifies how a certain input should
// be mapped to a replacement output.
#define SWAP(t,x,y) t=x; x=y; y=t;       // This is a macro

int someList[listSize] = {4,15,3,7,14,16,20,9,1,5,10,19,11,18,12,17,13,8,2,6};


/*************************************************************************************************
This function is not necessary.  I include it just for fun.  In main() the program is told to
automatically call this function upon normal exit. It will not be called if the program crashes.*/
void goodBye(void) {
   printf("\nThank you.  Come again.\n%s\n",Version);
}


/*************************************************************************************************/
void qwsort(int unsortedlist[], int left, int right) {
    int pivot, lf, rt, tmp;

    if(right > left) {
        pivot = unsortedlist[right]; //for big lists might pick pivot differently
        lf = left - 1;
        rt = right;

        while (1) {
            while (unsortedlist[ ++lf ] < pivot) ; //scan left to right til find >= pivot value
            while (unsortedlist[ --rt ] > pivot) ; //scan right to left til find <= pivot value
            if (lf >= rt) break;
            SWAP(tmp,unsortedlist[lf],unsortedlist[rt]);  //This macro is #defined above
        }//end-while(1)

        SWAP(tmp,unsortedlist[lf],unsortedlist[right]);//swap right most element with value at left scan

                                            //Recursion is when a function calls itself.
        qwsort(unsortedlist, left, lf - 1); //unsortedlist[0] to unsortedlist[left_scan_index_minus_1]
        qwsort(unsortedlist, lf + 1, right);//unsortedlist[left_scan_index_plus_1] to unsortedlist[listSize]
    }//end-if
}//end-qwsort()


/*************************************************************************************************/
int main(int argc, char **argv) {
    atexit(goodBye);

    qwsort(someList, 0, listSize-1);   //Remember indices go from zero to list size minus one.

    for (int k=0; k<listSize; k++)
        printf("%d\n",someList[k]);

    exit(EXIT_SUCCESS);
}