#include <iostream>
#include <cstdlib>

using namespace std;

class HotDogStand{
    public:
        HotDogStand();
        HotDogStand(int newID, int newNumSold);
        int getID();
        void setID(int newID);
        void justSold();
        int GetNumSold();
        static int GetTotalSold();

    private:
        static int totalSold;
        int numSold;
        int ID;
};

int HotDogStand::totalSold = 0;

HotDogStand::HotDogStand(){
    numSold = 0;
    ID = 0;
}

HotDogStand::HotDogStand(int newID, int newNumSold){
    numSold = newNumSold;
    ID = newID;
}

int HotDogStand::getID(){
    return ID;
}

void HotDogStand::setID(int newID){
    ID = newID;
}

void HotDogStand::justSold(){
    numSold++;
    totalSold++;
}

int HotDogStand::GetNumSold(){
    return numSold;
}

int HotDogStand::GetTotalSold(){
    return totalSold;
}

int main(){
    HotDogStand s1(1,0), s2(2,0), s3(3,0);

    s1.justSold();
    s2.justSold();
    s1.justSold();

    cout << "Stand " << s1.getID() << " sold " << s1.GetNumSold() << endl;
    cout << "Stand " << s2.getID() << " sold " << s2.GetNumSold() << endl;
    cout << "Stand " << s3.getID() << " sold " << s3.GetNumSold() << endl;
    cout << "Total sold = " << HotDogStand::GetTotalSold() << endl;
    cout << endl;

    s3.justSold();
    s1.justSold();

    cout << "Stand " << s1.getID() << " sold " << s1.GetNumSold() << endl;
    cout << "Stand " << s2.getID() << " sold " << s2.GetNumSold() << endl;
    cout << "Stand " << s3.getID() << " sold " << s3.GetNumSold() << endl;
    cout << "Total sold = " << HotDogStand::GetTotalSold() << endl;
    cout << endl;
    
    return 0;
}