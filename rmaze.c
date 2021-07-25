#define Version "rmaze (v2.0b)"
/******************************************************************************
rmaze.c                                                            24-July-2021

    This program uses a recursive depth-first search algorithm to solve a maze.
    This C program was derived from a Python program written and
    demonstrated on YouTube by Richard West.  His video can be found here:
    https://www.youtube.com/watch?v=XP94WC_XnZc  and his website can be found
    here: https://learnlearn.uk/about/

    I took his Python program and rewrote it in C.  Having done that I then
    used LaTeX to write a paper describing haw the depth-first search algorithm
    and this program work.

    The sole purpose of this project was to play with the LaTeX typesetting
    software, practice programming, and relearn some forgotten theory.

    The C source code is well documented and meaningful variable names and
    function names have been used.

    This program is not a graphics program.  It displays the maze and its
    progress on the screen of an ordinary ANSI terminal that recognizes
    ordinary ANSI escape codes.  Mac has such a terminal to access the Linux
    command line, and Windows DOS command line will also work.

    The program has a system("clear") call in the display routine to clear
    the screen.  Windows users will need to change that to "cls" before
    compiling.

    cc rmaze.c -o rmaze
    ./rmaze mazefile.csv

    The maze is built by hand as a comma separated values (csv) data file.
    The perimeter and internal walls are represented with ones and pathways
    are represented with zeros.  The finish, represented with the letter F
    can be anywhere in the maze.  If the finish can not be reached the
    program backtracks all the way back to the start then quits.  The
    starting position is hard coded into the program at row 2 column 1.
    The maze can be no larger than 50x50.

    Here is a sample maze:

    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
    S,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1
    1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1
    1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,1
    1,0,0,0,0,0,0,0,1,0,1,1,1,0,0,1
    1,1,0,0,1,0,1,1,1,0,0,0,0,0,0,1
    1,0,0,0,0,0,1,0,1,0,1,1,1,1,0,1
    1,0,0,1,1,0,1,0,1,0,1,0,0,0,0,1
    1,0,0,0,1,0,1,0,0,0,1,1,1,1,1,1
    1,1,1,1,1,0,0,0,1,0,1,0,0,0,0,1
    1,0,0,0,1,0,0,1,1,0,1,0,0,0,0,1
    1,0,0,0,1,0,0,1,1,0,1,0,1,0,0,1
    1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1
    1,1,1,1,1,1,1,1,1,0,0,0,1,0,0,1
    1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,F,1


    by Clyde Hoadley July 24, 2021.
*******************************************************************************

    Depth-first search algorithm pseudocode

    procedure DFS(G, v) is
        label v as discovered
        for all directed edges from v to w that are in G.adjacentEdges(v) do
            if vertex w is not labeled as discovered then
                recursively call DFS(G, w)

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

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_MAZE_SIZE 51

typedef struct pointStruct {
    int x; // row
    int y; // column
} typePoint;  //This typedef will be used to create and array that will then be used as a stack

time_t start_time;  //start_time is global to the whole program. I just didn't
                    //want to figure out how to pass it as a parameter.
int sDepth = 0; //Maximum stack depth.

/******************         Forward Declarations              ********************/
void juggleChoices(typePoint Choices[4]);
int findInPath(int x, int y, typePoint path[MAX_MAZE_SIZE], int pathSize);
void findTheWay(char mase[MAX_MAZE_SIZE][MAX_MAZE_SIZE], typePoint path[MAX_MAZE_SIZE], int pathSize, int number_of_rows);
int addToPath(typePoint path[MAX_MAZE_SIZE], int x, int y, int pathSize);
void copy_maze(char to_maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], char from_maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], int number_of_rows);
int get_maze(const char *fileName,char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE]);
void display_maze(char m[MAX_MAZE_SIZE][MAX_MAZE_SIZE], typePoint path[MAX_MAZE_SIZE], int pathSize, int number_of_rows);
void endTheProgram(int status, int rows, int cols);
int main(int argc, char const *argv[]);



/************************************  JuggleChoices  ****************************/
void juggleChoices(typePoint Choices[4]) {
//  This is a poor attempt at trying to randomize findTheWay() choice of which way
//  to go next.

    int r;
    typePoint tempPoint;

    r = rand();
    switch ( r % 4 ) {  // This is a feeble attempt to randomize findTheWay choices.
        case 0: { break;}
        case 1: { tempPoint = Choices[0];
                Choices[0] = Choices[1];
                Choices[1] = tempPoint;
                tempPoint = Choices[2];
                Choices[2] = Choices[3];
                Choices[3] = tempPoint;
                break;}
        case 2: { tempPoint = Choices[1];
                Choices[1] = Choices[3];
                Choices[3] = tempPoint;
                break;}
        case 3: { tempPoint = Choices[3];
                Choices[3] = Choices[2];
                Choices[2] = tempPoint;
                break;}
        default: { fprintf(stderr,"juggleChoices Oops\n"); }
    }
    return;
}
/***********************************  findInPath  ******************************************/
int findInPath(int x, int y, typePoint path[MAX_MAZE_SIZE], int pathSize) {
//  Search the stack to see of this pair of (row & column) coordinates are in the stack.

    for(int i=1; i<pathSize+1; i++) {
        if ( (x == path[i].x) && (y == path[i].y) ) return(i);
    }
    return(-1); // If the point is in the path the return will be > -1.
}


/************************************  addToPath  *********************************************/
int addToPath(typePoint path[MAX_MAZE_SIZE], int x,int y, int pathSize) {
//  "Push" a pair of (row & column) coordinates onto the top of the stack.

    //Had trouble getting pathSize to return its new value until I did this.
    int newSize = pathSize; //The compiler was trying to keep me out of trouble.
                            // Then again maybe I was just being stupid.

    // Path is a 1 dimensional array of typePoint.  This array is used as a Last In First Out
    // stack of (row & column) points along the path.  The program explicitly adds (push) to
    // the stack in two places in the findTheWay procedure, but never explicitly removes (pop)
    // from the stack - that happens by magic when findTheWay returns from a recursive call.
    ++newSize;
    path[newSize].x = x;
    path[newSize].y = y;
    if (newSize > sDepth) sDepth = newSize;
    return(newSize);
}


/*******************************************  copy_maze  **************************************/
void copy_maze(char to_maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], char from_maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], int number_of_rows) {

    // I initially started out using memcpy() then switched to memmove() before becoming unsure of
    // myself, but I know this works so it's what I did.
    //
    // The program makes changes to the maze array when displaying the maze but that would destroy
    // the maze, so I'll make a temporary copy of the maze and mess up the copy.

    int number_of_columns = strlen(from_maze[0]);
    for(int x=0; x<number_of_rows; x++) {
       for(int y=0; y<number_of_columns; y++)
          to_maze[x][y] = from_maze[x][y];
      to_maze[x][number_of_columns]=(char)NULL; //NULL the end of the row.
    }
    return;
}//end-copy_maze()


/********************************************* get_maze ***************************************/
int get_maze(const char *fileName,char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE]) {
// Load the comma separated value (csv) file into a two dimensional array.
// Strip out anything that's not a digit.

    FILE *fp;
    char ch;
    int x, y;

    if ( (fp = fopen(fileName,"r")) == NULL) {
        fprintf(stderr, "\nError opening \"%s\" file for reading.\n",fileName);
        exit(EXIT_FAILURE);
    }
    // insure maze is filled with nulls
    for (x=0;  x<MAX_MAZE_SIZE; x++)
        for (y=0; y<MAX_MAZE_SIZE; y++)
            maze[x][y]=(char)NULL;  // I want to make darn sure the array is clean

    x=0; y=0;
    while( !feof(fp) ) {
        ch=(char)fgetc(fp);
        if ( ch == '\n') { x++; y=0; continue;}  //Start a new row.
        if (ch < '0') continue;  // skip this character if it is less than an ASCII zero.
        if ( (ch > '9') && ((ch != 'S') && (ch != 'F')) ) continue;  // skip this character if it is
                                                        // greater than an ASCII nine but not S or F.
        maze[x][y++]=(char)ch;
    }
    fclose(fp);
    return(x+1); // Return the number of rows in the maze.  (add one because C starts counting at zero.)
}//end-get_maze()


/******************************  display_maze  ************************************/
void display_maze(char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], typePoint path[MAX_MAZE_SIZE], int pathSize, int number_of_rows) {
// Display the maze and findTheWay() current progress on the screen.
// This procedure would destroy the maze if we didn't make a copy of it first.

    int x,y;
    struct timespec t1, t2;
    t1.tv_sec = 0;
    t1.tv_nsec = 125000000L;
    char mzCopy[MAX_MAZE_SIZE][MAX_MAZE_SIZE];

    // Make a copy of the maze because we need to mark it up for displaying.
    copy_maze(mzCopy,maze,number_of_rows);

    if (pathSize) { // If we've started down a path...
        for(int i=1; i<(pathSize+1); i++)
            mzCopy[path[i].x][path[i].y] = (char)'.'; //Show where we've been.
        mzCopy[ path[pathSize].x ][ path[pathSize].y ] =(char)'*';//Show where we are right now.
    }
    system("clear");
    for(x=0; x<number_of_rows; x++) {
        for(y=0; y<MAX_MAZE_SIZE; y++) {
            if (mzCopy[x][y]==(char)NULL) { printf("\n"); break; }
            if (mzCopy[x][y]=='2') { printf(" "); continue;}  // #2 node has already been visited
            if (mzCopy[x][y]=='1') { printf("█"); continue;}  // #1 represents a wall so we set it to a solid white.
            if (mzCopy[x][y]=='0') { printf(" "); continue;}  // #0 path is open so we can move there
            printf("%c",mzCopy[x][y]);
        }
    }
    printf("\n\n");
    nanosleep(&t1 , &t2); //Slow the display down long enough for us to see it.
}//end-display_maze()


/******************************  move  *****************************************/
void findTheWay(char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], typePoint path[MAX_MAZE_SIZE], int pathSize, int number_of_rows) {
// An implementation of a depth first search algorithm to find a path (any path) through a maze.

    typePoint choices[4];
    int x,y,number_of_columns=0;

    display_maze(maze,path,pathSize,number_of_rows);

    // Each point has a north, east, south, and west.
    // Or a back, left, front, and right.
    choices[0].x = path[pathSize].x;
    choices[0].y = path[pathSize].y+1;
    choices[1].x = path[pathSize].x;
    choices[1].y = path[pathSize].y-1;
    choices[2].x = path[pathSize].x+1;
    choices[2].y = path[pathSize].y;
    choices[3].x = path[pathSize].x-1;
    choices[3].y = path[pathSize].y;
    juggleChoices(choices);  //Feeble attempt to randomize the choices.

    number_of_columns = strlen(maze[0]);
    for(int i=0; i<4; i++) {    //check each direction then decide what to do.
        x=choices[i].x;
        y=choices[i].y;

        // Check coordinates to insure they're within the maze.
        if ( (x < 0) ||
             (y < 0) ||
             (x > number_of_rows) ||
             (y > number_of_columns) ) continue;

                    // Are the coordinates pointing to a wall or to somewhere we've
                    // already exhausted (all directions already checked?)
        else if( (maze[x][y] ==(char)'1' ) || (maze[x][y] ==(char)'2') )
                continue;

                    // Are these coordinates already in the path?
        else if (findInPath(x,y,path,pathSize) > -1 ) continue;

                // Could it be! Have we solved the maze?
        else if ( maze[x][y] ==(char)'F' ) {
                pathSize = addToPath(path, x,y ,pathSize);
                display_maze(maze,path,pathSize,number_of_rows);
                endTheProgram(EXIT_SUCCESS,number_of_rows,number_of_columns);
            }

        else {  // Let's go this way.
                pathSize = addToPath(path, x,y ,pathSize);

                // This is where the recursion takes place. findTheWay() calls findTheWay()
                findTheWay(maze, path, pathSize, number_of_rows);

                // When findTheWay() returns it effectively pops the stack (backtracked.)
                // This point has been completely checked so mark it as exhausted.
                maze[x][y] = (char)'2';
                display_maze(maze,path,pathSize,number_of_rows);
        }//end-if-else
    }//end-for

    return;
}//end-findTheWay()


/******************************  endTheProgram  *****************************************/
void endTheProgram(int status, int rows, int cols) {
// If successful the program terminates inside findTheWay; if unsuccessful the program
// terminates in main().  I want to make a report before terminating but I want to
// do it in one place.  If the code gets this far it didn't generate an error so the
// code itself is completing successfully/

    double duration;
    time_t finish_time;

    finish_time = time(0);
    duration = difftime(finish_time,start_time);// start_time is global to the program.
    if (status == EXIT_SUCCESS) {
        printf("\n%s\nMaze was solved in %.0lf seconds.\n", Version, duration);
        printf("Maze size (%d x %d). Maximum stack depth: %d.\n",rows, cols, sDepth);
    } else {
        printf("\n%s\nUnable to solve maze after %.0lf seconds.\n", Version, duration);
        printf("Maze size (%d x %d). Maximum stack depth: %d.\n",rows, cols, sDepth);
    }
    exit(EXIT_SUCCESS);
}


/******************************  main  *****************************************/
int main(int argc, char const *argv[]) {
// This is where it all begins; this is the Eve of rmaze.c  main() creates and
// initializes a few variables and arrays, loads the data file, establishes the
// starting coordinates, then sets findTheWay() into motion.

    char aMaze[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
    typePoint path[MAX_MAZE_SIZE*MAX_MAZE_SIZE]; //The stack can get big.
    int number_of_rows=0, pathSize=0;

    if (argc != 2) {
        printf("\n%s ERROR: missing filename.\nUsage: rmaze mazefile.csv\n\n",Version);
        exit(EXIT_FAILURE);
    }
    number_of_rows = get_maze(argv[1],aMaze);
    display_maze(aMaze,path,pathSize,number_of_rows);
    sleep(1);  // give the user a chance to see what the maze looks like.

    srand(time(NULL)); //give the system random function a good kick in the ass.
    start_time = time(0);
    pathSize=addToPath(path, 1,0, pathSize);  //prime the stack with a starting point
    findTheWay(aMaze,path,pathSize,number_of_rows);  // and ... we're off to the races.
    int columns = strlen(aMaze[0]);  // number of columns isn't used enough to store variable
                                     // or pass as a parameter, so I compute it when needed.
    endTheProgram(EXIT_FAILURE,number_of_rows,columns);  // report results and quit.
}
