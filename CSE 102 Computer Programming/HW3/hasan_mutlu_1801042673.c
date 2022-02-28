#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define LINESIZE 250 /*LINESIZE is for maximum amount of character per line in input2.txt*/
#define WORDSIZE 10 /*WORDSIZE is for maximum length of the searched word from input1.txt.*/

void HorizontalSearch(char a[WORDSIZE], FILE* fp2, FILE* fp3);
void VerticalSearch(char a[WORDSIZE], FILE* fp2, FILE* fp3);

void main(){
    FILE *fp1, *fp2, *fp3;
    char a[WORDSIZE];
    fp1 = fopen ("input1.txt", "r+");
    fp2 = fopen ("input2.txt", "r+");
    fp3 = fopen ("output.txt", "w+");

    if (fp1 == NULL) {
         printf("File1 opening error!");
    }
    if (fp2 == NULL){
        printf("File2 opening error!");
    }
    if (fp3 == NULL){
        printf("File3 opening error!");
    }
    /*scan the words in the input1.txt and send them to searching functions with file pointers.
    Reset the fp2 pointer back to beginning of file after each search.*/
    while (fscanf(fp1, "%s", a) != EOF){
        HorizontalSearch(a,fp2,fp3);
        fseek(fp2, 0,SEEK_SET);
        VerticalSearch(a,fp2,fp3);
        fseek(fp2, 0,SEEK_SET);
    }
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
}




/*This function gets a string long as word's length,
if it is not the word we search for,
goes backwards 1 less than word's length and get the next string.*/

void HorizontalSearch(char a[WORDSIZE], FILE* fp2, FILE* fp3){
    char b[WORDSIZE];             /*We will search into 'b' array.*/
    memset(b, '\0', WORDSIZE);    /*reset the array to prevent possible errors.*/
    int i, j, x, y, flag = 1;
    x=1; y=1;
    /*Flag will be 0 when the word is found*/
    while(flag){
        /*scan characters as much as strlen(a) into 'b' array*/
        for(i = 0; i<strlen(a); i++){
            fscanf(fp2, "%c", &b[i]);
            if(feof(fp2))
                break;
        }
        /*If the the word is found, print into the output file.*/
        if(strcmp(a,b)==0){
                flag = 0;
                fprintf(fp3, "%s (%d,%ld) Horizontal\n", a, y, x-(strlen(a)-1));
                memset(b, '\0', WORDSIZE);
            }
        /*We increment the x coordinate.*/
        x++;
        /*If last element of b array is newline, we increment y coordinate and reset the x coordinate.*/
        if(b[strlen(a)-1] == '\n'){
            y++;
            x=1;
        }
        /*If the file pointer reaches fp2 at that point, it means that the word is not found.*/
        if(feof(fp2))
            break;
        /*Otherwise, move file pointer backwards 1 less than word's length*/
        fseek(fp2, -(strlen(a)-1), SEEK_CUR);
    }
}





/*This function gets lines much as word's length into two dimensional buffer array,
create a string from vertical characters and checks if the word is found.
If the word is not found, searching continues starting from the next line.*/

void VerticalSearch(char a[WORDSIZE], FILE* fp2, FILE* fp3){
    char b[WORDSIZE], buffer[WORDSIZE][LINESIZE]; /*buffer is for scanning from file and b is for creating string from vertical chars.*/
    int i, j, x, y, flag = 1;
    memset(b, '\0', WORDSIZE);        /*arrays are reset to prevent possible errors.*/
    memset(buffer, '\0', LINESIZE);
    x=0; y=1;
    while(!feof(fp2)){
        /*Flag will be 0 when the word is found or a newline character is assigned to b array*/
        while(flag){
            /*scan lines into buffer and create strings from vertical chars.*/
            for(i=0;i<strlen(a);i++){
                fgets(buffer[i], LINESIZE, fp2);
                b[i] = buffer[i][x];
                if(b[i] == '\n')
                    flag=0;
            }
            /*If the word is found, print into output file.*/
            if(strcmp(a,b)==0){
                fprintf(fp3, "%s (%d,%d) Vertical\n", a, y, x+1);
                flag=0;
            }
            /*Else, reset arrays, send file pointer to beginning and move it '1 less than current y coordinate' lines and increment x*/
            else{
                memset(b, '\0', WORDSIZE);
                memset(buffer, '\0', LINESIZE);
                fseek(fp2, 0,SEEK_SET);
                for(j=1;j<y;j++)
                    fgets(buffer[i], LINESIZE, fp2);
                x++;
            }
        }
        /*If one line is ended, increment y, reset x back to 0*/
        y++;
        x=0;
        /*send file pointer to beginning and move it '1 less than current y coordinate' lines*/
        fseek(fp2, 0,SEEK_SET);
        for(j=1;j<y;j++)
            fgets(buffer[i], LINESIZE, fp2);
        /*Reset the flag and arrays.*/
        flag=1;
        memset(b, '\0', WORDSIZE);
        memset(buffer, '\0', LINESIZE);
    }
}
