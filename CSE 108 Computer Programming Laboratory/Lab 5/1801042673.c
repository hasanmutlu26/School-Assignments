/* Hasan Mutlu - 1801042673 - Lab5 */

#include <stdio.h>
#include <stdlib.h>

/*Function declerations.*/
void expression0 (int x, int *result);
void expression1 (int x, int *result);
void expression2 (int x, int *result);
void addition(void (*func)(int, int *), int *sum, int n);
void calculation (int  *sumArray, int n);

void createArray(int (*array)[10]);

void main(){
	/*PART 1*/
	/*Array, n, and sum are created.*/
	int arr[3];
	int n, sum;

	/*Value of n is obtained by user.*/
	printf("n: ");
	scanf("%d", &n);

	/*Calculation function is called.*/
	calculation(arr, n);

	/*Results are printed.*/
	printf("Sum of expression0: %d\n", arr[0]);
	printf("Sum of expression1: %d\n", arr[1]);
	printf("Sum of expression2: %d\n\n\n\n", arr[2]);


	/*PART 2*/
	/*The array and its indexes are declared.*/
	int matrixArray[10][10], i, j;

	/*createArray function is called to fill the array.*/
	createArray(matrixArray);

	/*The array is printed.*/
	for(i=0; i<10; i++){
		for(int j=0; j<10; j++){
			printf("%d ", matrixArray[i][j]);
		}
		printf("\n");
	}

	printf("\n");

	/*An endless loop until an invalid input.*/
	while(1){
		/*Indexes of element are obtained by user.*/
		printf("Which element of the matrix do you want to reach?\n");
		printf("i: ");
		scanf("%d", &i);
		printf("j: ");
		scanf("%d", &j);

		/*If the input is invalid, loop is broken.*/
		if(i < 0 || i > 9 || j < 0 || j > 9)
			break;
		
		/*If input is valid, the result is printed.*/
		printf("%d. row %d. column of the matrix is %d\n\n", i, j, matrixArray[i][j]);
	}
	printf("Invalid input. Terminating...\n");
}



void calculation (int  *sumArray, int n){
	int sum;
	/*Sum is assigned to 0. Addition function is called with current expression. 
	Then, the result is assigned to necessary element of array.
	This process is repeated with all three expressions.*/
	sum = 0;
	addition(expression0, &sum, n);
	sumArray[0] = sum;

	sum = 0;
	addition(expression1, &sum, n);
	sumArray[1] = sum;

	sum = 0;
	addition(expression2, &sum, n);
	sumArray[2] = sum;
}


void addition(void (*func)(int, int *), int *sum, int n){
	/*A variable is declared to assign the returned values from expression functions.*/
	int result;
	/*A loop from 0 to n.*/
	for(int i=0; i<=n; i++){
		/*An expression function is called.*/
		func(i, &result);
		/*The result is added to sum variable.*/
		*sum += result;
	}
}

/*Expressions functions only calculate the expression for the current x.*/
void expression0 (int x, int *result){
	*result = x*x+5;
}

void expression1 (int x, int *result){
	*result = 2*x + 1;
}

void expression2 (int x, int *result){
	*result = x*x;
}





/*Array is created in a nested loop.*/
void createArray(int (*array)[10]){
	int i, j;
	for(i=0; i<10; i++){
		for(int j=0; j<10; j++){
			array[i][j] = rand () % 90 + 10;
		}
	}
}