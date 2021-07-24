#define Version "rmaze (v2.0b)"
/******************************************************************************
rmaze.c                                                            24-July-2021

    This program uses a recursive depth-first search algorithm to solve a maze.
    This     C program was derived from a Python program written and
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
    The maze can be no larger than 50x50 but that could take a long time
    to build and a long time to solve.

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
    int x;
    int y;
} typePoint;  //This doesn't exists because it's a type not a thing.
              //But the typedef has to be above the forward declarations.

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
    int r;
    typePoint tempPoint;

    r = rand();
    switch ( r % 4 ) {  // This is a feeble attempt to randomize solvers choices.
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
    for(int i=1; i<pathSize+1; i++) {
        if ( (x == path[i].x) && (y == path[i].y) ) return(i);
    }
    return(-1); // If the point is in the path the return will be > -1.
}


/************************************  addToPath  *********************************************/
int addToPath(typePoint path[MAX_MAZE_SIZE], int x,int y, int pathSize) {
    //Had trouble getting pathSize to return its new value until I did this.
    int newSize = pathSize; //The compiler was trying to keep me out of trouble.
    ++newSize;
    path[newSize].x = x;
    path[newSize].y = y;
    if (newSize > sDepth) sDepth = newSize;
    return(newSize);
}


/****************************** copy_maze  *************************************/
void copy_maze(char to_maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], char from_maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], int number_of_rows) {
    int number_of_columns = strlen(from_maze[0]);
    for(int x=0; x<number_of_rows; x++) {
       for(int y=0; y<number_of_columns; y++)
          to_maze[x][y] = from_maze[x][y];
      to_maze[x][number_of_columns]=(char)NULL; //NULL the end of the row.
    }
    return;
}


/****************************** get_maze ***************************************/
int get_maze(const char *fileName,char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE]) {     //read the maze from a csv data file
    FILE *fp;
    int ch=0, x, y;

    if ( (fp = fopen(fileName,"r")) == NULL) {
        fprintf(stderr, "\nError opening \"%s\" file for reading.\n",fileName);
        exit(EXIT_FAILURE);
    }
    // insure maze is filled with nulls
    for (x=0;  x<MAX_MAZE_SIZE; x++)
        for (y=0; y<MAX_MAZE_SIZE; y++)
            maze[x][y]=(char)NULL;

    x=0; y=0;
    while( !feof(fp) ) {
        ch=fgetc(fp);
        if ( ch == '\n') { x++; y=0; continue;}  //Start a new row.
        if (ch < 48) continue;  // skip this character if it is less than an ASCII zero.
        maze[x][y++]=(char)ch;
    }
    fclose(fp);
    return(x+1); // Return the number of rows in the maze.
}//end-get_maze()


/******************************  display_maze  ************************************/
void display_maze(char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], typePoint path[MAX_MAZE_SIZE], int pathSize, int number_of_rows) {
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
}


/******************************  move  *****************************************/
void findTheWay(char maze[MAX_MAZE_SIZE][MAX_MAZE_SIZE], typePoint path[MAX_MAZE_SIZE], int pathSize, int number_of_rows) {
    typePoint choices[4];
    int x,y,number_of_columns=0;

    display_maze(maze,path,pathSize,number_of_rows);

    choices[0].x = path[pathSize].x;
    choices[0].y = path[pathSize].y+1;
    choices[1].x = path[pathSize].x;
    choices[1].y = path[pathSize].y-1;
    choices[2].x = path[pathSize].x+1;
    choices[2].y = path[pathSize].y;
    choices[3].x = path[pathSize].x-1;
    choices[3].y = path[pathSize].y;
    juggleChoices(choices);

    number_of_columns = strlen(maze[0]);
    for(int i=0; i<4; i++) {
        x=choices[i].x;
        y=choices[i].y;

        // Check point to insure it's within the maze.
        if ( (x < 0) ||
             (y < 0) ||
             (x > number_of_rows) ||
             (y > number_of_columns) ) continue;

                    // Is the point being inspected a wall or somewhere
                    // we've already exhausted?
        else if( (maze[x][y] ==(char)'1' ) || (maze[x][y] ==(char)'2') )
                continue;

                    // Is this point in current path?
        else if (findInPath(x,y,path,pathSize) > -1 ) continue;

                // Could it be! Have we solved the maze?
        else if ( maze[x][y] ==(char)'F' ) {
                pathSize = addToPath(path, x,y ,pathSize);
                display_maze(maze,path,pathSize,number_of_rows);
                endTheProgram(EXIT_SUCCESS,number_of_rows,number_of_columns);
            }

        else {  // Let's go this way.
                pathSize = addToPath(path, x,y ,pathSize);

                // This is where the recursion takes place.
                findTheWay(maze, path, pathSize, number_of_rows);

                // When findTheWay() returns we've effectively popped the stack (backtracked.)
                // this point has been completely checked so mark it.
                maze[x][y] = (char)'2';
                display_maze(maze,path,pathSize,number_of_rows);
        }//end-if-else
    }//end-for

    return;
}//end-findTheWay()


/******************************  endTheProgram  *****************************************/
void endTheProgram(int status, int rows, int cols) {
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
    char aMaze[MAX_MAZE_SIZE][MAX_MAZE_SIZE];
    typePoint path[MAX_MAZE_SIZE*MAX_MAZE_SIZE]; //The stack can get big.
    int number_of_rows=0, pathSize=0;

    if (argc != 2) {
        printf("\n%s ERROR: missing filename.\nUsage: rmaze mazefile.csv\n\n",Version);
        exit(EXIT_FAILURE);
    }
    number_of_rows = get_maze(argv[1],aMaze);
    display_maze(aMaze,path,pathSize,number_of_rows);
    sleep(1);

    srand(time(NULL));
    start_time = time(0);
    pathSize=addToPath(path, 1,0, pathSize);
    findTheWay(aMaze,path,pathSize,number_of_rows);
    int columns = strlen(aMaze[0]);
    endTheProgram(EXIT_FAILURE,number_of_rows,columns);
}
