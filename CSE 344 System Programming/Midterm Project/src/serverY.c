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

#define LOGBUF_SIZE 200

struct flock lock;
mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
int serverFd, clientFd, logFileFd, sharedFd;
char clientFifo[CLIENT_FIFO_NAME_LEN];
int duration;
int zPid;
struct request req;
struct response resp;
struct worker *wptr;
int* workerStatusTable;

int sharedSize;
int workerNum;
int zWorkerNum;
int zPipe[2];
char timestamp[50];
char logBuf[LOGBUF_SIZE];

void signalHandler(int);
int becomeDaemon(int);
void create_worker(int i);
void zcreate_worker(int i);
void worker_main(int pfd, int workerID);
void startServerZ();

void getCofactor(int* mat, int* temp, int p, int q, int n, int size);
int determinantOfMatrix(int* mat, int n, int size);

/*./serverY -s pathToServerFifo -o pathToLogFile –p poolSize -r poolSize2 -t 2*/
int main(int argc, char* argv[]){
    if(argc != 11 || strcmp(argv[1],"-s") != 0 || strcmp(argv[3], "-o") != 0 || strcmp(argv[5], "-p") != 0 || 
    strcmp(argv[7], "-r") != 0 || strcmp(argv[9], "-t") != 0){
        printf("Usage: [%s] s pathToServerFifo -o pathToLogFile -p poolSize -r poolSize2 -t 2\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    memset(logBuf, '\0',LOGBUF_SIZE);
    
    workerNum = atoi(argv[6]);
    zWorkerNum = atoi(argv[8]);
    duration = atoi(argv[10]);

    if(becomeDaemon(1) == -1){
        perror("daemon creation");
        exit(EXIT_FAILURE);
    }

    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_whence = SEEK_CUR;
    
    logFileFd = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, mode);
    if(logFileFd == -1){
        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Can't open log file\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("logfile");
        exit(EXIT_FAILURE);
    }
    snprintf(logBuf, LOGBUF_SIZE,"[%s] Server Y (%s, p=%d, t=%d) started\n", GetTimestamp(timestamp), argv[4], atoi(argv[6]), atoi(argv[10]));
    write(logFileFd, logBuf, strlen(logBuf));
    if(workerNum < 1){
        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Invalid poolsize.\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("Poolsize");
        exit(EXIT_FAILURE);
    }

    sharedSize = (workerNum+3)*sizeof(int);

    sharedFd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, mode);
    if(sharedFd == -1){
        snprintf(logBuf, LOGBUF_SIZE, "[%s] ERROR: Server PID#%d can't open shared memory\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("shm_open");
        exit(EXIT_FAILURE);
    }
    
    if(ftruncate(sharedFd, sharedSize) == -1){
        snprintf(logBuf, LOGBUF_SIZE, "[%s] ERROR: Server PID#%d can't ftruncate\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    workerStatusTable = mmap(NULL, sharedSize, PROT_READ | PROT_WRITE, MAP_SHARED, sharedFd, 0);
    if(workerStatusTable == MAP_FAILED){
        snprintf(logBuf, LOGBUF_SIZE, "[%s] ERROR: Server PID#%d can't mmap\n", GetTimestamp(timestamp), getpid());
        write(logFileFd, logBuf, strlen(logBuf));
        perror("mmap");
        exit(EXIT_FAILURE);
    }
    for(int i = 0; i < workerNum + 3; i++){
        workerStatusTable[i] = 0;
    }
    
    wptr = calloc(workerNum, sizeof(struct worker));
    for(int i = 0; i < workerNum; i++){
        create_worker(i);
    }
    
    //startServerZ();

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    if (mkfifo(argv[2], S_IRUSR | S_IWUSR | S_IWGRP) == -1 && errno != EEXIST){
        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Create server FIFO.\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
    
    serverFd = open(argv[2], O_RDONLY, mode);
    if(serverFd == -1){
        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Open server FIFO.\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    int i;
    for(;;){
        if (read(serverFd, &req, sizeof(struct request)) != sizeof(struct request)) {
            continue;                   /* Either partial read or error */
        }
        while(1){
            int flag = 0;
            for(i = 0; i < workerNum; i++){
                if(workerStatusTable[i] == 0){
                    flag = 1;
                    break;
                }
            }
            if(flag)
                break;
        }

        workerStatusTable[i] = 1;
        workerStatusTable[workerNum] += 1;

        if(write(wptr[i].pipefd, &req, sizeof(struct request)) != sizeof(struct request)){
            snprintf(logBuf, LOGBUF_SIZE, "[%s]  Can't send request from client PID#%d to worker PID#%d\n", GetTimestamp(timestamp), req.pid, wptr[i].pid);
            write(logFileFd, logBuf, strlen(logBuf));
        }
    }
    printf("End\n");
    exit(EXIT_SUCCESS);
}

void signalHandler(int sig){
    snprintf(logBuf, LOGBUF_SIZE,  "[%s] SIGINT received, terminating Z and exiting server Y.\n", GetTimestamp(timestamp));
    write(logFileFd, logBuf, strlen(logBuf));
    snprintf(logBuf, LOGBUF_SIZE,  "[%s] Total requests handled: %d, %d invertible, %d not. no requests are forwarded.\n", GetTimestamp(timestamp), (workerStatusTable[workerNum+1]+workerStatusTable[workerNum+2]), workerStatusTable[workerNum+1], workerStatusTable[workerNum+2]);
    write(logFileFd, logBuf, strlen(logBuf));
    for(int i = 0; i < workerNum; i++){
        kill(wptr[i].pid, SIGINT);
    }
    //kill(zPid, SIGINT);
    close(serverFd);
    close(logFileFd);
    free(wptr);
    exit(EXIT_SUCCESS);
}

void create_worker(int i){
    int pfd[2];
    pid_t pid;

    if(pipe(pfd) == -1){
        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Open worker pipe.\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) > 0) {
		close(pfd[0]);
		wptr[i].pid = pid;
		wptr[i].pipefd = pfd[1];
	}
    else if(pid == 0){
        dup2(pfd[0], STDERR_FILENO);
        close(pfd[0]);
        close(pfd[1]); 

        struct request msg;
        struct response resp;
        long int redd;
        for(;;){
            redd = read(STDERR_FILENO, &msg, sizeof(struct request));
            if(redd != sizeof(struct request)){
                snprintf(logBuf, LOGBUF_SIZE, "[%s] ERROR: Worker PID#%d with PFD#%d can't handle client PID#%d, read bytes: %ld from %ld\n", GetTimestamp(timestamp), getpid(), pfd[0], msg.pid, redd, sizeof(struct request));
                write(logFileFd, logBuf, strlen(logBuf));
                continue;
            }
            lock.l_type = F_WRLCK;
            fcntl(logFileFd, F_SETLKW, &lock);
            snprintf(logBuf, LOGBUF_SIZE, "[%s] Worker PID#%d is handling client PID#%d, matrix size %dx%d, pool busy %d/%d\n", GetTimestamp(timestamp), getpid(), msg.pid, msg.matrixSize, msg.matrixSize, workerStatusTable[workerNum], workerNum);
            write(logFileFd, logBuf, strlen(logBuf));
            lock.l_type = F_UNLCK;
            fcntl(logFileFd, F_SETLKW, &lock);

            snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,(long) msg.pid);
            
            if(msg.pid != 0){
                clientFd = open(clientFifo, O_WRONLY, mode);
                if(clientFd == -1){
                    snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Open client FIFO.\n", GetTimestamp(timestamp));
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
                lock.l_type = F_WRLCK;
                fcntl(logFileFd, F_SETLKW, &lock);
                if(det == 0){
                    workerStatusTable[workerNum+2]++;
                    snprintf(logBuf, LOGBUF_SIZE,"[%s] Worker PID#%d responding to client PID#%d: the matrix IS NOT invertible.\n", GetTimestamp(timestamp), getpid(), msg.pid);
                    write(logFileFd, logBuf, strlen(logBuf));
                }
                else{
                    snprintf(logBuf, LOGBUF_SIZE,"[%s] Worker PID#%d responding to client PID#%d: the matrix IS invertible.\n", GetTimestamp(timestamp), getpid(), msg.pid);
                    write(logFileFd, logBuf, strlen(logBuf));
                    workerStatusTable[workerNum+1]++;
                }
                lock.l_type = F_UNLCK;
                fcntl(logFileFd, F_SETLKW, &lock);
                resp.invertable = det;
                if (write(clientFd, &resp, sizeof(struct response)) != sizeof(struct response))
                    perror("writing to client fifo");
                close(clientFd);         
                sleep(duration);   
                workerStatusTable[i] = 0;
                workerStatusTable[workerNum]--;
            }
            
        }
    }
    else{
        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Create worker(fork).\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("fork");
        exit(EXIT_FAILURE);
    }
    
}

void startServerZ(){
    pid_t pid;
    char zworkernumbuf[50], zpipebuf[50], durationbuf[50], logFileFdbuf[50];
    char* cmdline[5];
    cmdline[4] = NULL;

    if(pipe(zPipe) == -1){
        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Open ServerZ pipe.\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("pipe");
        exit(EXIT_FAILURE);
    }
    
    if((pid = fork()) > 0){
        close(zPipe[0]);
        zPid = pid;
    }
    else if(pid == 0){
        close(zPipe[1]);
        snprintf(zworkernumbuf, 50, "%d", zWorkerNum);
        cmdline[0] = zworkernumbuf;
        snprintf(zpipebuf, 50, "%d", zPipe[0]);
        cmdline[1] = zpipebuf;
        snprintf(durationbuf, 50, "%d", duration);
        cmdline[2] = durationbuf;
        snprintf(logFileFdbuf, 50, "%d", logFileFd);
        cmdline[3] = logFileFdbuf;
        
        execvp("./serverZ", cmdline);

        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Execvp ServerZ.\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("execvp");
        exit(EXIT_FAILURE);
    }
    else{
        snprintf(logBuf, LOGBUF_SIZE,"[%s] ERRROR: Create ServerZ(fork).\n", GetTimestamp(timestamp));
        write(logFileFd, logBuf, strlen(logBuf));
        perror("fork");
        exit(EXIT_FAILURE);
    }

}
