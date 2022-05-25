#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include"helper.h"

struct request req;
struct response resp;
int serverFd, clientFd, dataFd;

mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
static char clientFifo[CLIENT_FIFO_NAME_LEN];


void readMatrix(char* inputFilePath);
void signalHandler(int sig);

/*./client -s pathToServerFifo -o pathToDataFile*/
int main(int argc, char* argv[]){
    clock_t begin;
    double time_spent;
    
    char outBuf[1024];
    char timestamp[50];
    
    if(argc != 5 || strcmp(argv[1], "-s") != 0 || strcmp(argv[3], "-o") != 0){
        printf("Usage: %s -s pathToServerFifo -o pathToDataFile\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    //readInput(argv[4]);

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    req.pid = getpid();
    readMatrix(argv[4]);
    umask(0);
    
    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long) getpid());
    if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1  && errno != EEXIST){
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    
    serverFd = open(argv[2], O_WRONLY,mode);
    
    if(serverFd < 0){
        perror("open server fifo");
        exit(EXIT_FAILURE);
    }

    snprintf(outBuf, 1024, "[%s] Client PID#%d (%s) is submitting a %dx%d matrix\n",GetTimestamp(timestamp), req.pid, argv[4], req.matrixSize, req.matrixSize);
    write(1, outBuf, strlen(outBuf));
    begin = clock();
    if(write(serverFd, &req, sizeof(struct request)) != sizeof(struct request)){
        perror("Can't write to server.");
        exit(EXIT_FAILURE);
    }
    clientFd = open(clientFifo, O_RDONLY, mode);
    if(clientFd == -1){
        perror("open client fifo");
        exit(EXIT_FAILURE);
    }
    if (read(clientFd, &resp, sizeof(struct response)) != sizeof(struct response)){
        perror("Can't read client fifo");
        exit(EXIT_FAILURE);
    }
    time_spent = (double)(clock() - begin);
    if(resp.invertable == 0){
        printf("[%s] the matrix is not invertable, total time: %lf seconds, goodbye\n", GetTimestamp(timestamp), time_spent / CLOCKS_PER_SEC);
    }
    else{
        printf("[%s] the matrix is invertable, total time: %lf seconds, goodbye.\n", GetTimestamp(timestamp), time_spent / CLOCKS_PER_SEC);
    }
    close(clientFd);
    close(serverFd);
    exit(EXIT_SUCCESS);
}

void readMatrix(char* inputFilePath){
    int count = 0;
    char numbuf[20];
    char buf[5];
    memset(numbuf, '\0', 20);
    memset(buf, '\0', 5);
    dataFd = open(inputFilePath, O_RDONLY, mode);
    if(dataFd < 0){
        perror("open");
        exit(EXIT_FAILURE);
    }
    while(1){
        memset(buf, '\0', 5);
        if(read(dataFd, buf, 1) < 1){
            perror("read");
            exit(EXIT_FAILURE);
        }
        if(strcmp(buf, "\n") == 0){
            req.matrix[0][count] = atoi(numbuf);
            memset(numbuf, '\0', 20);
            count++;
            break;
        }
        else if(strcmp(buf, ",") == 0 || strcmp(buf, ";") == 0){
            req.matrix[0][count] = atoi(numbuf);
            count++;
            memset(numbuf, '\0', 20);
            continue;
        }
        else if(buf[0] >= '0' && buf[0] <= '9') {
            strcat(numbuf, buf);
        }
    }
    req.matrixSize = count;
    memset(numbuf, '\0', 20);
    for(int i = 1; i < req.matrixSize; i++){
        int j = 0;
        memset(numbuf, '\0', 20);
        while(1){
            memset(buf, '\0', 5);
            if(read(dataFd, buf, 1) < 1){
                if(j == req.matrixSize - 1){
                    req.matrix[i][j] = atoi(numbuf);
                    break;
                }
                perror("read");
                exit(EXIT_FAILURE);
            }
            if(strcmp(buf, "\n") == 0){
                req.matrix[i][j] = atoi(numbuf);
                j++;
                break;
            }
            else if(strcmp(buf, ",") == 0 || strcmp(buf, ";") == 0){
                req.matrix[i][j] = atoi(numbuf);
                j++;
                memset(numbuf, '\0', 20);
                continue;
            }
            else{
                strcat(numbuf, buf);
            }
        }
    }
    
    close(dataFd);
}


void signalHandler(int sig){
    close(clientFd);
    close(serverFd);
    exit(EXIT_SUCCESS);
}