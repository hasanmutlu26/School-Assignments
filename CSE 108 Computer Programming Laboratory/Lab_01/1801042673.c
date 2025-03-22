/* Hasan Mutlu - 1801042673 */

#include <stdio.h>


void trigonometry_calculation(int a, int b, int c)
{
    float sinx, cosx, tanx;

    /*Trigonometric values are calculated.*/
    sinx = (float)a/(float)c;
    cosx = (float)b/(float)c;
    tanx = (float)a/(float)b;

    /*Results are printed.*/
    printf("sinx = %f\n", sinx);
    printf("cosx = %f\n", cosx);
    printf("tanx = %f\n\n", tanx);

    return;
}

void trigonometry(int a, int b, int c)
{
    /*Values are scanned.*/
    printf("a: ");
    scanf("%d", &a);

    printf("b: ");
    scanf("%d", &b);
    
    printf("c: ");
    scanf("%d", &c);

    /*Calling trigonometry_calculation function.*/
    trigonometry_calculation(a,b,c);
    
    return;
}



/*I changed the function's return type to unsigned long because 
it is not possible to keep the correct answer while the function is returning int.*/
unsigned long age_calculation(int x)
{
    unsigned long seconds;
    /*Seconds are calculated. 1 year = 31536000 seconds.*/
    seconds = 31536000*(unsigned long)x;
    /*Result is returned.*/
    return seconds;
}



void age(int x)
{   
    /*Age is scanned.*/
    printf("Your age: ");
    scanf("%d", &x);

    /*age_calculation function is called and returned value is printed.*/
    printf("You are %ld seconds\n", (unsigned long)age_calculation(x));
    return;
}





int main()
{
    int a, b, c, x;

    /*Calling trigonomentry function.*/
    trigonometry(a,b,c);    
    
    /*Calling age function.*/
    age(x);
    return 0;
}
