#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TEXTSIZE 251 /*TEXTSIZE is 251 instead of 250 because a '\0' character will be put in the end of text.*/
#define NODESIZE 10

/*function takes a pointer to array, an integer 'dash' to decide how many '-' will be fprinted, and the file pointer to fprint.*/
void func(char* tp, int dash, FILE* fp2);

void main(){
    FILE *fp1, *fp2;
    char c, text[TEXTSIZE], *tp;
    int i=0;
    fp1 = fopen ("input.txt", "r+");
    fp2 = fopen ("output.txt", "wt");
    memset(text, '\0', TEXTSIZE);

    /*The text in input.txt is assigned into an array.*/
    while(fscanf(fp1, "%c", &c) != EOF){
        text[i] = c;
        i++;
    }
    text[i] = '\0';     /*A '\0' character is put to indicate end of array.*/

    tp = &text[0];      /*Pointer is assigned to array.*/
    func(tp, 0, fp2);   /*Function is called. Initial value for dash is 0.*/

    fclose(fp1);
    fclose(fp2);
}

void func(char* tp, int dash, FILE* fp2){
    char node[NODESIZE];
    int i=0, nextdash;
    memset(node, '\0', NODESIZE);

    nextdash = dash;    /*nextdash variable will be used as the argument of next function.*/

    /*when *tp doesn't reach end of text, parse the text.*/
    if(*tp!='\0'){
        /*Until reaching comma or end of text, scan the current node by one character at a time.*/
        while(*tp!=',' && *tp!='\0'){
            if(*tp == '('){     /*If '(' is current character, dash and nextdash variables are incremented.*/
                dash++;
                nextdash++;
               }
            else if(*tp == ')') /*If ')' is current character, only nextdash is decremented.*/
                nextdash--;
            else{
                node[i] = *tp;  /*Else, assign the current character into node array as it is a character in node name.*/
                i++;
            }
            tp++;   /*Go to next character of the text.*/
        }

        for(i=0;i<dash;i++)     /*Print '-' as much as dash variable.*/
            fprintf(fp2, "-");

        for(i=0;node[i]!='\0';i++){     /*Print the node name.*/
            fprintf(fp2, "%c", node[i]);
            }

        fprintf(fp2, "\n");     /*Print next line.*/

        tp++;       /*Go to next character of the text to start scanning next node.*/
        func(tp, nextdash, fp2);    /*Call the function.*/
    }

    /*If pointer to array reaches '\0' character, the function will do nothing and the recursion will end.*/
}
