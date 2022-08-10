#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int NUM_DUELS = 10000;

void aaron_shoots(bool aaron, bool& bob, bool& charlie);
void bob_shoots(bool& aaron, bool bob, bool& charlie);
void charlie_shoots(bool& aaron, bool& bob, bool charlie);



int main(){
    bool aaronAlive, bobAlive, charlieAlive;
    int aaronWins = 0,
        bobWins = 0,
        charlieWins = 0,
        i;

    srand(time(NULL));


    for(i=0;i < NUM_DUELS; i++){
        aaronAlive = true;
        bobAlive = true;
        charlieAlive = true;

        do{
            aaron_shoots(aaronAlive,bobAlive,charlieAlive);
            bob_shoots(aaronAlive,bobAlive,charlieAlive);
            charlie_shoots(aaronAlive,bobAlive,charlieAlive);
        } while (aaronAlive && bobAlive || aaronAlive && charlieAlive || bobAlive && charlieAlive);

        if(aaronAlive)
            aaronWins++;
        else if(bobAlive)
            bobWins++;
        else if(charlieAlive)
            charlieWins++;
    }

    cout << "Aaron won " << aaronWins << "/10000 duels or " << (static_cast<double>(aaronWins) / NUM_DUELS)*100 << "%" << endl;
    cout << "Bob won " << bobWins << "/10000 duels or " << (static_cast<double>(bobWins) / NUM_DUELS)*100 << "%" << endl;
    cout << "Charlie won " << charlieWins << "/10000 duels or " << (static_cast<double>(charlieWins) / NUM_DUELS)*100 << "%" << endl;
   
}



void aaron_shoots(bool aaron, bool& bob, bool& charlie){
    int shot;
    
    if (aaron == false)
        return;

    shot = rand() % 3;

    if(shot == 0){
        if(charlie)
            charlie = false;
        else if(bob)
            bob = false;
    }

    return;
}

void bob_shoots(bool& aaron, bool bob, bool& charlie){
    int shot;

    if(bob == false)
        return;

    shot = rand() % 2;

    if(shot == 0){
        if(charlie)
            charlie = false;
        else if(aaron)
            aaron = false;
    }
    return;
}

void charlie_shoots(bool& aaron, bool& bob, bool charlie){
    if(charlie == false)
        return;

    if(bob)
        bob = false;
    else if(aaron)
        aaron = false;

    return;
}