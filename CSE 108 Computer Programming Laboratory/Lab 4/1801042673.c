/* Hasan Mutlu 1801042673 Lab4 */

#include <stdio.h>


void russian_multiplication(unsigned int* multiplicand, unsigned int* multiplier);
void multiply_polynomials(double* a3, double* a2, double* a1, double* a0, double* b3, double* b2, double* b1, double b0);

void main(){
	/*PART 1*/
	unsigned int multiplicand, multiplier;

	/*Scanning values of multiplicand and multiplier.*/
	printf("Enter multiplicand: ");
	scanf("%d", &multiplicand);
	printf("Enter multiplier: ");
	scanf("%d", &multiplier);

	/*Function call by reference.*/
	russian_multiplication(&multiplicand, &multiplier);

	/*Result is printed.*/
	printf("\nProduct: %d\nMultiplier: %d\n\n\n\n", multiplicand, multiplier);



	/*PART 2*/
	double a3, a2, a1, a0, b3, b2, b1, b0;
	
	/*Scanning values.*/
	printf("Enter first polynomial's numbers. (a3, a2, a1, a0): ");
	scanf("%lf %lf %lf %lf", &a3, &a2, &a1, &a0);
	printf("Enter second polynomial's numbers. (b3, b2, b1, b0): ");
	scanf("%lf %lf %lf %lf", &b3, &b2, &b1, &b0);

	/*Function call by reference.*/
	multiply_polynomials(&a3, &a2, &a1, &a0, &b3, &b2, &b1, b0);

	/*Result is printed.*/
	printf("\nResult:\n(%lf)X^6 + (%lf)X^5 + (%lf)X^4 + (%lf)X^3 + (%lf)X^2 + (%lf)X + (%lf)\n", a3, a2, a1, a0, b3, b2, b1);
}



void russian_multiplication(unsigned int*  multiplicand, unsigned int*multiplier){
	/*We will add multiplicand's values into sum when multiplier is odd.*/
	unsigned int sum = 0;

	/*Loop until multiplier reaches 1.*/
	while(*multiplier != 1){
		/*If multiplier is odd, current value of multiplicand is added to sum.*/
		if(*multiplier % 2 == 1){
			sum += *multiplicand; 
		}
		/*multiplicand is multiplied and multiplier is divided by two.*/
		*multiplicand *= 2;
		*multiplier /= 2;
	}
	/*The last value of multiplicand is added to sum manually as the loop ended when multiplier reached to 1*/ 
	sum += *multiplicand;

	/*sum is assigned to multiplicand to return the result.*/
	*multiplicand = sum;
}



void multiply_polynomials(double* a3, double* a2, double* a1, double* a0, double* b3, double* b2, double* b1, double b0){
	/*Declared 7 variables with similar format to calculate the results into. r stands for result.*/
	double r6, r5, r4, r3, r2, r1, r0;

	/*The results are calculated.*/
	r6 = (*a3 * *b3);
	r5 = (*a2 * *b3) + (*a3 * *b2);
	r4 = (*a2 * *b2) + (*a3 * *b1) + (*b3 * *a1);
	r3 = (*a3 *  b0) + (*b3 * *a0) + (*a1 * *b2) + (*b2 * *a1);
	r2 = (*a2 *  b0) + (*b2 * *a0) + (*a1 * *b1);
	r1 = (*a1 *  b0) + (*b1 * *a0);
	r0 = (*a0 *  b0);

	/*The results are assigned to incoming variables to return the result.*/
	*a3 = r6;
	*a2 = r5;
	*a1 = r4;
	*a0 = r3;
	*b3 = r2;
	*b2 = r1;
	*b1 = r0;
}