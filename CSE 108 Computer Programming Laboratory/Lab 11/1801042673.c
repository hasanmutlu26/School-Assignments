/*Hasan Mutlu - 1801042673 - CSE 108 Lab11*/
#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <stdbool.h>

struct node
{
    int data;
    float number;
    char* name;
    struct node *next;
};


void insert_end(struct node **head, int val, float num, char* name_)
{
    //create a new node
    struct node *newNode = malloc(sizeof(struct node));
    newNode->data = val;
    newNode->number = num;
    newNode->name = name_;
    newNode->next = NULL;

    //if head is NULL, it is an empty list
    if(*head == NULL)
         *head = newNode;
    //Otherwise, find the last node and add the newNode
    else
    {
        struct node *lastNode = *head;

        //last node's next address will be NULL.
        while(lastNode->next != NULL)
        {
            lastNode = lastNode->next;
        }

        //add the newNode at the end of the linked list
        lastNode->next = newNode;
    }

}

void printList_withLoop(struct node *head)
{	
	/*Create an iterator and assign it to head pointer.*/
    struct node *iter;
    iter = head;

    /*print current iter node and move on to next node until it reaches NULL.*/
    while(iter != NULL){
        printf("%d->%f->%s\n",iter->data, iter->number, iter->name );
        iter = iter->next;
    }

}

void printList_withRecursive(struct node *list)
{
	/*Print current list node.*/
   	printf("%d->%f->%s\n",list->data, list->number, list->name);

   	/*If next node isn't null, send next node to function recursively*/
   	if(list->next != NULL)
       	printList_withRecursive(list->next);
}


void sortedInsert(struct node** head_ref, struct node* new_node)
{
	/*A temporary node to hold previous node. It's initial value is NULL.*/
    struct node* prev = NULL;

    /*An iterator to move through the list.*/
    struct node* iter = *head_ref;

    /*Cover whole list until iter reaches NULL.*/
    while(iter != NULL){
    	/*if strcmp of new_node and iter's names return a value smaller than 0, it means that
    	new_node must be added in front of iter.*/
        if(strcmp(new_node->name, iter->name) < 0){

        	/*If prev == NULL, it means that new_node is added at the beginning of the list.*/
            if(prev == NULL){
                new_node->next = iter;
                *head_ref = new_node;
            }
            
            else{
                prev->next = new_node;
                new_node->next = iter;
            }
        }

        /*If next of new_node isn't NULL, it means that we inserted it and loop can end.*/
        if(new_node->next != NULL)
            break;

        /*iter is assigned to prev and moved to next node.*/
        prev = iter;
        iter = iter->next;
    }

    /*if loop ends with iter being NULL, it means that new_node must be added to end of the list.*/
    if(iter == NULL)
    	prev->next = new_node;
}
 
 
/* A utility function to create a new node */
struct node* newNode(int new_data, float num, char* name_)
{
    struct node *newNode = malloc(sizeof(struct node));
    newNode->data = new_data;
    newNode->number = num;
    newNode->name = name_;
    newNode->next = NULL;
    return newNode;
}


bool search(struct node* head, int x)
{	
	/*An iterator is created.*/
    struct node* iter;
    iter = head;

    /*Whole list is covered, if x is found, true is returned.*/
    while(iter != NULL){
        if(iter->data == x)
            return true;
        iter = iter->next;
    }

    /*If loop ends, x is not found. False is returned.*/
    return false;
}

int main()
{
     struct node *head = NULL;

     insert_end(&head,10,5.5,"hello1");
     insert_end(&head,20,5.5,"hello2");
     insert_end(&head,30,5.5,"hello3");

     printf("Print List with Loop: \n");
     printList_withLoop(head);
     
     struct node* new_node = newNode(1,5.5, "a");
     sortedInsert(&head, new_node);

     
     new_node = newNode(3,5.5, "c");
     sortedInsert(&head, new_node);

     
     new_node = newNode(2,5.5, "d");
     sortedInsert(&head, new_node);
     
     new_node = newNode(2,5.5, "b");
     sortedInsert(&head, new_node);
     
     printf("Print List with Recursive: \n");
     printList_withRecursive(head);
	 
	 printf("\nIs 20 belong this linked list?: " );
     printf(search(head, 20) ? "YES \n" : "NO \n");
     
     printf("\nIs 18 belong this linked list?: " );
     printf(search(head, 18) ? "YES \n" : "NO \n");


     return 0;
}