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
    int r;
    strcpy(fileName, argv[1]);
    C = atoi(argv[2]);
    N = atoi(argv[3]);
    FILE* fp = fopen(fileName, "w");
    srand(time(0));
    while(1){
        r = (rand() % 2) + 1;
        //printf("%d %d %d\n", r, numOf1, numOf2);
        if(r == 1){
            if(numOf1 < N*C){
                fprintf(fp, "1");
                numOf1++;
            }
            else{
                fprintf(fp, "2");
                numOf2++;
            }
        }
        else if(r == 2){
            if(numOf2 < N*C){
                fprintf(fp, "2");
                numOf2++;
            }
            else{
                fprintf(fp, "1");
                numOf1++;
            }
        }
        if(numOf1 >= N*C && numOf2 >= N*C){
            break;
        }
    }
    fclose(fp);
}