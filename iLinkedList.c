#define Version "{iLinkedList (v1.0a)}"
/***********************************************************************************************
 iLinkedList.c                                                                      30 July 2021

 Demonstrate a linked list using integers.  The program was developed with the vision of some day
 developing into a binary tree.  So, integers are linked & listed in descending order with larger
 numbers first and smaller numbers.  This will be important when this program becomes the core of
 a binary tree program.

 cc iLinkedList.c
 ./a.out

    by Clyde Hoadley July 30, 2021.
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
                  int nodeInteger;
    struct nodeStruct *nextNode;
                  int nodeNumber;
} listNode;


/****************************************  Global  variables  *************************/
listNode *rootNode = NULL;
int nextNumber=0;


/****************************************  Forward Declarations  **********************/
listNode *walkNodes(listNode *someNode);
listNode *insertNode(int someInteger);
listNode *findNode(int nodeNumber);
listNode *makeNode(void);
listNode *updateNode(int nodeNumber, int someInteger);
void deleteNode(int nodeNumber);
int main(int argc, char const *argv[]);


/****************************************  walkNodes **********************************/
listNode *walkNodes(listNode *someNode) {
    listNode *thisNode=NULL;

    thisNode=someNode;
    while (thisNode) {
        printf("node#=%-4d   pointer=%-12p   data=%-4d\n",thisNode->nodeNumber,thisNode,thisNode->nodeInteger);
        thisNode = walkNodes(thisNode->nextNode);
    }
    return(NULL);
}//end-walkNodes()


/****************************************  insertNode  ********************************/
listNode *insertNode(int someInteger) {
    listNode *thisNode=NULL, *lastNode=NULL, *nextNode=NULL, *newNode=NULL;

    newNode=makeNode();
    newNode->nodeInteger=someInteger;
    // Handle cases involving rootNode
    if ( !rootNode ) {
        rootNode = newNode;
        return(rootNode);
    }
    if (someInteger == rootNode->nodeInteger) {free(newNode); --nextNumber; return(rootNode); }
    if (someInteger > rootNode->nodeInteger) {
        nextNode = rootNode;
        newNode->parentNode = NULL;
        nextNode->parentNode = newNode;
        newNode->nextNode = nextNode;
        //no change to rootNode->nextNode;
        rootNode = newNode;
        return(rootNode);
    }
    if (someInteger < rootNode->nodeInteger) {
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
        if ( thisNode->nodeInteger == someInteger ) {free(newNode); --nextNumber; return(rootNode); }
        if ( someInteger > thisNode->nodeInteger ) break; // found the spot.
        lastNode=thisNode;
        thisNode=thisNode->nextNode;
    }
    if ( !thisNode ) {
        lastNode->nextNode = newNode; // ran out of nodes so add newNode to end.
        newNode->parentNode = lastNode;
        newNode->nextNode=NULL;
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
        newNode->nodeInteger = 0;
        newNode->nextNode = NULL;
        newNode->nodeNumber = ++nextNumber;
        return(newNode);
    }
    return(NULL);
}//end-makeNode()


/****************************************  updateNode  **********************************/
listNode *updateNode(int nodeNumber, int someInteger) {
    listNode *thisNode=NULL,*newNode=NULL, *lastNode=NULL, *nextNode=NULL;

    thisNode = findNode(nodeNumber);
    if ( !thisNode ) return (NULL);
    lastNode=thisNode->parentNode;
    nextNode=thisNode->nextNode;
    if (lastNode) lastNode->nextNode=nextNode;
    if (nextNode) nextNode->parentNode=lastNode;
    if (thisNode == rootNode) rootNode = nextNode;
    free(thisNode);
    newNode=insertNode(someInteger);
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
    printf("Insert Ten integers:\n");
    insertNode(0);   //1
    insertNode(9);   //2
    insertNode(1);   //3
    insertNode(8);   //4
    insertNode(2);   //5
    insertNode(7);   //6
    insertNode(3);   //7
    insertNode(6);   //8
    insertNode(4);   //9
    insertNode(5);   //10
    printf("Display the list:\n");
    walkNodes(rootNode);

    printf("\n\nTest end conditions ...\n");
    printf("Update root node #2 with a new integer:\n");
    updateNode(2,109);
    printf("Display the list after update:\n");
    walkNodes(rootNode);

    printf("\n\nTest end conditions ...\n");
    printf("Update last node #1 with a new integer:\n");
    updateNode(1,-10);
    printf("Display the list after update:\n");
    walkNodes(rootNode);

    printf("\n\nUpdate in between node #7 with a new integer\n");
    updateNode(7,747);
    printf("Display the list after update:\n");
    walkNodes(rootNode);

    printf("\n\nDelete node #5.\n");
    deleteNode(5);
    printf("Display the list after delete:\n");
    walkNodes(rootNode);

    printf("\n\nUpdate node #1 again with another integer:\n");
    updateNode(1,636);
    printf("Display the list after update:\n");
    walkNodes(rootNode);

    printf("\n\nInsert two more strings:\n");
    insertNode(33);
    insertNode(-33);
    printf("Display the list after inserts:\n");
    walkNodes(rootNode);

    printf("\n\nInsert three more integers all value 42:\n");
    insertNode(42);
    insertNode(42);
    insertNode(42);
    printf("Display the list after inserts.  There should only be single 42:\n");
    walkNodes(rootNode);

    fprintf(stderr,"\nThank you! Come again.  %s\n",Version);
    return (0);
}//end-main()

