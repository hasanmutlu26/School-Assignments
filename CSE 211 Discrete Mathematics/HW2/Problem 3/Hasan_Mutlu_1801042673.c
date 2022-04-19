#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Hasan Mutlu - 1801042673*/
/*This program works for the default input file sent by teacher.*/

#define ASIZE 10    /*ASIZE and BSIZE are for maximum possible number elements in A and R sets.*/
#define BSIZE 10
#define SETNUMBER 8 /*SETNUMBER is for number of sets in input file. It is 8 in the one sent by teacher.*/



void readInput(FILE* fp1, FILE* fp2);
void fprintSet(FILE *fp2, char A[ASIZE], char B[BSIZE][3], int n);
void reflexiveCheck(FILE* fp2, char A[ASIZE], char B[BSIZE][3], int n, int ALen);
void symmetricCheck(FILE* fp2, char B[BSIZE][3], int n);
void transitiveCheck(FILE* fp2, char B[BSIZE][3], int n);



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
    memset(A, '\0', ASIZE);
    int n, i=0, j=0, ALen;
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
    }                       /*Functions are called*/
    fprintSet(fp2, A, B, n);
    reflexiveCheck(fp2,A,B,n,ALen);
    symmetricCheck(fp2,B,n);
    transitiveCheck(fp2,B,n);
}


/*This function prints the 'n' and the set of R*/
void fprintSet(FILE *fp2, char A[ASIZE], char B[BSIZE][3], int n){
    int i, j;
    fprintf(fp2, "n\n");
    for(i=0; i<n; i++){
        fprintf(fp2, "(");
        for(j=0;j<=2;j++){
            fprintf(fp2, "%c",B[i][j]);
        }
        fprintf(fp2, ")");
    }
    fprintf(fp2, "\n");
}


/*This function checks reflexivity.*/
void reflexiveCheck(FILE* fp2, char A[ASIZE], char B[BSIZE][3], int n, int ALen){
    int counter = 0, i, j, flag=0;
    char missing;
    /*Repeating relations are counted. (a,a), (b,b) ...*/
    for(i=0; i<n; i++){
        if(B[i][0] == B[i][2])
            counter++;
    }
    /*If the number of counted relations is equal to number of members in set A, it means that it is reflexive.*/
    if(counter == ALen)
        fprintf(fp2, "Reflexive: Yes, all elements are present.\n");
    /*If not, it is not reflexive.*/
    else{
        /*Here, we search for the missing set of relation. The flag indicates if that specific character is missing.*/
        for(j=0;j<strlen(A);j=j+2){
            /*Initial value of flag is 0*/
            flag=0;
            for(i=0;i<n;i++){
                /*If repeating relation is present, flag turns to 1*/
                if((B[i][0] == B[i][2]) && (B[i][2] == A[j])){
                    flag=1;
                }
            }
            /*If the flag went out as 0 from the previous loop, it means that the character in A[j] is the missing one.*/
            if(flag==0){
                missing = A[j];
                j+=strlen(A);   /*This is to end the whole loop.*/
            }
        }
        /*The missing set is fprinted.*/
        fprintf(fp2, "Reflexive: No, (%c,%c) is not present.\n", missing, missing);
    }
}

/*This function check symmetry situation.*/
void symmetricCheck(FILE* fp2, char B[BSIZE][3], int n){
    int i, j, temp, count=0, sym=1, flag=0;
    /*sym variable indicates the symmetry situation.
    It's initial value is 1 and if it turns to 0,
    it means that all elements are not symmetric.*/

    /*This function works similarly to the previous one.*/
    for(i=0; i<n; i++){
        for(j=0;j<n;j++){
            if (B[i][0] == B[j][2] && B[i][2] == B[j][0])
                /*If one symmetry is found, flag is turned to 1.*/
                flag = 1;
        }
        /*If the flag wents out as the 0. It means that one of the elements does not have a symmetric pair.*/
        if(flag==0){
            temp = i;   /*Temp holds the current value of i*/
            i=n;        /*This is to end the loop.*/
            sym=0;      /*sym variable turns to 0 to indicate that it is not symmetric.*/
        }
        flag=0;
    }
    /*If sym stays as 1, it means that all elements are symmetric.*/
    if(sym){
        fprintf(fp2, "Symmetric: Yes. All elements are symmetric.\n");
        fprintf(fp2, "Antiymmetric: No. All elements are symmetric.\n");
    }
    /*If not, the last value of i is reassigned to i and the element which hasn't got a symmetric pair is fprinted.*/
    else{
        i=temp;
        fprintf(fp2, "Symmetric: No. (%c%c%c) is found whereas (%c%c%c) is not found.\n", B[i][0], B[i][1], B[i][2], B[i][2], B[i][1], B[i][0]);
        fprintf(fp2, "Antiymmetric: Yes. (%c%c%c) is found whereas (%c%c%c) is not found.\n", B[i][0], B[i][1], B[i][2], B[i][2], B[i][1], B[i][0]);
    }
}

/*I couldn't check transivity, sorry.*/
void transitiveCheck(FILE* fp2, char B[BSIZE][3], int n){
    fprintf(fp2, "Transitive: Not implemented.\n");
}
