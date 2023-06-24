#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>


mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
struct flock lock;
int fdout;
char writeBuf[50];
int x, y, z;
char xStr[10], yStr[10], zStr[10];
char* outputFilePath;
float mainMatrix[10][3];
float avg[3]; 
float deviationMatrix[10][3];
float covarianceMatrix[3][3];

/**
 * This function handles the SIGINT signal. It closes the file.
 * @param signum signal id
 */
void signal_handler_child(int signum){
    if(signum == SIGINT){
        close(fdout);
        exit(1);
    }
}

void calculate();

int main(int argc, char* argv[]){

    //Prepare signal handler
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler_child;
    sigaction(SIGINT, &sa, NULL);

    //Check command line arguments.
    if(argc < 2){
        printf("Error.\n");
        return 1;
    }

    //Open output file.
    outputFilePath = argv[0];
    fdout = open(argv[0], O_APPEND | O_WRONLY | O_CREAT, mode);
    if(fdout == -1){
        perror("open");
        return 1;
    }
    lseek(fdout, 0, SEEK_END);

    //Lock file
    lock.l_type = F_WRLCK;
    lock.l_len = 0;
    lock.l_start = 0;
    fcntl(fdout, F_SETLKW, &lock);

    //Write current process' number (i)
    if(write(fdout, argv[2], strlen(argv[2])) == -1){
        perror("write");
        return 1;
    }
    //Put space
    if(write(fdout, " ", strlen(argv[2])) == -1){
        perror("write");
        return 1;
    }

    //Assign main matrix
    int j = 0;
    for(int i = 0; i < 30; i+=3){
            mainMatrix[j][0] =  argv[1][i];
            mainMatrix[j][1] = argv[1][i+1];
            mainMatrix[j][2] = argv[1][i+2];
            j++;
    }

    //Calculate covariance matrix
    calculate();

    //Write all coordinates to output file
    for(int i = 0; i < 3; i++){
        memset(writeBuf, '\0', 25);
        memset(xStr, '\0', 3);
        memset(yStr, '\0', 3);
        memset(zStr, '\0', 3);
        snprintf(yStr, 10, "%.3f", covarianceMatrix[i][0]);
        snprintf(zStr, 10, "%.3f", covarianceMatrix[i][1]);
        snprintf(xStr, 10, "%.3f", covarianceMatrix[i][2]);
        strcat(writeBuf, xStr);
        strcat(writeBuf, " ");
        strcat(writeBuf, yStr);
        strcat(writeBuf, " ");
        strcat(writeBuf, zStr);
        strcat(writeBuf, " ");
        if(write(fdout, writeBuf, strlen(writeBuf)) == -1){
            perror("write");
            return 1;
        }
    }
    if(write(fdout, "\n", 1) == -1){
        perror("write");
        return 1;
    }

    //Unlock and close file.
    lock.l_type = F_UNLCK;
    fcntl(fdout, F_SETLKW, &lock);
    if(close(fdout) == -1){
        perror("close");
        return 1;
    }
    return 0;
}

/**
 * This function calculates the covariance matrix.
 */
void calculate(){
    int j = 0;

    for(int i = 0; i < 10; i++){
        avg[0] += mainMatrix[i][0];
        avg[1] += mainMatrix[i][1];
        avg[2] += mainMatrix[i][2];
    }

    avg[0] /= 10; avg[1] /= 10; avg[2] /= 10;

    for(int i = 0; i < 10; i++){
        deviationMatrix[i][0] = mainMatrix[i][0] - avg[0];
        deviationMatrix[i][0] = mainMatrix[i][0] - avg[0];
        deviationMatrix[i][0] = mainMatrix[i][0] - avg[0];
    }

    for(int i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            covarianceMatrix[i][j] =    deviationMatrix[i][0]*deviationMatrix[j][0] + deviationMatrix[i][1]*deviationMatrix[j][1] +
                                        deviationMatrix[i][2]*deviationMatrix[j][2] + deviationMatrix[i][3]*deviationMatrix[j][3] +
                                        deviationMatrix[i][4]*deviationMatrix[j][4] + deviationMatrix[i][5]*deviationMatrix[j][5] +
                                        deviationMatrix[i][6]*deviationMatrix[j][6] + deviationMatrix[i][7]*deviationMatrix[j][7] +
                                        deviationMatrix[i][8]*deviationMatrix[j][8] + deviationMatrix[i][9]*deviationMatrix[j][9];
        }
    }

}