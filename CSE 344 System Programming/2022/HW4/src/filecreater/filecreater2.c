#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

char fileName[50];
int C;
int N;
int numOf1 = 0;
int numOf2 = 0;


int main(int argc, char* argv[]){
    strcpy(fileName, argv[1]);
    C = atoi(argv[2]);
    N = atoi(argv[3]);
    FILE* fp = fopen(fileName, "w");

    for(int i = 0; i < N*C; i++){
        fprintf(fp, "1");
    }
    for(int i = 0; i < N*C; i++){
        fprintf(fp, "2");
    }
    fclose(fp);
}