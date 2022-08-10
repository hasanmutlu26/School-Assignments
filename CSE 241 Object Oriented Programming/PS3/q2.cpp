#include <iostream>

using namespace std;

class Fraction{
    public:
        double getDouble();
        void outputReducedFraction();
        void setNumerator(int n);
        void setDenominator(int d);

    private:
        int numerator;
        int denominator;
        int gcd();
};

double Fraction::getDouble(){
    return (static_cast<double>(numerator) / denominator);
}

void Fraction::outputReducedFraction(){
    int g;
    g = gcd();
    cout << numerator / g << " / " << denominator / g << endl;
    return; 
}

void Fraction::setNumerator(int n){
    numerator = n;
}

void Fraction::setDenominator(int d){
    denominator = d;
}

int Fraction::gcd(){
    int g;

    if(numerator / denominator)
        g = denominator;

    else
        g = numerator;
    
    while(g > 1){
        if(((numerator % g) == 0) && ((denominator % g) == 0))
            return g;
        g--;
    }

    return 1;
}

int main(){
    Fraction f1, f2;

    f1.setNumerator(4);
    f1.setDenominator(2);
    cout << f1.getDouble() << endl;
    f1.outputReducedFraction();

    cout << endl;

    f2.setNumerator(20);
    f2.setDenominator(60);
    cout << f2.getDouble() << endl;
    f2.outputReducedFraction();
    cout << endl;

}