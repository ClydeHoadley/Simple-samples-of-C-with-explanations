#define Version "{sLinkedList (v1.0a)}"
/***********************************************************************************************
 sLinkedList.c                                                                      30 July 2021

 Demonstrate a linked list using strings.  The program was developed with the vision of some day
 developing into a binary tree.  So, strings are linked & listed with larger ASCII character
 values first and lower ASCII characters values on last.  (z-a) come before (Z-A).  This will
 be important when this program becomes the core of a binary tree program.

 cc sLinkedList.c
 ./a.out

************************************************************************************************
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
***********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/****************************************  Defines  ***********************************/
typedef struct nodeStruct {
    struct nodeStruct *parentNode;
                 char nodeString[50];
    struct nodeStruct *nextNode;
                  int nodeNumber;
} listNode;


/****************************************  Global  variables  *************************/
listNode *rootNode = NULL;
int nextNumber=0;


/****************************************  Forward Declarations  **********************/
listNode *walkNodes(listNode *someNode);
listNode *insertNode(char *someString);
listNode *findNode(int nodeNumber);
listNode *makeNode(void);
listNode *updateNode(int nodeNumber, char *someString);
void deleteNode(int nodeNumber);
int main(int argc, char const *argv[]);


/****************************************  walkNodes **********************************/
listNode *walkNodes(listNode *someNode) {
    listNode *thisNode=NULL;

    thisNode=someNode;
    while (thisNode) {
        printf("node#=%-4d   pointer=%-12p   data=%-12s ASCII value of %c is %3d\n",
            thisNode->nodeNumber,thisNode,thisNode->nodeString,thisNode->nodeString[0],
                (int)thisNode->nodeString[0]);
        thisNode = walkNodes(thisNode->nextNode);
    }
    return(NULL);
}//end-walkNodes()



/****************************************  insertNode  ********************************/
listNode *insertNode(char *someString) {
    listNode *thisNode=NULL, *lastNode=NULL, *nextNode=NULL, *newNode=NULL;
    int compare;

    newNode=makeNode();
    strcpy(newNode->nodeString, someString);
    // Handle cases involving rootNode
    if ( !rootNode ) {
        rootNode = newNode;
        return(rootNode);
    }
    compare=strcmp(someString,rootNode->nodeString); // no need to do strcmp twice.
    if ( compare == 0) {free(newNode); --nextNumber; return(rootNode); }   // duplicate values are not allowed.
    if ( compare > 0) {  //put above root
        nextNode = rootNode;
        newNode->parentNode = NULL;
        nextNode->parentNode = newNode;
        newNode->nextNode = nextNode;
        //no change to rootNode->nextNode;
        rootNode = newNode;
        return(rootNode);
    }
    if ( strcmp(someString,rootNode->nodeString) > 0) { //put below root
        if ( !rootNode->nextNode ) {
            newNode->parentNode = rootNode;
            //no change to rootNode->parentNode;
            newNode->nextNode = NULL;
            rootNode->nextNode = newNode;
            return(newNode);
        }
    }
    // rootNode cases have been taken care off

    // insert newNode.  It will either go between two nodes or at the end
    thisNode = rootNode->nextNode;
    lastNode = rootNode;
    nextNode = NULL;

    // find the insertion point
    while (thisNode) {
        compare=strcmp(someString,thisNode->nodeString); // no need to do strcmp twice.
        if ( compare == 0 ) {free(newNode); --nextNumber; return(rootNode); }   // duplicate values are not allowed.
        if ( compare > 0 ) break;  // found the spot.
        lastNode=thisNode;
        thisNode=thisNode->nextNode;
    }
    if ( !thisNode ) {
        lastNode->nextNode = newNode; // ran out of nodes so add newNode to end.
        newNode->parentNode = lastNode;
        return(newNode);
    }

    newNode->parentNode=lastNode;  // insert newNode between lastNode and thisNode.
    thisNode->parentNode=newNode;
    newNode->nextNode=thisNode;
    lastNode->nextNode=newNode;
    //no change to thisNode->nextNode

    return(newNode);
}//end-insertNode()


/***************************************  findNode  ***********************************/
listNode *findNode(int nodeNumber) {
    listNode *thisNode=NULL;

    if (!rootNode ) return (NULL);
    thisNode = rootNode;
    while (thisNode) {
        if (thisNode->nodeNumber == nodeNumber) return(thisNode);
        thisNode = thisNode->nextNode;
    }
    return(NULL);
}//end-findInteger()


/****************************************  makeNode  **********************************/
listNode *makeNode(void) {
    listNode *newNode=NULL;

    newNode = (listNode*)malloc(sizeof(struct nodeStruct));
    if (newNode) {
        newNode->parentNode=NULL;
        newNode->nodeString[0] = (char)"";
        newNode->nextNode = NULL;
        newNode->nodeNumber = ++nextNumber;
        return(newNode);
    }
    return(NULL);
}//end-makeNode()


/****************************************  updateNode  **********************************/
listNode *updateNode(int nodeNumber, char *someString) {
    listNode *thisNode=NULL,*newNode=NULL, *lastNode=NULL, *nextNode=NULL;

    thisNode = findNode(nodeNumber);
    if ( !thisNode ) return (NULL);
    lastNode=thisNode->parentNode;
    nextNode=thisNode->nextNode;
    if (lastNode) lastNode->nextNode=nextNode;
    if (nextNode) nextNode->parentNode=lastNode;
    if (thisNode == rootNode) rootNode = nextNode;
    free(thisNode);
    newNode=insertNode(someString);
    if (newNode) { newNode->nodeNumber = nodeNumber;  --nextNumber; }
    return(newNode);

}//end-updateNode()


/****************************************  deleteNode  ********************************/
void deleteNode(int nodeNumber) {
    listNode *thisNode=NULL, *nextNode=NULL, *lastNode=NULL;

    thisNode = findNode(nodeNumber);
    if ( !thisNode ) return;
    lastNode=thisNode->parentNode;
    nextNode=thisNode->nextNode;
    nextNode->parentNode=lastNode;
    lastNode->nextNode=nextNode;
    free(thisNode);
    return;
}//end-deleteNode()


/****************************************  main()  ************************************/
int main(int argc, char const *argv[]) {
    listNode *thisNode=NULL;

    fprintf(stderr,"\n%s\n\n",Version);
    printf("Insert Ten strings:\n");
    insertNode("Five");    //1
    insertNode("Three");   //2
    insertNode("Eight");   //3
    insertNode("Nine");    //4
    insertNode("Six");     //5
    insertNode("Zapple");  //6
    insertNode("Seven");   //7
    insertNode("Two");     //8
    insertNode("Four");    //9
    insertNode("One");     //10
    printf("Display the list:\n");
    walkNodes(rootNode);

    printf("\n\nTest end conditions ...\n");
    printf("Update root node #6 with a new string.\n");
    updateNode(6,"Zipple");
    printf("Display the list after update:\n");
    walkNodes(rootNode);

    printf("\n\nTest end conditions ...\n");
    printf("Update last node #3 with a new string.\n");
    updateNode(3,"apple");
    printf("Display the list after update:\n");
    walkNodes(rootNode);

    printf("\n\nUpdate in between node #10 with a new string.\n");
    updateNode(10,"Golf");
    printf("Display the list after update:\n");
    walkNodes(rootNode);

    printf("\n\nDelete node #5.\n");
    deleteNode(5);
    printf("Display the list after delete:\n");
    walkNodes(rootNode);

    printf("\n\nUpdate node #3 again with a new string \"Apple\".\n");
    updateNode(3,"Apple");
    printf("Display the list after update:\n");
    walkNodes(rootNode);

    printf("\n\nInsert two more strings:\n");
    insertNode("weather");
    insertNode("Weather");
    printf("Display the list after inserts:\n");
    walkNodes(rootNode);

    printf("\n\nInsert three more strings all reading \"forty two\":\n");
    insertNode("forty two");
    insertNode("forty two");
    insertNode("forty two");
    printf("Display the list after inserts.  There should only be one \"forty two\":\n");
    walkNodes(rootNode);

    fprintf(stderr,"\nThank you! Come again.  %s\n",Version);
    return (0);
}//end-main()




