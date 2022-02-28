#include <iostream>
#include <cstring>
#include <ctime>

using namespace std;

/*Number of iterations required to fail.*/
const int FAILSTATE = 100;

/*This function checks if a number is unique or not.*/
bool uniqueCheck(int number);

/*This function checks existance of one digit in a number.*/
int checkExistance(int number, int digit, int placeOfDigit);

/*This function generates a number.*/
int generateRandomNumber(int numOfDigits);

/*This function counts digits of a number.*/
int countDigits(int number);

/*This function is the game.*/
void mastermind(int number);


int main(int argc, char** argv){
    /*Setting the random number generator.*/
    srand(time(NULL));

    /*If argc != 3, it means that parameters are wrong. E0 error message is printed 
    and the program is terminated.*/
    if(argc != 3){
        cout << "E0" << endl;
        return 0;
    }

    /*The third value entered by user is turned to an integer and assigned to number variable.*/
    int number = stoi(argv[2], NULL, 0);

    /*If the second argument in program call is -r..*/
    if(strcmp(argv[1],"-r") == 0){

        /*If number entered is smaller than or equal to 0, E0 error message is printed 
        and the program is terminated.*/
        if(number <= 0){
            cout << "E0" << endl;
            return 0;
        }

        /*Else, the number entered by the user is the length of the number to be guessed. 
        A random number is generated and sent to mastermind function.*/
        mastermind(generateRandomNumber(number));
    }


    /*If the second argument in program call is -u..*/
    else if(strcmp(argv[1],"-u") == 0){

        /*The number entered by user is checked if it is unique. 
        If it isn't, E0 error message is printed and the program is terminated.*/
        if(uniqueCheck(number) != true){
            cout << "E0" << endl;
            return 0;
        }

        /*Else, the number is sent to mastermind function.*/
        mastermind(number);
    }

    /*If the second argument is neither -r nor -u, it is wrong parameter. 
    E0 error message is printed and the program is terminated.*/
    else{
        cout << "E0" << endl;
        return 0;
    }
}




/*This function checks the existance of a digit in a number. It takes the number, the digit and the expected place of the digit.
If the digit doesn't exist in the number, it returns 0. 
If the digit exists in the number but isn't in the expected place from right, it returns 1.
If the digit exists and is in the expected place, it returns 2.
For example, if the function is called like 
checkExistance(12345,4,2), 
it will return 2. Because 4 exists in the number and is in the 2nd place from right.
This mechanism will be used to calculate firstCount and secondCount.*/
int checkExistance(int number, int digit, int placeOfDigit){

    int exist = 0,          /*The exist status. Initial value is 0.*/
        placeCounter = 1;   /*Counts the place of the number from right. Initial value is 1*/
    
    /*In this while loop, we get rightmost digit by %10 operation until we find the digit or the number reaches 0.
    Number is divided by 10 at the end of the loop to get rid of the rightmost digit we already covered.*/
    while(number != 0){

        /*If the digit we search for is found..*/
        if(number % 10 == digit){

            /*If placeOfDigit is equal to placeCounter, it means that the digit exists in the number and is in the expected place.
            exist status is set to 2.*/
            if(placeOfDigit == placeCounter)
                exist = 2;

            /*Else, it means that the digit exists but not in the expected place. exist status is set to 1.*/
            else
                exist = 1;

            /*Number is set to 0 to end the while loop.*/
            number = 0;
            }

        /*placeCounter is incremented as we advance to the next digit.*/
        placeCounter++;

        /*Number is divided by 10 to get rid of the current rightmost digit.*/
        number /= 10;
    }
    /*The exist status is returned.*/
    return exist;
}





/*This function checks if a number is unique or not. It takes the number as argument. 
If the number is unique, function returns true. If it isn't unique, function returns false.
It checks the existance of every digit in the number itself. 
When a number is found at an unexpected place, it means that the number is not unique.*/
bool uniqueCheck(int number){

    int temp = number,      /*We will use temp to get the digits of the number one by one.*/
        placeCounter = 1;   /*placeCounter is for counting the digits we cover. 
                            We will send this to checkExistance function as 3rd parameter.*/
    bool uniqueState = true;    /*The uniqueness status of number. Its initial value is 1.*/

    /*We will get the rightmost digit by % 10 operation and send it to checkExistance function..*/
    while(temp != 0){

        /*If checkExistance doesn't return 2, it means that the current digit exists in an unexpected place in the number,
        meaning that this digit is repeated. uniqueState is set to 0 and temp is set to 0 to end the loop.*/
        if(checkExistance(number,temp % 10,placeCounter) != 2){
            uniqueState = false;
            temp = 0;
        }

        /*placeCounter is incremented as we advance to the next digit. temp is divided by 10 to get rid of the current rightmost digit*/
        placeCounter++;
        temp /= 10;
    }
    /*uniqueState is returned.*/
    return uniqueState;
}


/*This function generates a unique random number. It takes the number of digits in the number as argument.
Number is generated by first creating a number which starts as 0, generating a random digit, 
checking if it exists in the current number. If it doesn't exist, it is multiplied by the coef and added to the current number.
The coef variable will be used to multiply the new digit by 1, 10 ,100, 1000... */ 
int generateRandomNumber(int numOfDigits){

    int number = 0,     /*The random number will be generated into this variable.*/
        digit,          /*The digit generated randomly.*/
        coef = 1;       /*The coefficient. It will be multiplied by 10 for every new digit.*/

        /*For the amount of numOfDigits..*/
        for(int i=1;i<=numOfDigits;i++){
            /*A random digit, which doesn't exist in the current number, is generated.*/ 
            do{
                digit = rand() % 10;
            }while(checkExistance(number, digit, 0) != 0);  /*We send 0 as third parameter because we are not interested in the digit's place.*/

            /*When generating the very last digit, we check if it is generated as 0.
            If it is, then we create a new random digit other than 0 to prevent the number from having 1 less digit
            than we desire.*/
            if(i == numOfDigits && digit == 0){
                while(checkExistance(number, digit, 0) != 0 && digit == 0){
                    digit = rand() % 10;
                }
            }
            /*The current digit is multiplied by coef and added to current number.
            Then, coefficient is multiplied by 10 for the next use.*/
            number += coef*digit;
            coef *= 10;
        }

    /*The random number generated is returned.*/
    return number;
}


/*This function counts the digits of the number. It takes the number as argument and divide it by 10 until reaching 0.
The divisions are counted and its final value, the number of digits, is returned. We will use this function for error checking.*/
int countDigits(int number){
    int counter = 0;

    while(number != 0){
        number /= 10;
        counter++;
    }
    
    return counter;
}



/*This function is the mastermind game. It takes the number to be guessed as argument.*/
void mastermind(int number){

    int userInput,      /*The user's guess.*/
        firstCount,     /*firstCount and secoundCount are counted into these.*/
        secondCount,
        iterations = 1, /*The number of iterations. Its initial value is 1.*/
        placeCounter,   /*placeCounter is for counting the digits we cover. 
                        We will send this to checkExistance function as 3rd parameter.*/
        numOfDigits;    /*The number of digits in the number to be guessed, will be used for error checking.*/
    
    /*number's number of digits is assigned to numOfDigits.*/
    numOfDigits = countDigits(number);

    /*The loop control is provided by return statements.*/
    while(1){

        /*If iterations passes 100, fail message is printed and the program is terminated.*/
        if(iterations > FAILSTATE){
            cout << "FAILED" << endl;
            return;
        }

        /*If user enters something other than an integer, E2 error message is printed, the program is terminated.*/
        if(!(cin >> userInput)){
            cout << "E2" << endl;
            return;
        }

        /*If the user's guess is not a unique number, E1 error message is printed, the program is terminated.*/
        if(uniqueCheck(userInput) != true){
            cout << "E1" << endl;
            return;
        }

        /*If user guesses the number right, the FOUND message is printed, the program is terminated.*/
        if(userInput == number){
            cout << "FOUND " << iterations << endl;
            return;
        }

        /*If the user's guess has different amount of digits from the number, E1  error message is printed, the program is terminated.*/
        if(countDigits(userInput) != numOfDigits){
            cout << "E1" << endl;
            return;
        }

        /*If the game continues, these variables are prepared for checking the current guess.*/
        firstCount = 0;
        secondCount = 0;
        placeCounter = 1;

        /*We will get the digit with % 10 operation and divide by 10 after covering current digit 
        until reaching 0, like we did in previous functions.*/
        while(userInput != 0){

            /*If checkExistance function returns 2, the current digit from guess matches with the number.
            firstCount is inremented.*/
            if(checkExistance(number, userInput%10, placeCounter) == 2)
                firstCount++;
            /*Else if checkExistance function returns 1, the current digit from guess exists in the number, but in a different place.
            secondCount is incremented.*/
            else if(checkExistance(number, userInput%10, placeCounter) == 1)
                secondCount++;

            /*Guess is divided by 10, the placeCounter is incremented.*/
            userInput /= 10;
            placeCounter++;
        }

        /*firstCount and secondCount are printed, iterations is incremented.*/
        cout << firstCount << " " << secondCount << endl;
        iterations++;
    }
}