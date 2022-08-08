/*Hasan Mutlu - 1801042673 - Lab6*/

#include <stdio.h>
#include <string.h>

/*The maximum string size for part2*/
#define STRING_MAXSIZE 20


float average_finder(float *matrix, int rows, int columns);

/*I changed the parameter types to pointer to send whole array.*/
void intervew(const char* s1, const char* s2, char * s3);

void main(){
	/*PART 1*/
	/*Declaring matrix arrays.*/
	float Matrix3x5[3][5];
	float Matrix4x9[4][9];
	float count;
	
	/*Count will count the nth entry of matrix to make calculation of 2x^2-3*/
	/*By this way, all matrix members are asssigned.*/
	count = 1;
	for(int i=0;i<3;i++){
		for(int j=0; j<5; j++){
			Matrix3x5[i][j] = 2*count*count - 3;
			count++;
		}
	}

	/*Same for the second matrix.*/
	count = 1;
	for(int i=0;i<4;i++){
		for(int j=0; j<9; j++){
			Matrix4x9[i][j] = 2*count*count - 3;
			count++;
		}
	}


	printf("Average Finder\n\n");
	/*Matrix is printed.*/
	printf("Matrix :\n");
	for(int i=0;i<3;i++){
		for(int j=0; j<5; j++){
			printf("%.0f	", Matrix3x5[i][j]);
		}
		printf("\n");
	}
	/*Matrix is sent to function and the result is printed.*/
	printf("\nResult = %.1f\n\n", average_finder(&Matrix3x5[0][0], 3, 5));

	printf("\n\n");

	/*The same for the second matrix.*/
	printf("Matrix :\n");
	for(int i=0;i<4;i++){
		for(int j=0; j<9; j++){
			printf("%.0f	", Matrix4x9[i][j]);
		}
		printf("\n");
	}
	printf("\nResult = %.1f\n\n", average_finder(&Matrix4x9[0][0], 4, 9));

	printf("\n\n\n");


	/*PART 2*/

	/*String arrays are declared and all their members are set to NULL character.
	We will us this NULL character to indicate that we got to the end of string.*/
	char s1[STRING_MAXSIZE+1] = {'\0'};
	char s2[STRING_MAXSIZE+1] = {'\0'};
	char s3[STRING_MAXSIZE + STRING_MAXSIZE+1] = {'\0'};

	/*Strings are obtained from the user.*/
	printf("Enter first string:\n");
	scanf("%s", s1);

	printf("\nEnter second string:\n");
	scanf("%s", s2);

	/*Strings are sent to function and result is printed.*/
	intervew(s1, s2, s3);
	printf("\nResult: %s\n", s3);

}

/*Function takes the array as it is 1D. We will get required value with 
*(matrix + columns*i + j) indexing operation.*/
float average_finder(float *matrix, int rows, int columns){
	/*We declared sum and evenAmount.*/
	float sum = 0.0;
	float evenAmount = 0;
	/*We cover whole matrix and when sum of indexes is even number, 
	number in that index is added to the sum and evenAmount is incremented.*/
	for(int i=0; i<rows; i++){
		for(int j=0; j<columns; j++){
			if(((i + j) % 2) == 0){
				/*We get required value by this indexing operation.*/
				sum += *(matrix + columns*i + j);
				evenAmount++;
			}
		}
	}
	/*Sum is divided by number of even numbers and the result, average is returned.*/
	return sum / evenAmount;
}

void intervew(const char* s1, const char* s2, char * s3){
	/*i will be the index of s1 and s2, while j will be the index of s3*/
	int i = 0;
	int j = 0;

	/*A loop while we don't get to the end of both strings.*/
	while(s1[i] != '\0' || s2[i] != '\0'){
		/*If we didn't get to the end of s1 yet, the character in s1[i] is assigned to s3[j]*/
		if(s1[i] != '\0'){
			s3[j] = s1[i];
			j++;	/*j is incremented as we have added a new character in s3.*/ 
		}

		/*As same, if we didn't get to the end of s1 yet, the character in s1[i] is assigned to s3[j]*/
		if(s2[i] != '\0'){
			s3[j] = s2[i];
			j++;	/*j is incremented as we have added a new character in s3.*/
		}
		/*i is incremented as we move on to next pair of characters in s1 and s2.*/
		i++;
	}
}