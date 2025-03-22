#include <stdio.h>

/*Number of inputs in part1.*/
#define NUMINPUTS 5	

int find_max_distance(int num_inputs);
void find_odd_average();

void main(){
	/*Part1 is called and result is printed.*/
	printf("Max dist between two consecutive nums: %d\n", find_max_distance(NUMINPUTS));
	
	printf("\n\n\n");

	/*Part2 is called.*/
	find_odd_average();
}

int find_max_distance(int num_inputs){
	int currentNum, 	 /*Number read from user.*/
		prevNum, 		 /*Previous number.*/
		currentDistance, /*Current distance of last two entries.*/	
		maxDistance = 0; /*The max distance in all entries. Its initial value is 0.*/

	printf("Please enter %d numbers:\n", num_inputs);

	/*First number is directly assigned to prevNum.*/
	printf("1 : ");
	scanf("%d", &prevNum);

	/*Loop starts from 2 as we already read first number.*/
	for(int i=2; i<=NUMINPUTS; i++){

		/*New number is read.*/
		printf("%d : ", i);
		scanf("%d", &currentNum);

		/*Current distance of last two entries calculated.*/
		currentDistance = currentNum - prevNum;

		/*If current distance comes out negative, we turn it to positive.*/
		if(currentDistance < 0)
			currentDistance *= -1;

		/*If current distance is bigger than the max distance, it is the new max distance.*/
		if(currentDistance > maxDistance)
			maxDistance = currentDistance;

		/*Current number is assigned to prevNum.*/
		prevNum = currentNum;
	}
	/*Max distance is returned.*/
	return maxDistance;
}

void find_odd_average(){
	int i = 1, 		/*This will be used to print like 1: 2: 3:*/
		number, 	/*Number read from the user.*/
		numOfOdds = 0, /*Number of odds among the entered numbers.*/ 
		sum = 0;	/*Sum of the odd numbers.*/

	float average = 0.0;

	printf("Please enter positive numbers. (End -1 to stop entering):\n");

	/*Loop will be broken when user enters a negative number.*/
	while(1){
		
		/* 1: 2: 3: printed and i is incremented.*/
		printf("%d: ", i);
		i++;

		/*Number is read.*/
		scanf("%d", &number);
		
		/*If number is negative, loop ends.*/
		if(number < 0)
			break;

		/*If number is odd, it is added to the sum and numOfOdds is inremented.*/
		if(number % 2 != 0){
			sum += number;
			numOfOdds++;
		}
	}

	/*Average is calculated.*/
	average = (float)sum / (float)numOfOdds;
	
	/*Result is printed.*/
	printf("Average of odd numbers: %f\n", average);

}