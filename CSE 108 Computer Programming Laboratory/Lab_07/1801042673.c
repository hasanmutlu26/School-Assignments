/*Hasan Mutlu - 1801042673 - Lab 7*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>

/*Array size for part 1.*/
#define ARRAYSIZE 50

int check_palindrome(int a[],int n);
int search_element(int arr[],int input_number);
//float cos_calculation(int n, float x);

int MAX;

int main(){
	/*Part 1*/
	/*The array, a buffer for inputs and a counter for number of elements are declared.*/
	int array[ARRAYSIZE], buffer, n = 0;

	/*The elements of array are obtained from the user. -1 is used to stop.*/
	/*The last entered -1 is also added to array to indicate end of array.*/
	printf("Enter numbers for array. Enter -1 to stop.\n");
	for(int i = 0; i< ARRAYSIZE; i++){
		scanf("%d", &buffer);
		array[i] = buffer;
		if(buffer == -1)
			break;
		n++;
	}

	/*If check_palindrome function returns 1 the input is a palindrome. Else, it isnt.
	The size of the array, n is send initially.*/
	if(check_palindrome(array, n))
		printf("Your input is a palindrome.\n");
	else
		printf("Your input is not a palindrome.\n");

	printf("\n\n\n");



	/*Part 2*/
	/*Random number generator is set.*/
	srand(time(NULL));

	/*The array is created. A -1 is assigned at the end to indicate end of array.*/
	for(int i = 0; i<20; i++)
		array[i] = rand() % 100;
	array[20] = -1;

	/*Numbers in the array are printed.*/
	printf("The numbers in the array are: \n");
	for(int i = 0; i<20; i++)
		printf("%d: %d \n",i+1, array[i]);

	/*User is asked to enter a number for search.*/
	printf("\nEnter a number for search: ");
	scanf("%d", &buffer);

	/*If search_element returns 1, the number is in the array. Else, it isnt.*/
	if(search_element(array, buffer))
		printf("The number is in the array.\n");
	else
		printf("The number is not in the array.\n");


	printf("\n\n\n");

	/*Part 3
	int x;
	printf("Enter n: ");
	scanf("%d", &n);
	printf("Enter x: ");
	scanf("%d", &x);
	printf("Enter MAX: ");
	scanf("%d", &MAX);
	printf("cos(%d,%d) = %f\n\n", n, x, cos_calculation(2*n, x));
	*/


}

/*Check palindrome function returns 1 if array is palindrome, 0 if it isnt.*/
int check_palindrome(int a[],int n){
	/*If n is smaller than or equal to 0, it means that we got to the middle of the array 
	successfully and every element matched with required element. 
	The array is palindrome, 1 is returned.*/
	if(n <= 0)
		return 1;

	/*If a[0] is equal to a[n-1], the array still can be a palindrome. Next element of
	the array is sent as start of the array, n is decremented by 2 and sent to array.*/
	if(a[0] == a[n-1]){
		return check_palindrome(&a[1], n-2);
	}

	/*Else, 0 it means that the array is not palindrome, 0 is returned.*/
	else
		return 0;
}


int search_element(int arr[],int input_number){
	/*If arr[0] is -1, it means that we got to the end of the array.
	Number is not found. 0 is returned.*/
	if(arr[0] == -1)
		return 0;

	/*If a[0] is equal to number, it means that the number is found. 1 is returned.*/
	if(arr[0] == input_number)
		return 1;

	/*Else, the number isn't found yet, search_element is called again by sending 
	next element as the start of array.*/
	else
	 	return search_element(&arr[1], input_number);
}

/*
float cos_calculation(int n, float x){
	if(n/2 >= MAX)
		return 1-(x*x/(2*MAX-1)*(2*MAX));

	else
		return (1 - (x*x/(n-1)*(n))*cos_calculation(n+2,x) );
}*/