#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/sem.h>
#include<sys/time.h>
#include<math.h>
#include"helper.h"


int fd;
int fdout;
char* filePath1;
char* filePath2;
char* outPath;
int N;
int M;

int size;
long* matrixA;
long* matrixB;
long* matrixC;
double* matrixWReel;
double* matrixWImag;

pthread_t* threads;
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond2 = PTHREAD_COND_INITIALIZER;

int mFinished = 0;
int multFinished = 0;

int inputControl(int argc, char* argv[]);
void signalHandler(int sig);
void readFile(char* filePath, long* matrix);
void writeFile();
void* ThreadFunction(void* p);
void matrixMultiplication(int id);
void dftmatrix(int id);
double time_elapsed(struct timeval* t1, struct timeval* t2);

int main(int argc, char* argv[]){
    char tmstmp[150];
    struct timeval t1, t2;
    if(inputControl(argc, argv)){
        fprintf(stderr, "[%s] Usage: ./hw5 -i filePath1 -j filePath2 -o output -n 4 -m 2\n", GetTimestamp(tmstmp));
        fprintf(stderr, "[%s] n > 2, m >= 2k, k>=1\n", GetTimestamp(tmstmp));
        exit(EXIT_FAILURE);
    }
    

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);


    size = pow(2, N);
    while(size % M != 0)
        M--;

    matrixA = malloc(size*size*sizeof(long));
    matrixB = malloc(size*size*sizeof(long));
    matrixC = malloc(size*size*sizeof(long));
    matrixWReel = malloc(size*size*sizeof(double));
    matrixWImag = malloc(size*size*sizeof(double));
    

    readFile(filePath1, matrixA);
    readFile(filePath2, matrixB);
    gettimeofday(&t1, NULL);
    fprintf(stderr, "[%s] Two matrices of size %dx%d have been read. The number of threads is %d\n", GetTimestamp(tmstmp), size, size, M);

    threads = malloc(M * sizeof(pthread_t));
    for(int i = 0; i < M; i++){
        if(pthread_create(&threads[i], NULL, ThreadFunction,(void *) (intptr_t) i) != 0){
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }


    pthread_mutex_lock(&mtx);
    while(mFinished < M)
        pthread_cond_wait(&cond1, &mtx);
    pthread_mutex_unlock(&mtx);

    pthread_mutex_lock(&mtx);
    multFinished++;
    pthread_cond_broadcast(&cond2);
    pthread_mutex_unlock(&mtx);
    
    for(int i = 0; i < M; i++){
        if(pthread_join(threads[i], NULL) != 0){
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    writeFile();

    gettimeofday(&t2, NULL);
    double elapsed = time_elapsed(&t1, &t2);

    fprintf(stderr, "[%s] The process has written the output file. The total time spent is %lf seconds.\n", GetTimestamp(tmstmp), elapsed);

    free(matrixA);
    free(matrixB);
    free(matrixC);
    free(matrixWReel);
    free(matrixWImag);
    free(threads);
}

double time_elapsed(struct timeval* t1, struct timeval* t2){
    long sec = t2->tv_sec - t1->tv_sec;
    long usec = (t2->tv_usec - t1->tv_usec);
    int coef = 10;
    if(usec < 0){
        while(usec / coef != 0)
        coef *= 10;
        usec = coef - (-usec);
        sec--;
    }
    return (double)sec + (double)usec / 1000000.0;
}

int inputControl(int argc, char* argv[]){
    int n = 0;
    int m = 0;
    int f1 = 0;
    int f2 = 0;
    int o = 0;

    if(argc != 11)
        return 1;

    for(int i = 1; i < 11; i++){
        if(strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "-N") == 0){
            N = atoi(argv[++i]);
            n++;
        }
        else if(strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "-M") == 0){
            M = atoi(argv[++i]);
            m++;
        }
        else if(strcmp(argv[i], "-i") == 0){
            filePath1 = argv[++i];
            f1++;
        }
        else if(strcmp(argv[i], "-j") == 0){
            filePath2 = argv[++i];
            f2++;
        }
        else if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "-O") == 0){
            outPath = argv[++i];
            o++;
        }
    }
    if(n == 1 && m == 1 && f1 == 1 && f2 == 1 && o == 1 && N > 0 && M > 0 && M % 2 == 0 && N > 2)
        return 0;

    return 1;
}

void signalHandler(int sig){
    for(int i = 0; i < M; i++){
        pthread_cancel(threads[i]);
        pthread_join(threads[i], NULL);
    }
    free(matrixA);
    free(matrixB);
    free(matrixC);
    free(matrixWReel);
    free(matrixWImag);
    free(threads);
    close(fd);
    close(fdout);
}

void readFile(char* filePath, long* matrix){
    char tmstmp[150];
    char c[3];
    fd = open(filePath, O_RDONLY, mode);
    if(fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    int row = pow(2, N+N);

    for(int i = 0; i < row; i++){
        memset(c, '\0', 3);
        if(read(fd, c, 1) != 1){
            fprintf(stderr, "[%s] Fatal Error: Insufficient content or read error.\n", GetTimestamp(tmstmp));
            exit(EXIT_FAILURE);
        }
        matrix[i] = (int) c[0];
    }
    close(fd);
}

void writeFile(){
    char wrbuf[50];
    fdout = open(outPath, O_WRONLY | O_CREAT | O_TRUNC, mode);
    if(fdout == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size - 1; j++){
            snprintf(wrbuf, 50, "%lf + j(%lf), ", matrixWReel[i*size + j], matrixWImag[i*size + j]);
            write(fdout, wrbuf, strlen(wrbuf));
        }
        snprintf(wrbuf, 50, "%lf + j(%lf)\n", matrixWReel[i*size + size - 1], matrixWImag[i*size + size - 1]);
        write(fdout, wrbuf, strlen(wrbuf));
    }
    close(fdout);
}

void* ThreadFunction(void* p){
    char tmstmp[150];
    int id = (intptr_t) p;
    struct timeval t1, t2;
    
    gettimeofday(&t1, NULL);
    matrixMultiplication(id);
    gettimeofday(&t2, NULL);

    double elapsed = time_elapsed(&t1, &t2);

    fprintf(stderr, "[%s] Thread %d has reached the rendezvous point in %lf seconds.\n", GetTimestamp(tmstmp), id+1, elapsed);
    pthread_mutex_lock(&mtx);
    mFinished++;
    pthread_cond_signal(&cond1);
    pthread_mutex_unlock(&mtx);

    pthread_mutex_lock(&mtx);
    while(multFinished == 0)
        pthread_cond_wait(&cond2, &mtx);
    pthread_mutex_unlock(&mtx);
    fprintf(stderr, "[%s] Thread %d is advancing to the second part\n", GetTimestamp(tmstmp), id+1);

    gettimeofday(&t1, NULL);
    dftmatrix(id);
    gettimeofday(&t2, NULL);

    elapsed = time_elapsed(&t1, &t2);
    fprintf(stderr, "[%s] Thread %d has finished the second part in %lf seconds.\n", GetTimestamp(tmstmp), id+1, elapsed);

    return NULL;
}

void matrixMultiplication(int id){
    int ind = (int)pow(2, N) / M;
    for(int i = id*ind; i < (id+1)*ind; i++){
        for(int j = 0; j < size; j++){
            matrixC[j*size + i] = 0;
            for(int k = 0; k < size; k++){
                matrixC[j*size + i] += matrixA[j*size + k] * matrixB[k*size + i];
            }
        }
    }
}

void dftmatrix(int id){
    int ind = (int)pow(2, N) / M;
    for(int i = id*ind; i < (id+1)*ind; i++){
        for(int j = 0; j < size; j++){
            matrixWReel[j*size + i] = 0.0;
            matrixWImag[j*size + i] = 0.0;
            double ak = 0.0;
            double bk = 0.0;
            for(int k = 0; k < size; k++){
                for(int l = 0; l < size; l++){
                    double x = -2.0*M_PI*(double)j*(double)k/(double)size;
                    double y = -2.0*M_PI*(double)i*(double)l/(double)size;
                    ak += (double)matrixC[k*size + l]*cos(x+y);
                    bk += (double)matrixC[k*size + l]*1.0*sin(x+y);
                }
            }
            matrixWReel[j*size + i] = ak;
            matrixWImag[j*size + i] = bk;
        }
    }
}









