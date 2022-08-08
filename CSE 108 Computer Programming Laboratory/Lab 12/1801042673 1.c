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
	/*An iterator created.*/
	struct node_list* iter;
	iter = head_1;
	/*iterator reaches the end of head_1 list.*/
	while(iter->next != NULL)
		iter = iter->next;
	/*head_2 is added to end of head_1*/
	iter->next = head_2;
	return head_1;
}

struct node_list* merge_interleaved(struct node_list* head_1, struct node_list* head_2)
{	
	/*2 iterators and 2 temporary nodes to hold next nodes are created.*/
	struct node_list* iter1;
	struct node_list* iter2;
	struct node_list* temp1;
	struct node_list* temp2;

	iter1 = head_1;
	iter2 = head_2;

	/*Until iter2's next is NULL.*/
	while(iter2->next != NULL){
		/*next nodes are assigned to temp nodes.*/
		temp1 = iter1->next;
		temp2 = iter2->next;

		/*iter2 node is linked to iter1 and temp1 node is linked to iter2.*/
		iter1->next = iter2;
		iter2->next = temp1;

		/*temps are assigned to iterators back.*/
		iter1 = temp1;
		iter2 = temp2;
	}

	/*After loop ends, the last node of head_2 list is assigned at the end*/
	iter1->next = iter2;

	return head_1;
}

struct node_list* merge_array(struct node_list* a, int na, struct node_list* b, int nb)
{
	/*A new array is created with na+nb size.*/
	struct node_list* array = (struct node_list*)calloc(na+nb, sizeof(struct node_list));
	
	/*Index for a and b arrays.*/
	int j=0;

	for(int i=0; i< na+nb; ){
		/*if j is smaller than size of a, add a[j] to array.*/
		if(j < na){
			array[i] = a[j];
			i++;
		}
		/*if j is smaller than size of b, add b[j] to array.*/
		if(j < nb){
			array[i] = b[j];
			i++;
		}
		j++;
	}
	/*free a and b and return array.*/
	free(a);
	free(b);
	return array;
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
	
	/*Two integers to hold size of arrays.*/
	int na, nb;

	/*Size of arrays are obtained from user.*/
	printf("Enter size of array 1: ");
	scanf("%d", &na);
	printf("Enter size of array 2: ");
	scanf("%d", &nb);

	/*Two arrays are created according to user's inputs.*/
	struct node_list *a = (struct node_list*)calloc(na, sizeof(struct node_list));
	struct node_list *b = (struct node_list*)calloc(nb, sizeof(struct node_list));

	/*Arrays are filled with random numbers and same name.*/
	for(int i=0; i<na;i++){
		a[i].data = rand() % 10;
		a[i].number = i;
		a[i].name = "hello1";
	}

	for(int i=0; i<nb;i++){
		b[i].data = rand() % 10;
		b[i].number = i;
		b[i].name = "hello2";
	}
	
	/*A new array is created to assign function return.*/
	struct node_list* merged_array = (struct node_list*)calloc(na+nb, sizeof(struct node_list));
	merged_array = merge_array(a, na, b, nb);
	print_array(merged_array, na+nb);
	
	
	
	
	
	return 0;
}
