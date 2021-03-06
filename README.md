# Simple-samples-of-C-with-explanations

Relearning C programming, algorithms and graphs, and creating simple documented example programs with explanations.  Caution: I am not an expert.

File list:

    bubbleSort.c
    Demonstrate the classic bubble sort algorithm on a list of 100 unique random
    words.
    
    i2c_stoplight.ino
    Slave I2C program for the Arduino to switch LED's on and off as instructed
    by the master I2C program running on the Raspberry Pi.
    
    i2c_sysfs_stoplight.c
    Master I2C program for the Raspberry to instruct the Arduion via i2c which
    LED's to turn on and off.  No extra libraries or header files needed.
    
    iLinkedList.c
    Demonstrate a linked list using integers.  The program was developed with the 
    vision of some day  developing into a binary tree.  So, integers are linked & 
    listed in descending order with larger numbers first and smaller numbers.  
    This will be important when this program becomes the core of a binary tree 
    program.  There is an identical program sLinkedList.c using strings.

    loadWordTable.c
    An example C program using the Maira/MySQL database and C api to demonstrate reading
    an ascii data file of words (1 word per line) and storing each word into the database.
    The program prompts the user for their MySQL username and password.  The MYSERVER
    #define statement should be updated to point to the correct MySQL server.

    loadWordTable.c is a companion program of readDBWords.c and readDBWordsSorted.c
    readDBWordsSorted and readDBWords are identical except for the format of the SELECT
    statement.

    maze1.csv
    Comma separated values data file of a maze for solving by rmaze.c  See the
    comments in rmaze.c
    
    mergeSort.c
    This program is a C implementation of the merge sort algorithm described on
    pages 82-83 in the text "Algorithms in a Nutshell" 2nd Edition by Heineman,
    Pollice, and Selkow.

    The program is limited to a maximum of 100 records.  No record can be
    longer than 9 characters (10 counting the ending null.)  This version of
    the program is also limited by the requirement that numeric data must be
    left padded with leading zeros.  Input data are ASCII characters.

    onehundrednumbers.txt
    A datafile of 100 numbers 1-100 in random order left padded with leading zeros.
    
    PaperDepthFirstMazeSolver.pdf
    I decided to develop a program to solve a maze then use LaTex to write a paper
    about what I learned.  I didn???t know at the time that this subject has been 
    done to death on the Web and YouTube.  This paper is not intended to be a real
    research paper but as a form of geeky play. I get to relearn some things I???ve 
    forgotten, practice programming, and play with the LaTeX software.
    
    ptrAdd.c
    Use only pointers to do array stuff.  Demonstrates allocating memory and pointers,
    reading string data into memory pointed to by pointer, pass pointer to a function,
    printing strings pointed to by pointer, and pointer addition.  Input data is string data.

    quicksort.c
    This program demonstrates the quick sort algorithm.  The quick sort is nearly identical to the
    merge sort in that the list to be sorted is repeatedly divided into smaller and smaller lists
    until there is nothing left to sort.  This makes it ideal for recursion.  Unlike merge sort
    the smaller lists do not need to be merged back together since the records are swapped in-place
    and there is no copying of lists.
    
    readDBWords.c
    See loadWordTable.c
    
    readDBWordsSorted.c
    See loadWordTable.c

    rmaze.c
    This program uses a recursive depth-first search algorithm to solve a maze.
    This C program was derived from a Python program written and demonstrated on YouTube
    by Richard West.

    selectionSort.c
    An working example of the selection sort.  It uses the wordsdata.txt file.

    sLinkedList.c
     Demonstrate a linked list using strings.  The program was developed with the vision of 
     some day developing into a binary tree.  So, strings are linked & listed in descending 
     order with larger ASCII character values first and lower ASCII characters values on 
     last.  (z-a) come before (Z-A).  This will be important when this program becomes the 
     core of a binary tree program.  There is an identical program sLinkedList.c using 
     strings.
    
    wordsdata.txt
    A datafile of 1 to 6 character unsorted words, all lower case, for use as an input file.
    
