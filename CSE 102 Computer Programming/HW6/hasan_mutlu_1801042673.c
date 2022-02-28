#include <stdio.h>
#include <string.h>

#define OBJSIZE 21  /*Object name size.*/
#define BUFSIZE 100 /*Entire input text max size.*/
#define PROPERTYSIZE 10 /*Program can work for any kind of property, and this is property's size.*/
#define PROPERTY "cost" /*For the example in PDF, the property is "cost". Any other property of the object can be printed by changing this definition.*/

/*The function works recursively.
It takes amount of object for calculation and parent object's name to be used in "define obj in obj?:" output.
If new objects are entered, those objects are sent to function with their amount.
If price of an object is entered, it is multiplied by amount of that object and returned to the parent object's cost sum.
Parent objects' costs are also multiplied with their amount and returned.*/
float func(float num, char *prnt);

void main(){
    char buffer[OBJSIZE], property[PROPERTYSIZE] = PROPERTY;
    printf("Name the object:\n");
    scanf("%[^\n]%*c", buffer);     /*Reading object name.*/
    printf("Define %s?:\n", buffer);
    /*Object is sent to function and result is printed. We calculate cost of one object so 1 is sent as amount.*/
    printf("Total %s of %s is %.1f\n",property, buffer, func(1,buffer));
}

float func(float num, char *prnt){
    /*buffer will store entire text inputted. obj will store object's name.*/
    char buffer[BUFSIZE], obj[OBJSIZE], *p;
    /*numprice is either amount or price of the object. result is sum of child object's prices.*/
    float numprice, result=0;
    /*p pointer will move forwards when one object is sent to function.*/
    p=buffer;
    /*Entire text inputted is stored in buffer array. A null character is put at the end to indicate end of array.*/
    scanf("%[^\n]%*c", buffer);
    buffer[strlen(buffer)] = '\0';
    /*Until reaching end of array, entire text will be covered.*/
    while(*p!='\0'){
        /*If sscanf returns only one, it means that the price of the object is entered. It is multiplied by object's amount and returned.*/
        if(sscanf(p,"%f %s", &numprice, obj)==1){
            return num*numprice;
        }
        /*Else, it means that new child objects are entered. Amount and name of the objects are scanned and sent to the function.
        Returned value is added to result variable.*/
        else{
            printf("Define %s in %s?:\n", obj, prnt);
            result += func(numprice, obj);
        }
        /*Next commands move array pointer to next object's beginning.
        Pointer is moved to beginning of current object's name with this function.*/
        p = strstr(buffer, obj);
        /*Pointer is moved beyond the object's name by moving as much as it's length.*/
        p += strlen(obj);
        /*Then, pointer is moved forwards until it reaches a character which is not any kind of whitespace.
        This while loop prevents program from infinite loop when there is whitespace after entire text. For example: "1 handle     "*/
        while(*p==' ' || *p=='\t' || *p=='\v' || *p=='\f' || *p=='\r')
            p++;
    }
    /*After costs of all child objects are summed, the sum is multiplied by amount and returned to parent object's sum.*/
    return num*result;
}

