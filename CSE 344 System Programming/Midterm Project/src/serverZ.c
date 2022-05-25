#include "helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <math.h>

#define QUEUESIZE 50

mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
char timestamp[QUEUESIZE];
char logBuf[1024];
int workerNum;
int* workerPids;
int pipeFd;
int duration;
int logFileFd;
int sharedReqSize;
struct request* sharedReq;
int sharedReqFd;
int *reqQueue;
int reqQueueFd;
int reqQueueSize;
char clientFifo[CLIENT_FIFO_NAME_LEN];
int clientFd;

void create_worker();
void signalHandler(int sig);

int main(int argc, char* argv[]){
    struct request msg;

    /*if(argc != 5){
        perror("argc");
        exit(EXIT_FAILURE);
    }*/

    workerNum = atoi(argv[1]);
    if(workerNum < 0){
        perror("WorkerNum");
        exit(EXIT_FAILURE);
    }

    pipeFd = atoi(argv[2]);

    duration = atoi(argv[3]);
    logFileFd = atoi(argv[4]);

    becomeDaemon(1);

    sharedReqSize = (QUEUESIZE)*sizeof(struct request);
    sharedReqFd = shm_open(SHARED_REQ_NAME, O_CREAT | O_RDWR, mode);
    if(sharedReqFd == -1){
        snprintf(logBuf, 1024, "[%s] Z:ERROR: ServerZ PID#%d can't open shared memory\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    snprintf(logBuf, 1024, "[%s] Z:ServerZ PID#%d opened shared memory#%d\n", GetTimestamp(timestamp), getpid(), sharedReqFd);
    write(logFileFd, logBuf, strlen(logBuf));
    if(ftruncate(sharedReqFd, sharedReqSize) == -1){
        snprintf(logBuf, 1024, "[%s] Z:ERROR: ServerZ PID#%d can't ftruncate\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    sharedReq = mmap(NULL, sharedReqSize, PROT_READ | PROT_WRITE, MAP_SHARED, sharedReqFd, 0);
    if(sharedReq == MAP_FAILED){
        snprintf(logBuf, 1024, "[%s] Z:ERROR: Server PID#%d can't mmap\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    reqQueueSize = (QUEUESIZE+3)*sizeof(int);

    reqQueueFd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, mode);
    if(reqQueueFd == -1){
        snprintf(logBuf, 1024, "[%s] Z:ERROR: Server PID#%d can't open shared memory\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    snprintf(logBuf, 1024, "[%s] Z:Server PID#%d opened shared memory#%d\n", GetTimestamp(timestamp), getpid(), reqQueueFd);
    write(logFileFd, logBuf, strlen(logBuf));
    if(ftruncate(reqQueueFd, reqQueueSize) == -1){
        snprintf(logBuf, 1024, "[%s] Z:ERROR: Server PID#%d can't ftruncate\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    reqQueue = mmap(NULL, reqQueueSize, PROT_READ | PROT_WRITE, MAP_SHARED, reqQueueFd, 0);
    if(reqQueue == MAP_FAILED){
        snprintf(logBuf, 1024, "[%s] Z:ERROR: Server PID#%d can't mmap\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < QUEUESIZE; i++){
        reqQueue[i] = 0;
    }

    workerPids = calloc(workerNum, sizeof(int));
    for(int i = 0; i < workerNum; i++){
        create_worker(i);
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    long int redd;
    for(;;){
        redd = read(pipeFd, &msg, sizeof(struct request));
        if(redd != sizeof(struct request)){
            snprintf(logBuf, 1024, "[%s] Z: Z:ServerZ PID#%d with PFD#%d can't handle client PID#%d, read bytes: %ld from %ld\n", GetTimestamp(timestamp), getpid(), pipeFd, msg.pid, redd, sizeof(struct request));
            write(logFileFd, logBuf, strlen(logBuf));
            continue;
        }

        while(1){
            int flag = 0;
            for(int i = 0; i < QUEUESIZE; i++){
                if(reqQueue[i] == 0){
                    sharedReq[i] = msg;
                    reqQueue[i] = 1;
                    flag = 1;
                }
            }
            if(flag)
                break;
        }
    }


}

void create_worker(int i){
    struct request msg;
    struct response resp;
    pid_t pid;

    if((pid = fork()) > 0){
        workerPids[i] = pid;
    }
    else if(pid == 0){
        for(;;){
            for(int i = 0; i < QUEUESIZE; i++){
                if(reqQueue[i] == 1){
                    msg = sharedReq[i];
                    reqQueue[i] = 0;
                    snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long) msg.pid);
                    snprintf(logBuf, 1024, "[%s] Z:Worker PID#%d is handling client PID#%d, matrix size %dx%d, pool busy %d/%d\n", GetTimestamp(timestamp), getpid(), msg.pid, msg.matrixSize, msg.matrixSize, reqQueue[workerNum], workerNum);
                    write(logFileFd, logBuf, strlen(logBuf));

                    clientFd = open(clientFifo, O_WRONLY, mode);
                    if(clientFd == -1){
                        snprintf(logBuf, 1024,"[%s] Z:ERRROR: Open client FIFO.\n", GetTimestamp(timestamp));
                        write(logFileFd, logBuf, strlen(logBuf));
                        perror("open");
                        exit(EXIT_FAILURE);
                    }
                    int* mat = malloc(sizeof(int)*msg.matrixSize*msg.matrixSize);
                    for(int i = 0; i < msg.matrixSize; i++){
                        for(int j = 0; j < msg.matrixSize; j++)
                            *(mat+i*msg.matrixSize+j) = msg.matrix[i][j];
                    }
                    int det = determinantOfMatrix(mat, msg.matrixSize, msg.matrixSize);
                    free(mat);
                    if(det == 0)
                        reqQueue[workerNum+2]++;
                    else
                        reqQueue[workerNum+1]++;
                    resp.invertable = det;
                    if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
                        perror("writing to client fifo");
                    close(clientFd);

                }
            }
            sleep(duration);
        }
    }
    else{
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

void signalHandler(int sig){
    snprintf(logBuf, 1024,  "[%s] Z:SIGINT received, exiting server Z.\n", GetTimestamp(timestamp));
    write(logFileFd, logBuf, strlen(logBuf));
    snprintf(logBuf, 1024,  "[%s] Z:Total requests handled: %d, %d invertible, %d not. no requests are forwarded.\n", GetTimestamp(timestamp), (reqQueue[workerNum+1]+reqQueue[workerNum+2]), reqQueue[workerNum+1], reqQueue[workerNum+2]);
    write(logFileFd, logBuf, strlen(logBuf));
    for(int i = 0; i < workerNum; i++){
        kill(workerPids[i], SIGINT);
    }
    close(logFileFd);
    free(workerPids);
    exit(EXIT_SUCCESS);
}