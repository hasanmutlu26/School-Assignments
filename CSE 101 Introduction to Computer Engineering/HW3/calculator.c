#include <stdio.h>
float main(){
	char operation, pause;
	float number1, number2, result, i, j, max;
	

	printf("Welcome to Calculator Machine\n");
	printf("Student name: Hasan Mutlu\nNumber: 1801042673\n\n");
	printf("Please enter the number for your operation.\n");
	printf("Press '1' for addition..\n");
	printf("Press '2' for substraction.\n");
	printf("Press '3' for multiplynig.\n");
	printf("Press '4' for division.\n");
	printf("Press '5' for finding the Nth power of a number.\n");
	printf("Press '6' for finding the average of several numbers.\n");
	printf("Press '7' for finding the maximum number of several numbers.\n");
	printf("Press '0' to exit.\n");

	scanf("%c", &operation);

	if (operation=='1'){
	    printf("You have selected addition.\n");
	    printf("First Number + Second Number = Result. \n");
		printf("Please enter the first value\n");
		scanf("%f", &number1);
		printf("Please enter the second value.\n");
		scanf("%f", &number2);
		result = number1 + number2;
		printf("The result of the addition:\n");
		printf("%f + %f = %f\n", number1, number2, result);}
		
		
	else if (operation=='2'){
	    printf("You have selected substraction.\n");
	    printf("First Number - Second Number = Result. \n");
		printf("Please enter the first value\n");
		scanf("%f", &number1);
		printf("Please enter the second value.\n");
		scanf("%f", &number2);
		result = number1 - number2;
		printf("The result of the substraciton\n:");
		printf("%f - %f = %f\n", number1, number2, result);}

		
	else if (operation=='3'){
		printf("You have selected multiplication.\n");
	    printf("First Number x Second Number = Result. \n");
		printf("Please enter the first value\n");
		scanf("%f", &number1);
		printf("Please enter the second value.\n");
		scanf("%f", &number2);
		result = number1 * number2;
		printf("The result of the multiplication\n:");
		printf("%f x %f = %f\n", number1, number2, result);}	

		
	else if (operation=='4'){
	    printf("You have selected division.\n");
	    printf("First Number / Second Number = Result. \n");
		printf("Please enter the first value\n");
		scanf("%f", &number1);
		printf("Please enter the second value.\n");
		scanf("%f", &number2);
		result = number1 / number2;
		printf("The result of the division\n:");
		printf("%f / %f = %f\n", number1, number2, result);}


	else if (operation == '5'){
	    printf("You have selected finding power.\n");
	    printf("First Number^Second Number = Result.\n");
	    printf("Please enter the first value\n");
		scanf("%f", &number1);
		printf("Please enter the second value.\n");
		scanf("%f", &number2);
		
		    if (number2=='0'){
		        result = 1;
		        printf("The result of finding power\n:");
		        printf("%f^%f = %f\n", number1, number2, result);}
		        
		    else { j = number1;
		           result = 1;
		        for (i=1; i<=number2; i++){
		            result = result * number1;} 
		            printf("The result of finding power\n:");
		            printf("%f^%f = %f\n", j, number2, result);} }			

	        
	else if (operation =='6'){
	    printf("You have selected finding the average.\n");
	    printf("Sum of the numbers / Amount of numbers = Result.\n"); 
	    printf("Enter a value.\n");
	    scanf("%f",&number1);
	    i=0;
	        while (pause != 'y'){
	            number2 = number1 + number2;
	            printf("Enter a value.\n");
                scanf("%f", &number1); 
                printf("Press 'y' when you are done entering values.\n");
                scanf("%c", &pause);
	            i++; } 
	    result = number2 / i;
	    printf("The average is: %f \n",result); }
	    
	    
	else if (operation == '7'){
		printf("You have selected finding the maximum of numbers inputted \n");
		printf("Enter a value:\n");
		scanf("%f", &number1);
		max = number1;
	        while (pause != 'y'){
	            printf("Enter a value.\n");
                scanf("%f", &number1); 
                	if ( number1 > max) {
                		max = number1;}
                printf("Press 'y' when you are done entering values.\n");
                scanf("%c", &pause);
	            i++; } 	
	    printf("The maxmimum number is: %f \n", max);}
	    	
	
						
	else if (operation == '0'){
	    printf("You have selected the exit.\n");
	    printf("Thank you for using my calculator.\n"); 
	    return 0;}
		

	else {
	    printf("You have pressed a wrong button.\n");}
	   

}
