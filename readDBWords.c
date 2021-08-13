#define Version "{readDBWords (v1.0a)}"
/****************************************************************************************
readDBWords.c                                                              12 August 2021

    An example C program using the Maira/MySQL database and C api to demonstrate querying
    words from a MySQL database and printing each word to stdout.
    The program prompts the user for their MySQL username and password.  The MYSERVER
    #define statement should be updated to point to the correct MySQL server.

    readDBWords.c is a companion program of loadWordTable.c and readDBWordsSorted.c
    readDBWordsSorted and readDBWords are identical except for the format of the SELECT
    statement.

    gcc readDBWords.c `mysql_config –cflags` `mysql_config –libs` -g -Wall \\
        -I/usr/include/mysql  -L/usr/local/mysql/lib -lmysqlclient -o readDBWords

    ./readDBWords

    The database should have already been loaded with words by the loadWordTable.c
    program.

    These programs were compiled and tested on a Debian workstation.


    by Clyde Hoadley
*****************************************************************************************

    Procedure:
        1.  Gather username/password and connect to the database
        2.  Issue SQL query to database
        3.  Get the whole result set back from the database
        4.  While there are still results remaining, fetch a row and print.
        5.  Free the result set and disconnect from (close) the database.

*****************************************************************************************
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
****************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <termios.h>
#include <mysql.h>


#define MYSERVER      "webserver"
#define MYDATABASE    "playroom"
#define MAXWORDLENGTH 50


void errorExit(char *string,FILE *fp, MYSQL *conn, MYSQL_STMT *stmt);
void echo_off(struct termios *term_save);
void echo_on(struct termios term_save);
bool logonToDatabase(MYSQL **connect);
bool fetchWords(MYSQL *connect);
int main(int argc, char *argv[]);


/***************************************************************************************/
void errorExit(char *string,FILE *fp, MYSQL *conn, MYSQL_STMT *stmt) {
    fprintf(stderr,"\nERROR: %s\n%s\n",string,Version);
    if (fp) fclose(fp);
    if (stmt) mysql_stmt_close(stmt);
    if (conn) mysql_close(conn);
    exit(EXIT_FAILURE);
}


/***************************************************************************************/
void echo_off(struct termios *term_save) {
    struct termios tp;

    tcgetattr( STDIN_FILENO, &tp);              /* get existing terminal properties */
    *term_save = tp;                            /* save existing terminal properties */
    tp.c_lflag &= ~ECHO;                        /* only cause terminal echo off */
    tcsetattr( STDIN_FILENO, TCSAFLUSH, &tp );  /* set terminal settings */
}


/***************************************************************************************/
void echo_on(struct termios term_save) {
    /* restore original terminal settings */
    tcsetattr( STDIN_FILENO, TCSANOW, &term_save);
}


/***************************************************************************************/
bool logonToDatabase(MYSQL **connect) {
    char username[50], password[50];
    int l=0, tryCount=0;
    struct termios term_save; //for echo_off and echo_on to use.

    *connect=NULL;
    do {
        if ( ++tryCount > 3 ) return false;  //Three strikes and you're out.
        bzero(username,20);
        printf("\nUsername: ");
        fgets(username,19,stdin);
        l=strlen(username);
        if ( !l ) return false;                               // remove CR/LF
        if ( (username[l-1] < ' ') || (username[l-1] > '~') ) username[l-1]=0;
        if ( (username[l-2] < ' ') || (username[l-2] > '~') ) username[l-2]=0;

        echo_off(&term_save);
        bzero(password,50);
        printf("Password: ");
        fgets(password,49,stdin);
        l=strlen(password);
        if ( !l ) return false;
        if ( (password[l-1] < ' ') || (password[l-1] > '~') ) password[l-1]=0;
        if ( (password[l-2] < ' ') || (password[l-2] > '~') ) password[l-2]=0;
        echo_on(term_save);

        *connect= mysql_init(NULL);  //initializes a MYSQL structure for connect to use
        if (connect == NULL) {
           errorExit("mysql_init() failed.",NULL,NULL,NULL);
        }

    } while ( !mysql_real_connect(*connect, MYSERVER, username, password,
                          MYDATABASE, 0, NULL, 0) );
    printf("\n[Connected]\n");
    return true;
}


/***************************************************************************************/
bool fetchWords(MYSQL *connect) {
    MYSQL_RES *result;
    MYSQL_ROW row;
    int rowCount=0; //num_fields=0;

    if ( !connect ) return false;

    if (mysql_query(connect, "SELECT * FROM words")){
        errorExit("Failed to select from table",NULL,connect,NULL);
    }

    result = mysql_store_result(connect);  //this version returns the entire result set.

    if (result == NULL){
        errorExit("Failed to save results",NULL,connect,NULL);
    }

    //num_fields = mysql_num_fields(result);  //tells how many fields are in one row.

    while ((row = mysql_fetch_row(result))){
        rowCount++;
        printf("%5s\t%s\n", row[0],row[1]);
    }

    mysql_free_result(result);
    printf("\n[Fetched %d rows.]\n",rowCount);

    return true;
}



/***************************************************************************************/
int main(int argc, char *argv[]) {
    MYSQL *mysql=NULL;

    if (argc != 1) {
        errorExit("lreadDBWords does not use arguments.",NULL,NULL,NULL);
    }
    if ( !logonToDatabase(&mysql) ) {
        errorExit("Could not connect to database.",NULL,NULL,NULL);
    }
    if ( !fetchWords(mysql) ) {
        errorExit("Could not fetch words.",NULL,mysql,NULL);
    }
    mysql_close(mysql);

    printf("\n[Thank you.  Come again.]\n%s\n",Version);
    exit(EXIT_SUCCESS);
}
