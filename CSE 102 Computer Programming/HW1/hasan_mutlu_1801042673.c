#include<stdio.h>

/*Hasan Mutlu - 1801042673*/

/*This is to define the class capacity. */
#define CLASS 10

void main(){
    float
        /* x and y variables will be used for user input.*/
        x, y,
        /*These four variables will be used to determine the average points of classes.*/
        class1_x = 0, class1_y = 0,
        class2_x = 0, class2_y = 0,
        /*These two variables will be used to hold midpoint of class average points.*/
        midpoint_x, midpoint_y,
        /*These two variables will be used to calculate slopes of connecting and separating lines.*/
        slope_con, slope_sep;

    printf("Please enter class1 points:\n");

    /*In this loop, user inputs coordinates of class 1 and all of the coordinates are summed.*/
    for(int i = 1; i<= CLASS; i++){
        scanf("%f", &x);
        scanf("%f", &y);
        class1_x = class1_x + x;
        class1_y = class1_y + y;
    }
    /*After summation of x and y coordinates, average point of class 1 is calculated.*/
    class1_x = class1_x / CLASS;
    class1_y = class1_y / CLASS;
    printf("Please enter class2 points:\n");

    /*In this loop, user inputs coordinates of class 2 and all of the coordinates are summed.*/
    for(int i = 1; i<= CLASS; i++){
        scanf("%f", &x);
        scanf("%f", &y);
        class2_x = class2_x + x;
        class2_y = class2_y + y;
    }
    printf("Please enter your test points:\n");
    /*After summation of x and y coordinates, average point of class 2 is calculated.*/
    class2_x = class2_x / CLASS;
    class2_y = class2_y / CLASS;

    /*Midpoint of class average points is calculated.*/
    midpoint_x = (class1_x + class2_x)/2;
    midpoint_y = (class1_y + class2_y)/2;

    /*In next if-else if blocks, user will input the points and
    the program will determine which class is that point is in.*/

    /*Next two if blocks are to prevent 0/0 situation.*/
    /*When x coordinates of class average points are equal,
    it means that the seperating line will be horizontal.
    Only y coordinate of the point matters.*/
    if(class1_x == class2_x){
        while(1){
            /*User enters coordinates.*/
            scanf("%f", &x);
            scanf("%f", &y);
            /*If user enters an unexpected data (character), program ends.*/
            if(getchar()!= '\n')
                break;
            /*If y coordinates of midpoint and user's point are equal,
            it means that the point is on the seperating line.*/
            if(y == midpoint_y){
                printf("on separating line\n");
            }
            /*If y coordinate of user's point and one of the classes average point are both bigger than midpoint's y coordinate,
            it means that the point is in that class.*/
            else if(y > midpoint_y){
                if(class1_y > midpoint_y)
                    printf("class1\n");
                else if(class2_y > midpoint_y)
                    printf("class2\n");
            }
            /*Same situation like above where y coordinates are both smaller than midpoint's y coordinate.*/
            else if(y < midpoint_y){
                if(class1_y < midpoint_y)
                    printf("class1\n");
                else if(class2_y < midpoint_y)
                    printf("class2\n");
            }
        }
    }
    /*When y coordinates of class average points are equal,
    it means that the seperating line will be vertical.
    Only y coordinate of the point matters.*/
    else if(class1_y == class2_y){
        while(1){
            /*User enters coordinates.*/
            scanf("%f", &x);
            scanf("%f", &y);
            /*If user enters an unexpected data (character), program ends.*/
            if(getchar()!= '\n')
                break;
            /*If x coordinates of midpoint and user's point are equal,
            it means that the point is on the seperating line.*/
            if(x == midpoint_x){
                printf("on separating line\n");
            }
            /*If x coordinate of user's point and one of the classes average point are both bigger than midpoint's x coordinate,
            it means that the point is in that class.*/
            else if(x > midpoint_x){
                if(class1_x > midpoint_x)
                    printf("class1\n");
                else if(class2_x > midpoint_x)
                    printf("class2\n");
            }
            /*Same situation like above where x coordinates are both smaller than midpoint's x coordinate.*/
            else if(x < midpoint_x){
                if(class1_x < midpoint_x)
                    printf("class1\n");
                else if (class2_x < midpoint_x)
                    printf("class2\n");
            }
        }
    }
    /*If there is no possibility of 0/0 situation, result is determined by line equations.*/
    else{
        /*Slope of connecting line is calculated. Slope = (y2 - y1) / (x2 - x1)*/
        slope_con = (class2_y - class1_y) / (class2_x - class1_x);
        /*Slope of seperating line is calculated.*/
        slope_sep = - (1 / slope_con);

        while(1){
            /*User enters coordinates.*/
            scanf("%f", &x);
            scanf("%f", &y);
            /*If user enters an unexpected data (character), program ends.*/
            if(getchar()!= '\n')
                break;
            /*Classes are determined by using the equation (y-midpoint_y)=m(x-midpoint_x)*/
            /*If the equation is satisfied, it means that the point is on the separating line.*/
            if ((y - midpoint_y) == (slope_sep*(x - midpoint_x)))
                printf("on separating line\n");
            /*If there is inequality, point's class is determined by using inequality.*/
            /*If both the user's point's coordinates and one of the classes coordinates satisfy the same inequality,
            it means that the point is in that class.*/
            /*If the inequality where the left side is smaller is satisfied for both user's point and one of classes average point,
            it means that the point is in that class.*/
            else if ((y - midpoint_y) < (slope_sep*(x - midpoint_x))) {
                    if((class1_y - midpoint_y) < (slope_sep*(class1_x - midpoint_x)))
                        printf("class1\n");
                    else if((class2_y - midpoint_y) < (slope_sep*(class2_x - midpoint_x)))
                        printf("class2\n");
            }
            /*If the inequality where the left side is bigger  is satisfied for both user's point and one of classes average point,
            it means that the point is in that class.*/
            else if (((y - midpoint_y) > (slope_sep*(x - midpoint_x)))) {
                    if ((class1_y - midpoint_y) > (slope_sep*(class1_x - midpoint_x)))
                        printf("class1\n");
                    else if ((class2_y - midpoint_y) > (slope_sep*(class2_x - midpoint_x)))
                        printf("class2\n");
            }
        }
    }
}
