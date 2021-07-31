#define Version "{iLinkedList (v1.0c)}"
/***********************************************************************************************
 iLinkedList.c                                                                      31 July 2021

 Demonstrate a linked list using integers.  The program was developed with the vision of some day
 developing into a binary tree.  So, integers are linked & listed in descending order with larger
 numbers first followed by smaller numbers.  This will be important when this program becomes the
 core of a binary tree program.

 Each node has two pointers: a pointer to its parent and a pointer to its child.  For this list
 a node can have zero or one parent and zero or 1 child.  The root node (first node of the list)
 is the only node that can have zero parent.  The end node is the only node that can have no
 child node.  This doesn't account for when the root node is the only node of the list.

 The main() function does all of the demonstrating.

 This program has a twin called sLinkedList.c it uses strings instead of integers otherwise it's
 the same program.

 cc iLinkedList.c
 ./a.out

    by Clyde Hoadley July 31, 2021.
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


/********************************************  Defines  ***************************************/
typedef struct nodeStruct {
    struct nodeStruct *parentNode;
                  int dataInteger;
    struct nodeStruct *nextNode;
                  int nodeKey;
} listNode;


/********************************************  Global  variables  ******************************
Nine out of ten Doctors say that global variables are detrimental to a programmers health and
should not be used.  I'm not a Doctor so I don't know if that's really true.
*/
listNode *rootNode = NULL;
int nextKey=0;


/********************************************  Forward Declarations  **************************/
void printList(listNode *someNode);
listNode *insertNode(int someInteger);
listNode *findNode(int nodeKey);
listNode *makeNode(void);
listNode *updateNode(int nodeKey, int someInteger);
void deleteNode(int nodeKey);
int main(int argc, char const *argv[]);


/********************************************  printList * *************************************
When given the pointer to a node printList will print the contents of that node then
follow and print nextNode until it reaches the end of the list.  Give printList the
rootNode to print the entire list from beginning to end.
list
*/
void printList(listNode *someNode) {
    listNode *thisNode=NULL;
    int counter=0;

    thisNode=someNode;
    while (thisNode) {
        ++counter;
        printf("key=%-4d   nodePtr=%-12p   data=%-4d\n",
                    thisNode->nodeKey,thisNode,thisNode->dataInteger);
        thisNode = thisNode->nextNode;
    }
    printf("I counted %d nodes.\n",counter);
    return;
}//end-printList()


/********************************************  insertNode  *************************************
Given some data (in this case an integer) insertNode will create a new node then figure out
where in the linked list the new node should be placed.  insertNode will prevent duplicate
entries.  insertNode must handle 5 different scenarios ... very first node, a new node taking
over as the root node, a new node being inserted directly beneath the root node, a new node
being inserted at the end of the list, and inserting the new node between two existing nodes.

I struggled with this.  I finally gave up and started over.  I concentrated on the root node and
drew a picture on some paper to aid me in figuring out a solution.  After solving the root
node conditions I was able, with the aid of another drawing, to solve the remaining insertions.

Preventing duplicate entries was added after the program was working correctly.

*/
listNode *insertNode(int someInteger) {
    listNode *thisNode=NULL, *lastNode=NULL, *nextNode=NULL, *newNode=NULL;

    newNode=makeNode();
    newNode->dataInteger=someInteger;
    // Handle cases involving rootNode
    if ( !rootNode ) {
        rootNode = newNode; // don't have a list yet.  This will start a new list.
        return(rootNode);
    }
                                                 // duplicate entries are not allowed.
    if (someInteger == rootNode->dataInteger) {free(newNode); --nextKey; return(rootNode); }

    if (someInteger > rootNode->dataInteger) {  //the new node needs to be inserted above
        nextNode = rootNode;                    //the root node and will take over as root.
        newNode->parentNode = NULL;
        nextNode->parentNode = newNode;
        newNode->nextNode = nextNode;
        //no change to rootNode->nextNode;
        rootNode = newNode;
        return(rootNode);
    }
    if (someInteger < rootNode->dataInteger) {  //the new node will be inserted directly
        if ( !rootNode->nextNode ) {            //beneath the root.
            newNode->parentNode = rootNode;
            //no change to rootNode->parentNode;
            newNode->nextNode = NULL;
            rootNode->nextNode = newNode;
            return(newNode);
        }
    }
    // rootNode cases have been taken care off


    // The new node will either go between two existing nodes or added to the end of the list.
    thisNode = rootNode->nextNode;
    lastNode = rootNode;
    nextNode = NULL;

    // find the insertion point
    while (thisNode) {                            //duplicate entries are not allowed.
        if ( thisNode->dataInteger == someInteger ){free(newNode); --nextKey; return(rootNode);}
        if ( someInteger > thisNode->dataInteger ) break; // found the spot before reaching
        lastNode=thisNode;                                // the end of the list.
        thisNode=thisNode->nextNode;
    }

    if ( !thisNode ) {
        lastNode->nextNode = newNode; // ran out of nodes so add newNode to the end of the list.
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


/********************************************  findNode  ***************************************
findNode does a leaner search of the linked list from beginning (rootNode) to end until
it finds the node with the right nodeKey.
*/
listNode *findNode(int nodeKey) {
    listNode *thisNode=NULL;

    if (!rootNode ) return (NULL);
    thisNode = rootNode;
    while (thisNode) {
        if (thisNode->nodeKey == nodeKey) return(thisNode);
        thisNode = thisNode->nextNode;
    }
    return(NULL);
}//end-findInteger()


/********************************************  makeNode  ***************************************
makeNode returns a pointer to an all new empty pointer that is not connected to anything.
makeNode assigns the new node its own nodeKey.  insertNode will find where the new node
should go within the list and assign parentNode and nextNode their values.
*/
listNode *makeNode(void) {
    listNode *newNode=NULL;

    newNode = (listNode*)malloc(sizeof(struct nodeStruct));
    if (newNode) {
        newNode->parentNode=NULL;
        newNode->dataInteger = 0;
        newNode->nextNode = NULL;
        newNode->nodeKey = ++nextKey;
        return(newNode);
    }
    return(NULL);
}//end-makeNode()


/********************************************  updateNode  *************************************
Given a nodeKey, search the linked list from beginning to end until the node having that
key is found.  Since changing the data integer may cause the node to no longer be in the
right place within the list, and since insertNode always puts the new node into its
right place, it's easiest to delete the node with the old integer value and create a
new node with the new integer and assign it the old nodes key value.

The program would crash every so often.  I finally realized it would crash any time I tried to
update the first node of the list or the last node of the list.  I again drew a picture on
paper to solve the problem.
*/
listNode *updateNode(int nodeKey, int someInteger) {
    listNode *thisNode=NULL,*newNode=NULL, *lastNode=NULL, *nextNode=NULL;

    thisNode = findNode(nodeKey);
    if ( !thisNode ) return (NULL);
    lastNode=thisNode->parentNode;
    nextNode=thisNode->nextNode;
    if (lastNode) lastNode->nextNode=nextNode; // ok to have a null member pointer but
    if (nextNode) nextNode->parentNode=lastNode;//not ok to access a member of a null node.
    if (thisNode == rootNode) rootNode = nextNode;//Is the new node taking over as root?
    free(thisNode);
    newNode=insertNode(someInteger);
    if (newNode) { newNode->nodeKey = nodeKey;  --nextKey; }//makeNode increments key but
    return(newNode);                                        //don't need a new key.

}//end-updateNode()


/********************************************  deleteNode  ************************************/
void deleteNode(int nodeKey) {
    listNode *thisNode=NULL, *nextNode=NULL, *lastNode=NULL;

    thisNode = findNode(nodeKey);
    if ( !thisNode ) return;
    lastNode=thisNode->parentNode;
    nextNode=thisNode->nextNode;
    nextNode->parentNode=lastNode;
    lastNode->nextNode=nextNode;
    free(thisNode);
    return;
}//end-deleteNode()


/********************************************  main()  ****************************************/
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
    printList(rootNode);

    printf("\n\nTest end conditions ...\n");
    printf("Update root node having key=2 with a new integer:\n");
    updateNode(2,109);
    printf("Display the list after update:\n");
    printList(rootNode);

    printf("\n\nTest end conditions ...\n");
    printf("Update last node having key=1 with a new integer:\n");
    updateNode(1,-10);
    printf("Display the list after update:\n");
    printList(rootNode);

    printf("\n\nUpdate in-between node key=7 with a new integer\n");
    updateNode(7,747);
    printf("Display the list after update:\n");
    printList(rootNode);

    printf("\n\nDelete node key=5.\n");
    deleteNode(5);
    printf("Display the list after delete:\n");
    printList(rootNode);

    printf("\n\nUpdate node key=1 again with another integer:\n");
    updateNode(1,636);
    printf("Display the list after update:\n");
    printList(rootNode);

    printf("\n\nInsert two more nodes with integers of 33 & -33:\n");
    insertNode(33);
    insertNode(-33);
    printf("Display the list after inserts:\n");
    printList(rootNode);

    printf("\n\nInsert three more integers all with value 42:\n");
    insertNode(42);
    insertNode(42);
    insertNode(42);
    printf("Display the list after inserts.  There should only be single 42:\n");
    printList(rootNode);

    printf("\n\nTry deleting a node with a key that doesn't exist.\n");
    deleteNode(876);
    deleteNode(-123);
    printList(rootNode);

    fprintf(stderr,"\nThank you! Come again.  %s\n",Version);
    return (0);
}//end-main()

