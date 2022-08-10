#include <iostream>
#include <ctime>

/*I added screenshot for every possible scenario.*/

using namespace std;

int rollDice();
int humanTurn(int humanTotalScore);
int computerTurn(int computerTotalScore);


int main(){
	bool gameOver = false;
	int humanScore = 0,
		computerScore = 0;

	srand(static_cast<int>(time(NULL)));

	while(!gameOver){
		humanScore += humanTurn(humanScore);

		if(humanScore < 100){
			computerScore += computerTurn(computerScore);
		}

		if(humanScore >= 100){
			cout << "Congratulations! You win!" << endl;
			gameOver = true;
		}

		else if(computerScore >= 100){
			cout << "Sorry, the computer won." << endl;
			gameOver = true;
		} 
	}
	
}


int rollDice(){
	return ((rand() % 6) + 1);
}

int humanTurn(int humanTotalScore){
	int score = 0, roll;
	char command;
	bool turnOver = false;

	cout << endl << endl << "It's your turn. Enter 'r' to roll." << endl;
	cin >> command;

	do {
		roll = rollDice();
		cout << endl << "You rolled " << roll << endl;

		if(roll == 1){
			cout << "You lose your turn. Enter 'c' to continue." << endl;
			cin >> command;
			return 0;
		}

		else{
			score += roll;
			cout << "Your score this round is: " << score << endl;
			cout << "If you hold, your total score will be: " << score + humanTotalScore << endl;
			cout << "Press 'h' to hold or 'r' to roll again." << endl;
			cin >> command;

			if(command == 'h')
				turnOver = true;
		}
	} while (!turnOver);

	return score;
}




int computerTurn(int computerTotalScore){
	int score = 0, roll;
	char command;
	bool turnOver = false;

	cout << endl << endl << "It's the computer's turn!" << endl;

	do{
		roll = rollDice();
		cout << endl << "Computer rolled " << roll << endl;

		if(roll == 1){
			cout << "The computer loses its turn. Press 'c' to continue." << endl;
			cin >> command;
			return 0;
		}

		else{
			score += roll;
			cout << "The computer's score this round is: " << score << endl;
			cout << "If the computer holds, its total score will be: " << score + computerTotalScore << endl;
			
			if (score >= 20 || score + computerTotalScore >= 100){
				cout << "Computer holds. Enter 'c' to continue." << endl;
				cin >> command;
				turnOver = true;
			}
		}
	} while (!turnOver);

	return score;
}