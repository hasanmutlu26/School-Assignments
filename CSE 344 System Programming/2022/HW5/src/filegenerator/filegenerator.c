#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

char fileName1[50];
char fileName2[50];
int N;
int M;


int main(int argc, char* argv[]){
    int r;
    strcpy(fileName1, argv[1]);
    strcpy(fileName2, argv[2]);
    N = atoi(argv[3]);
    M = atoi(argv[4]);
    FILE* fp1 = fopen(fileName1, "wb");
    FILE* fp2 = fopen(fileName2, "wb");
    srand(time(0));
    int row = pow(2, N+N);
    char c;
    for(int i = 0; i < row; i++){
        c = rand() % 126;
        fprintf(fp1, "%c", c);
        c = rand() % 126;
        fprintf(fp2, "%c", c);
    }
    fclose(fp1);
    fclose(fp2);
}