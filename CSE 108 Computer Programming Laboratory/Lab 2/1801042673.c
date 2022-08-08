/*Hasan Mutlu - 1801042673*/

#include <stdio.h>
#include <math.h>

#define PI 3.141592

void standart();

int integerAddition(int num1, int num2);
int integerMultiplication(int num1, int num2);

void complexAddition(int num1, int num1Complex, int num2, int num2Complex);
void complexMultiplication(int num1, int num1Complex, int num2, int num2Complex);


void areaPerimeter();

float rectanglePerimeter(float a,float b);
float trianglePerimeter(float a,float b,float c);
float circlePerimeter(float r);

float rectangleArea(float a,float b);
float triangleArea(float a,float b,float c);
float circleArea(float r);


void temperature();

float calculateTemperature(float temp, int type);



void main(){
	int selection;

	printf("Enter the calculator type:\n");
	printf("1. Standart Calculator\n");
	printf("2. Area&Perimeter Calculator\n");
	printf("3. Temperature calculator\n");
	printf("--------------------------\n");

	scanf("%d", &selection);

	if(selection == 1)
		standart();

	else if(selection == 2)
		areaPerimeter();

	else if(selection == 3)
		temperature();
}

void standart(){
	int numberType, calculationType, num1, num2, num1Complex, num2Complex;

	printf("Enter the number type\n");
	printf("1. Integer \n");
	printf("2. Complex\n");
	printf("--------------------------\n");

	scanf("%d", &numberType);

	printf("Enter the calculation type\n");
	printf("1. Addition\n");
	printf("2. Multiplication\n");
	printf("--------------------------\n");
	scanf("%d", &calculationType);

	if(numberType == 1){
		printf("Please enter first number: \n");
		scanf("%d", &num1);
		printf("Please enter second number: \n");
		scanf("%d", &num2);

		if(calculationType == 1)
			printf("Addition of the numbers: %d\n", integerAddition(num1, num2));

		else if(calculationType == 2)
			printf("Multiplication of the numbers: %d\n", integerMultiplication(num1, num2));
	}

	else if(numberType == 2){
		printf("Please enter first complex number (real part first): \n");
		scanf("%d", &num1);
		scanf("%d", &num1Complex);
		printf("Please enter second complex number (real part firs)t: \n");
		scanf("%d", &num2);
		scanf("%d", &num2Complex);

		if(calculationType == 1)
			complexAddition(num1, num1Complex, num2, num2Complex);

		else if(calculationType == 2)
			complexMultiplication(num1, num1Complex, num2, num2Complex);
	}


}

int integerAddition(int num1, int num2){
	return num1 + num2;
}

int integerMultiplication(int num1,int num2){
	return num1*num2;
}

void complexAddition(int num1, int num1Complex, int num2, int num2Complex){
	printf("Addition of numbers: %d + (%di\n)", num1+num2, num1Complex+num2Complex);
}

void complexMultiplication(int num1, int num1Complex, int num2, int num2Complex){
	printf("Multiplication of numbers: %d + (%di)\n", num1*num2+(-num1Complex*num2Complex), num1*num2Complex + num2*num1Complex);
}





void areaPerimeter(){
	int shapeType, a,b,c;

	printf("Enter the shape type\n");
	printf("1. Rectangle\n");
	printf("2. Triangle\n");
	printf("3. Circle\n");
	printf("--------------------------\n");

	scanf("%d", &shapeType);

	if(shapeType == 1){
		printf("Enter two sides of rectangle:\n");
		scanf("%d", &a);
		scanf("%d", &b);

		printf("Perimeter of rectangle: %f\n", rectanglePerimeter(a,b));
		printf("Area of rectangle: %f\n", rectangleArea(a,b));
	}
	else if(shapeType == 2){
		printf("Enter three sides of triangle:\n");
		scanf("%d", &a);
		scanf("%d", &b);
		scanf("%d", &c);

		printf("Perimeter of triangle: %f\n", trianglePerimeter(a,b,c));
		printf("Area of triangle: %f\n", triangleArea(a,b,c));
	}
	else if(shapeType==3){
		printf("Enter radius of circle:\n");
		scanf("%d", &a);

		printf("Perimeter of circle: %f\n", circlePerimeter(a));
		printf("Area of circle: %f\n", circleArea(a));
	}
}



float rectanglePerimeter(float a,float b){
	return 2*(a+b);
}


float rectangleArea(float a,float b){
	return a*b;
}

float trianglePerimeter(float a,float b,float c){
	return a+b+c;
}

float triangleArea(float a,float b,float c){
	int s;
	s= (a+b+c)/2;

	return sqrt(s*(s-a)*(s-b)*(s-c));
}

float circlePerimeter(float r){
	return 2*PI*r;
}

float circleArea(float r){
	return PI*r*r;
}





void temperature(){
	int calculationType;
	float temperature;
	printf("Enter the calculation type\n");
	printf("1. Fahrenheit to Kelvin\n");
	printf("2. Kelvin to Fahrenheit\n");
	printf("--------------------------\n");

	scanf("%d", &calculationType);

	
	printf("Please enter the temperature in ");

	if(calculationType == 1)
		printf("Fahrenheit\n");
	else if(calculationType == 2)
		printf("Kelvin\n");

	scanf("%f", &temperature);

	printf("Result = %f\n", calculateTemperature(temperature, calculationType));

}


float calculateTemperature(float temp, int type){
	if(type == 1)
		return ((5.0/9.0)*(temp-32.0)) + 273.15;
	

	else if(type == 2)
		return (9.0/5.0)*(temp-273.15)+32.0;
}