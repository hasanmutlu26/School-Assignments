#include <iostream>

using namespace std;

int main(){
	int n,j;
	bool isPrime;

	for(n=3; n<=100; n++){

		isPrime = true;
		for(j=2; j<n; j++){

			if(n % j == 0){
				isPrime = false;
				/*if number is not prime, j is set to n to finish looping.*/
				j=n;			
			}
		}

	if(isPrime)
		cout << n << " is a prime number." << endl;
	}
}