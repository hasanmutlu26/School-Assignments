#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

typedef struct country_data{
	char country[20];
	char capital[20];
	int population;
	bool driving_side;
}country_data;

typedef struct linked{
	char country[20];
	char capital[20];
	int population;
	bool driving_side;
	struct linked* next;
}linked;

void menu();
void printMenu();
void part1();
void part2();
void part3();
void part4();

/*Our arrays, its size and linked list are global variables.*/
country_data *arr;
int size = 0;
linked* head;

/*Main calls menu.*/
void main(){
	menu();
	free(arr);
	free(head);
}


void menu(){
	int selection;
	int size = 0;

	/*One block is allocated for array as start.*/
	arr = (country_data*)calloc(1,sizeof(country_data));

	while(1){
		printMenu(); /*Prints menu.*/

		/*User selection is obtained and operation is done.*/
		scanf("%d", &selection);

		if(selection == 1)
			part1();
		else if(selection == 2)
			part2();
		else if(selection == 3)
			part3();
		else if(selection == 4)
			part4();
		else if(selection == 5)
			break;
	}
}


void part1(){
	int intBuffer;
	/*Size is inremented.*/
	size++;

	/*A new array with new size is created.*/
	country_data *newArr = (country_data*)calloc(size,sizeof(country_data));

	/*current values in arr are assigned to new array.*/
	for(int i = 0; i< size - 1; i++){
		newArr[i] = arr[i];
	}

	/*arr is freed and new array is assigned to main array.*/
	free(arr);
	arr = newArr;

	/*Country data is obtained from user.*/
	printf("\nCountry name: ");
	scanf("%s", arr[size-1].country);

	printf("Capital: ");
	scanf("%s", arr[size-1].capital);

	printf("Population: ");
	scanf("%d", &arr[size-1].population);

	printf("Do people in %s drive in right side? (Yes: 1, No: 0): ", arr[size-1].country);
	scanf("%d", &intBuffer);
	arr[size-1].driving_side = (bool)intBuffer;
}


void part2(){
	/*File is opened.*/
	FILE *fp;
	fp = fopen("file.txt", "w+");

	/*Data is written into file.*/
	for(int i = 0; i < size ; i++){
		fprintf(fp, "%s ", arr[i].country);
		fprintf(fp, "%s ", arr[i].capital);
		fprintf(fp, "%d ", arr[i].population);
		if(arr[i].driving_side == true)
			fprintf(fp, "Right\n");
		else
			fprintf(fp, "Left\n");
	}

	fclose(fp);
}

/*Following part don't work.*/
void part3(){
	linked* iter = head;
	FILE *fp;
	fp = fopen("file.txt", "r+");
	char cBuffer[20];
	int intBuffer;

	for(int i = 0; i<size; i++){
		iter = (linked*)malloc(sizeof(linked));

		fscanf(fp, "%s", cBuffer);
		strcpy(iter->country,cBuffer);

		fscanf(fp, "%s", cBuffer);
		strcpy(iter->capital,cBuffer);

		fscanf(fp, "%d", &intBuffer);
		iter->population = intBuffer;

		fscanf(fp, "%s", cBuffer); 
		if(strcmp(cBuffer, "Right") == 0)
			iter->driving_side = true;
		else
			iter->driving_side = false;

		iter = iter->next;
	}

	fclose(fp);
}

void part4(){
	linked* iter = head;

	while(iter != NULL){
		printf("Country 	%s", iter->country);
		printf("Capital 	%s", iter->capital);
		printf("Population 	%d", iter->population);
		if(iter->driving_side == true)
			printf("Driving Side 	Right");
		else
			printf("Driving Side 	Left\n");
		iter = iter->next;
	}
}

/*Prints menu.*/
void printMenu(){
	printf("\n1: Enter new record\n");
	printf("2: Write to the file\n");
	printf("3: Read from file\n");
	printf("4: Print the linked list\n");
	printf("5: Exit\n");
	printf("Your Choice: ");
}