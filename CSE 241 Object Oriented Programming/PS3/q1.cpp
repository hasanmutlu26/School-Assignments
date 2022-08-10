#include <iostream>

using namespace std;

class Point{
    public:
        void set(double first, double second);
        void move(double first, double second);

        void rotate();

        double first();
        double second();

    private:
        double x;
        double y;
};

double Point::first(){
    return x;
}

double Point::second(){
    return y;
}

void Point::set(double first, double second){
    x = first;
    y= second;
}

void Point::move(double first, double second){
    x += first;
    y += second;
}

void Point::rotate(){
    double temp = x;
    x = -y;
    y = temp;
}

int main(){
    Point A, B, C;

    A.set(1,2);
    cout << A.first() << ", " << A.second() << endl;
    A.rotate();
    cout << A.first() << ", " << A.second() << endl;
    A.rotate();
    cout << A.first() << ", " << A.second() << endl;
    A.rotate();
    cout << A.first() << ", " << A.second() << endl;
    A.rotate();
    cout << A.first() << ", " << A.second() << endl;
    A.rotate();
    cout << A.first() << ", " << A.second() << endl;

    B.set(2,3);
    cout << B.first() << ", " << B.second() << endl;
    B.move(1,1);
    cout << B.first() << ", " << B.second() << endl;
    

    C.set(5, -4);
    cout << C.first() << ", " << C.second() << endl;
    cout << "Move C by -5 horizontally and 4 vertically." << endl;
    C.move(-5, 4);
    cout << C.first() << ", " << C.second() << endl;

    return 0;
}