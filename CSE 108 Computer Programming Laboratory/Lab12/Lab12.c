#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

struct node_list
{
    int data;
    float number;
    char* name;
    struct node_list *next;
};

/*Do not modify the following function.*/

void insert_end(struct node_list **head, int val, float num, char* name_)
{ 
    struct node_list *newnode = malloc(sizeof(struct node_list));
    newnode->data = val;
    newnode->number = num;
    newnode->name = name_;
    newnode->next = NULL;

    if(*head == NULL)
 		*head = newnode;
    else
    {
        struct node_list *lastnode = *head;

        while(lastnode->next != NULL)
			lastnode = lastnode->next;

        lastnode->next = newnode;
    }
}

/*Do not modify the following function.*/

void print_list(struct node_list *head)
{
	struct node_list *temp = head;

    while(temp != NULL)
    {
		printf("%d->", temp->data);
		printf("%lf->", temp->number);
		printf("%s", temp->name);
		printf("\t");
		temp = temp->next;
    }
    
    printf("NULL\n\n");
}

/*Do not modify the following function.*/

void print_array(struct node_list *array, int sizeof_array)
{
	int i;
	
	for(i=0;i<sizeof_array;i++)
	{
		printf("%d->", (array+i)->data);
    	printf("%lf->", (array+i)->number);
		printf("%s", (array+i)->name);
		printf("\t");
	}
		
    	printf("NULL\n\n");
}

/*You can modify following functions.*/

struct node_list* merge_list(struct node_list* head_1, struct node_list* head_2)
{
	/*You can modify the entire body of the function including the return statement one line below.*/
	return head_1;
}

struct node_list* merge_interleaved(struct node_list* head_1, struct node_list* head_2)
{
	/*You can modify the entire body of the function including the return statement one line below.*/
	return head_1;
}

struct node_list* merge_array(struct node_list* a, int na, struct node_list* b, int nb)
{
	/*You can modify the entire body of the function including the return statement one line below.*/
	return a;
}


int main()
{
	/*Do not modify anything between 95 and 130 lines.*/
	
	struct node_list *head_1 = NULL;
	struct node_list *head_2 = NULL;
	struct node_list *merged = NULL;
	struct node_list *interleaved = NULL;
	
	insert_end(&head_1,10,1.5,"hello1");
	insert_end(&head_1,30,3.5,"hello3");
	insert_end(&head_1,50,5.5,"hello5");
	 
	insert_end(&head_2,20,2.5,"hello2");
	insert_end(&head_2,40,4.5,"hello4");
	insert_end(&head_2,60,6.5,"hello6");
	
	printf("Print List 1: \n");
	 
	print_list(head_1);
	 
	printf("Print List 2: \n");
	 
	print_list(head_2);
	 
	merged=merge_list(head_1, head_2);
	
	printf("Print List Merged: \n");

    print_list(merged);

	interleaved=merge_interleaved(head_1, head_2);
	
	printf("Print List Interleaved: \n");
	
	print_list(interleaved);
 	
 	/*Do not modify anything between 95 and 130 lines.*/    
 	
	/*YOU CAN MODIFY BELOW THIS LINE FOR ONLY PART 3.*/
	
	
	
	
	
	
	
	
	
	
	
	return 0;
}
