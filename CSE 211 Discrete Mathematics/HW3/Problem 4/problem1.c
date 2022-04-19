#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Hasan Mutlu - 1801042673*/
/*The algorithms from HW2 were highly implemented in this homework.*/

#define ASIZE 15    /*ASIZE and BSIZE are for maximum possible number elements in A and R sets.*/
#define BSIZE 15
#define SETNUMBER 8 /*SETNUMBER is for number of sets in input file. It is 8 in the one sent by teacher.*/



void readInput(FILE* fp1, FILE* fp2);
void fprintPOSET(FILE* fp2, char A[ASIZE], char B[BSIZE][3], int n);
void fprintHase(FILE* fp2, char A[ASIZE], char B[BSIZE][3], int n);



void main(){
    FILE *fp1, *fp2;
    fp1 = fopen ("input.txt", "r+");
    fp2 = fopen ("output.txt", "w+");
    for(int i=1; i<=SETNUMBER;i++){ /*readInput function is called as much as number of sets in input file.*/
        readInput(fp1, fp2);
    }
    fclose(fp1);
    fclose(fp2);
}



void readInput(FILE* fp1, FILE* fp2){
    char A[ASIZE], B[BSIZE][3], c;
    int flag;
    memset(A, '\0', ASIZE);
    int n, i=0, j=0, k, ALen;
    fscanf(fp1, "%d", &n);  /*n is read.*/
    fgetc(fp1);
    while(1){                   /*A set is assigned to A array.*/
        fscanf(fp1, "%c", &c);
        if(c=='\n')
            break;
        A[i] = c;
        i++;
    }
    ALen = strlen(A)/2+1;   /*Number of elements in set A is found as ,'s are also read into array.*/
    for(i=0; i<n; i++){     /*Relations are assigned to two dimensional B array*/
        for(j=0;j<=2;j++){
            fscanf(fp1, "%c", &c);
            B[i][j]=c;
        }
        fgetc(fp1);
    }

    fprintPOSET(fp2,A,B,n);
    fprintHase(fp2,A,B,n);

}

/*This function fprints the POSET. The loops are implemented from HW2.*/
void fprintPOSET(FILE* fp2, char A[ASIZE], char B[BSIZE][3], int n){
    int i,j,k, flag;

    /*Symmetrics are deleted. A '-' char is assigned instead of the deleted ones.*/
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            /*If i==j, it means that it is same pair, don't compare it with itself.*/
            if(i==j)
                continue;
            if (B[i][0] == B[j][2] && B[i][2] == B[j][0]){
                B[i][0] = '-';
                B[i][1] = '-';
                B[i][2] = '-';
            }
        }
    }

    /*Adding new pairs to make the relation transitive is not implemented, sadly*/

    /*After deletions, missing reflexive pairs are put at the end.*/
    for(j=0;j<n;j=j+2){
        flag=0;
        for(i=0;i<n;i++){
            if((B[i][0] == B[i][2]) && (B[i][2] == A[j])){
                flag=1;
            }
        }
        /*If the flag comes out as 0, it means that the character in A[j] is the missing one for reflexivity. A new pair is added at the end.*/
        if(flag==0){
            B[i][0] = A[j];
            B[i][1] = ',';
            B[i][2] = A[j];
            n++;    /*n is incremented as the number of pairs could have been incremented.*/
        }
    }

    /*Print part.*/
    fprintf(fp2,"n\nPOSET: ");
    for(i = 0;i<n;i++){
        if(B[i][0] == '-')  /*If the character is '-', it means that it is deleted, don't print it.*/
            continue;
        /*Else, print the current pair with the correct format.*/
        else{
            fprintf(fp2,"(");
            fprintf(fp2, "%c%c %c), ",B[i][0],B[i][1],B[i][2]);
        }
    }
    fprintf(fp2,"\n");

}


/*This function prints the Hase diagram. It includes symmetric loops from the previous function and transitive loop from HW2.*/
void fprintHase(FILE* fp2, char A[ASIZE], char B[BSIZE][3], int n){
    int i,j,k,flag;

    /*This part checks transivity. If transivity is found, the shorter edge is deleted by putting '-' in it.*/
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            for(k=0;k<n;k++){
                if(i==j || j==k)
                    continue;
                if(B[i][0] == B[i][2] || B[j][0] == B[j][2] || B[k][0] == B[k][2])
                    continue;
                else if(B[i][0] == B[k][0] && B[i][2] == B[j][0] && B[j][2] == B[k][2]){
                    B[k][0] = '-';
                    B[k][1] = '-';
                    B[k][2] = '-';
                }
            }
        }
    }

    /*This function deletes both symmetric and reflexive pairs.*/
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if (B[i][0] == B[j][2] && B[i][2] == B[j][0]){
                B[i][0] = '-';
                B[i][1] = '-';
                B[i][2] = '-';
            }
        }
    }

    /*The remaining ones creates the Hasse diagram. They are printed.*/
    for(i = 0;i<n;i++){
        if(B[i][0] == '-')  /*If the character is '-', it means that it is deleted, don't print it.*/
            continue;
        else{
            fprintf(fp2, "%c%c%c\n",B[i][0],B[i][1],B[i][2]);
        }
    }

}
