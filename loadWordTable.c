#define Version "{loadWordTable (v1.0a)}"
/****************************************************************************************
loadWordTable.c                                                            11 August 2021

    An example C program using the Maira/MySQL database and C api to demonstrate reading
    an ascii data file of words (1 word per line) and storing each word into the database.
    The program prompts the user for their MySQL username and password.  The MYSERVER
    #define statement should be updated to point to the correct MySQL server.

    loadWordTable.c is a companion program of readDBWords.c and readDBWordsSorted.c
    readDBWordsSorted and readDBWords are identical except for the format of the SELECT
    statement.

    gcc loadWordTable.c `mysql_config –cflags` `mysql_config –libs` -g -Wall \\
        -I/usr/include/mysql  -L/usr/local/mysql/lib -lmysqlclient -o loadWordTable

    ./loadWordTable  datafile

    Where datafile is the name of a file containing words with 1 word per line.  Such a
    data file can be found on the GitHub where this program was found.

    A database and table should have already been set up using the following:

        CREATE DATABASE playroom;
        USE playroom;
        CREATE TABLE words
        (
            id              INT unsigned NOT NULL AUTO_INCREMENT, # Unique ID
            word            VARCHAR(150) NOT NULL,                # word to be stored
            PRIMARY KEY     (id)                                  # Make the id the  key
        );

    These programs were compiled and tested on a Debian workstation.


    by Clyde Hoadley
*****************************************************************************************

    Procedure:
        1.  gather username and password then connect to the database
        2.  get a statement pointer from the database
        3.  open datafile for reading
        4.  for each word fill in a statement structure one at a time
        5.  associate (bind) the statement structure to the statement pointer
        6.  tell the database to perform the statement.
        7.  close files, statements, and connections when end of data file.

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
bool readOneWord(FILE *file, char *word);
bool insertOneWord(char *word, MYSQL_STMT *stmt);
bool readAndInsertWords(MYSQL *connect, char *filename);
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
        if ( ++tryCount > 3 ) return false;
        bzero(username,50);
        printf("\nUsername: ");
        fgets(username,49,stdin);
        l=strlen(username);
        if ( !l ) return false;                                //remove CR/LF
        if ( (username[l-1] < ' ') || (username[l-1] > '~') ) username[l-1]=0;
        if ( (username[l-2] < ' ') || (username[l-2] > '~') ) username[l-2]=0;

        echo_off(&term_save);   //do not display password when entered
        bzero(password,50);     //echo on/off might not work on all terminals
        printf("Password: ");
        fgets(password,49,stdin);
        l=strlen(password);
        if ( !l ) return false;
        if ( (password[l-1] < ' ') || (password[l-1] > '~') ) password[l-1]=0;
        if ( (password[l-2] < ' ') || (password[l-2] > '~') ) password[l-2]=0;
        echo_on(term_save);

        *connect= mysql_init(NULL);
        if (connect == NULL) {
           errorExit("mysql_init() failed.",NULL,NULL,NULL);
        }

    } while ( !mysql_real_connect(*connect, MYSERVER, username, password,
                          MYDATABASE, 0, NULL, 0) );
    printf("\nConnected\n");
    return true;
}


/***************************************************************************************/
bool readOneWord(FILE *file, char *word) {
    int l=0;

    bzero(word,MAXWORDLENGTH);
    fgets(word,MAXWORDLENGTH,file);
    l=strlen(word);
    if ( !l ) return false;
    if ( (word[l-1] < ' ') || (word[l-1] > '~') ) word[l-1]=0; // remove CR/LF
    if ( (word[l-2] < ' ') || (word[l-2] > '~') ) word[l-2]=0;
    //printf("(%d) %s (%d)",l,word,strlen(word));
    return true;
}


/***************************************************************************************/
bool insertOneWord(char *word, MYSQL_STMT *stmt) {
    MYSQL_BIND bind[2];
    unsigned long wordLength;
    char id_ind[]= {STMT_INDICATOR_NULL, STMT_INDICATOR_NULL, STMT_INDICATOR_NULL};

    char myInsertStatement[]="INSERT INTO words VALUES (?,?)"; //this is a template
    if (mysql_stmt_prepare(stmt, myInsertStatement, strlen(myInsertStatement))) {
        return false;
    }

    memset(bind, 0, sizeof(MYSQL_BIND) * 2);  //the DB table has 2 fields

    /* We autogenerate id's, so all indicators are STMT_INDICATOR_NULL */
    bind[0].u.indicator=  id_ind;          //field 1 is the id used as a key but it
    bind[0].buffer_type= MYSQL_TYPE_LONG;  //will be automatically generated by the db

    wordLength = strlen(word);
    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)word;         //field 2 holds the word we are storing
    bind[1].buffer_length = wordLength;
    bind[1].length = &wordLength;

    // connect the bind array to the statement string.
    mysql_stmt_bind_param(stmt, bind);
    if (mysql_stmt_execute(stmt)) {
        return false;
    }

    return true;
}


/***************************************************************************************/
bool readAndInsertWords(MYSQL *connect, char *filename) {
    MYSQL_STMT *stmt=NULL;
    char word[MAXWORDLENGTH];
    FILE *fp=NULL;
    int rowCount=0;

    if ( !connect ) return false;
    stmt= mysql_stmt_init(connect);
    if (stmt == NULL) {
        errorExit("mysql_stmt_init() failed",NULL,connect,NULL);
    }

    if ( (fp=fopen(filename, "r")) == NULL ) {
        errorExit("Could not open file for reading.",NULL,connect,stmt);
    }
    printf("File %s opened for reading.\n",filename);
    while (readOneWord(fp,word) ) {
        insertOneWord(word,stmt);
        ++rowCount;
    }
    printf("\nInserted %d rows.\n",rowCount);
    fclose(fp);
    mysql_stmt_close(stmt);
    return true;
}



/***************************************************************************************/
int main(int argc, char *argv[]) {
    MYSQL *mysql=NULL;

    if (argc != 2) {
        errorExit("Usage: loadWordTable filename",NULL,NULL,NULL);
    }
    if ( !logonToDatabase(&mysql) ) {
        errorExit("Could not connect to database.",NULL,NULL,NULL);
    }
    if ( !readAndInsertWords(mysql, argv[1]) ) {
        errorExit("Could not read or insert words.",NULL,mysql,NULL);
    }
    mysql_close(mysql);

    printf("\nThank you.  Come again.\n%s\n",Version);
    exit(EXIT_SUCCESS);
}
