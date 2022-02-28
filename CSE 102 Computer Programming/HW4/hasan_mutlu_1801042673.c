#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

/*Note: The program works with '^' as the power sign.*/

/*The polynom and the monomial sizes.*/
#define POLISIZE 1001
#define MONOSIZE 20

/*Program finds the results by recursively calling this polynom function.
It takes the polynom array,
the value from value.txt file,
the sign of the current monomial
and 'i' variable to use as an index to hold current position.*/
double  polynom(char poly[POLISIZE], double value, char sign, int i);


void main(){
    FILE *fp1, *fp2, *fp3;
    double value;
    char poly[POLISIZE], nextsign = '+', c;
    int i = 0;
    memset(poly, '\0', POLISIZE);

    fp1 = fopen ("values.txt", "r+");
    fp2 = fopen ("polynomial.txt", "r+");
    fp3 = fopen ("evaluations.txt", "w+");

    if (fp1 == NULL) {
         printf("File1 opening error!");
    }
    if (fp2 == NULL){
        printf("File2 opening error!");
    }
    if (fp3 == NULL){
        printf("File3 opening error!");
    }

    /*The polynom is assigned into poly array without the whitespaces.*/
    while(fscanf(fp2, "%c", &c) != EOF){
        if(c==' ' || c=='\t' || c=='\n' || c=='\f' || c=='\v')
            continue;
        poly[i] = c;
        i++;
    }

    /*A '\0' character is assigned at the end of the array to indicate the end of array.*/
    poly[strlen(poly)] = '\0';

    /*nextsign variable indicates the sign of the first monomial.
    nextsign's initial value is '+' and if the very first character in poly array is '-', it turns to '-'.*/
    if(poly[0] == '-')
        nextsign = '-';

    /*Values are called from the file and the results are fpritned.*/
    while(fscanf(fp1, "%lf", &value)!=EOF){
        fprintf(fp3, "%.2lf\n", polynom(poly, value, nextsign, 0));
    }

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}

double  polynom(char poly[POLISIZE], double value, char sign, int i){
    char c = '\0', mono[MONOSIZE], x ='\0', nextsign;
    int j=0, nocoef;
    double coef = 0, pwr;
    memset(mono, '\0', MONOSIZE);
    /*If the sign is '\0', it means that we reached the end of the polynom and 0 is returnder.*/
    if(sign == '\0'){
        return 0;
    }
    /*One monomial is assigned into mono array.*/
    while(poly[i] != '+' && poly[i] != '-' && poly[i] != '\0'){
        mono[j] = poly[i];
        j++;
        i++;
    }
    /*The current poly[i] character, which is either +,- or \0 is assigned to nextsign variable.
    i index is incremented for next use.*/
    nextsign = poly[i];
    i++;

    /*If this sscanf function returns 1, it means that the current monomial includes a coefficient.*/
    if(sscanf(mono, "%lf", &coef)){
        /*If this sscanf function returns only 1, it means that the current monomials only includes a coefficient.*/
        if(sscanf(mono, "%lf %c %c %lf", &coef, &x, &c, &pwr) == 1){
            /*Only coefficient is returned according to its sign and the next function is called recursively.*/
            if(sign == '+')
                return (coef + polynom(poly, value, nextsign, i));
            else if(sign == '-')
                return (- coef + polynom(poly, value, nextsign, i));
        }

        else{
            /*Else, coefficient, power sign and power are scanned.*/
            sscanf(mono, "%lf %c %c %lf", &coef, &x, &c, &pwr);
            /*If true, it means that the monomial has no power.
            coef*value is returned with correct sign and next function is called..*/
            if(c != '^'){
                if(sign == '+')
                    return (coef*value + polynom(poly, value, nextsign, i));
                else if(sign == '-')
                    return (- coef*value + polynom(poly, value, nextsign, i));
            }
            /*If true, it means that there is power.
            coef*value^pwr is returned with correct sign and next function is called.*/
            else if(c == '^'){
                if(sign == '+')
                    return (coef*pow(value,pwr) + polynom(poly, value, nextsign, i));
                else if(sign == '-')
                    return (- coef*pow(value,pwr) + polynom(poly, value, nextsign, i));
            }
        }
    }

    /*Else, it means that there isn't coefficient.*/
    else{
        /*power sign and power are scanned. The correct result is returned same as above.*/
        sscanf(mono, "%c %c %lf", &x, &c, &pwr);
        if(c != '^'){
            if(sign == '+')
                return (value + polynom(poly, value, nextsign, i));
            else if(sign == '-')
                return (- value - polynom(poly, value, nextsign, i));
        }
        else if(c == '^'){
            if(sign == '+')
                return (pow(value,pwr) + polynom(poly, value, nextsign, i));
            else if(sign == '-')
                return (- pow(value,pwr) + polynom(poly, value, nextsign, i));
        }
    }
}
