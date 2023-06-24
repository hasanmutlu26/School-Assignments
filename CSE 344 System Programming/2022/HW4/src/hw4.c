#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<stdint.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/sem.h>
#include"helper.h"

int sem;
int C;
int N;
char *inputFilePath;
int fd;
pthread_t* consumers;
pthread_t supplier;
char tmstmp[50];

int inputControl(int argc, char* argv[]);

void* SupplierThread(void* p);
void checkDelivery(int* read1, int* read2, int* totdel1, int* totdel2);
void* ConsumerThread(void* p);
void signalHandler(int sig);


int main(int argc, char* argv[]){
    if(inputControl(argc, argv)){
        fprintf(stderr, "Usage:  ./hw4 -C 10 -N 5 -F inputfilePath\n");
        exit(EXIT_FAILURE);
    }

    // Prepare Signal Handler
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signalHandler;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);

    // Open input file.
    fd = open(inputFilePath, O_RDONLY, mode);
    if(fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }
    
    // Prepare semaphores
    sem = semget(IPC_PRIVATE, 2, IPC_CREAT | S_IRUSR | S_IWUSR);
    if(sem == -1){
        perror("semget");
        exit(EXIT_FAILURE);
    }
    
    //Set semaphores to 0
    union semun arg;
    struct sembuf sop[2];
    arg.array = malloc(2*sizeof(unsigned short));
    arg.array[0] = 0;
    arg.array[1] = 0;
    if (semctl(sem, 0, SETALL, arg, 2) == -1){
        perror("semctl");
        exit(EXIT_FAILURE);
    }
    free(arg.array);

    // Allocate space for consumers and create consumer threads
    consumers = malloc(sizeof(pthread_t)*C);
    for(int i = 0; i < C; i++){
        if(pthread_create(&consumers[i], NULL, ConsumerThread,(void *) (intptr_t) i) != 0){
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    //Create and detach supplier thread
    if(pthread_create(&supplier, NULL, SupplierThread, NULL) != 0){
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    pthread_detach(supplier);

    // Wait for consumers to finish
    for(int i = 0; i < C; i++){
        if(pthread_join(consumers[i], NULL) != 0){
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    sop[0].sem_num = 0;
    sop[1].sem_num = 1;
    sop[0].sem_op = sop[1].sem_op = -1;
    sop[0].sem_flg = sop[1].sem_flg = 0;
    if (semop(sem, sop, 2) == -1){
        perror("semop");
        exit(EXIT_FAILURE);
    }

    //Free resources, close file, remove semaphores and return
    free(consumers);
    close(fd);
    semctl(sem, 0, IPC_RMID);
    pthread_exit(0);
}

//Input validity control
int inputControl(int argc, char* argv[]){
    int c = 0;
    int n = 0;
    int f = 0;

    if(argc != 7)
        return 1;

    for(int i = 1; i < 7; i++){
        if(strcmp(argv[i], "-C") == 0 || strcmp(argv[i], "-c") == 0){
            C = atoi(argv[++i]);
            c++;
        }
        else if(strcmp(argv[i], "-N") == 0 || strcmp(argv[i], "-n") == 0){
            N = atoi(argv[++i]);
            n++;
        }
        else if(strcmp(argv[i], "-F") == 0 || strcmp(argv[i], "-f") == 0){
            inputFilePath = argv[++i];
            f++;
        }
    }
    if(c == 1 && f == 1 && n == 1 && C > 0 && N > 0)
        return 0;

    return 1;
}

void signalHandler(int sig){
    for(int i = 0; i < N*C; i++){
        pthread_cancel(consumers[i]);
        pthread_join(consumers[i], NULL);
    }
    pthread_cancel(supplier);
    semctl(sem, 0, IPC_RMID);
    free(consumers);
    close(fd);
    exit(EXIT_FAILURE);
}

void* SupplierThread(void* p){
    char buf[3];
    char tmstmp[50];
    struct sembuf sop;
    int read1 = 0;
    int read2 = 0;
    int totdel1 = 0;
    int totdel2 = 0;

    for(int i = 0; i < 2*N*C; i++){
        checkDelivery(&read1, &read2, &totdel1, &totdel2);  //Check if any character is delivered
        
        //Read one character from file
        memset(buf, '\0', 3);
        if(read(fd, buf, 1) < 1){
            perror("read");
            exit(EXIT_FAILURE);
        }

        //Post the semaphore and print read message
        if(strcmp(buf, "1") == 0){
            sop.sem_num = 0;
            sop.sem_op = 1;
            sop.sem_flg = 0;
            fprintf(stderr, "[%s] Supplier: read from input a '1'. Current amounts: %d x '1', %d x '2'.\n", GetTimestamp(tmstmp), totdel1, totdel2);
            if(semop(sem, &sop, 1) == -1){
                perror("semop");
                exit(EXIT_FAILURE);
            }
            read1++;    //increment read1 counter
        }
        else if(strcmp(buf, "2") == 0){
            sop.sem_num = 1;
            sop.sem_op = 1;
            sop.sem_flg = 0;
            fprintf(stderr, "[%s] Supplier: read from input a '2'. Current amounts: %d x '1', %d x '2'.\n", GetTimestamp(tmstmp), totdel1, totdel2);
            if (semop(sem, &sop, 1)== -1){
                perror("semop");
                exit(EXIT_FAILURE);
            }
            read2++;    //increment read2 counter
        }
    }
    //sleep(2);
    
    // Check if any character is delivered one last time
    checkDelivery(&read1, &read2, &totdel1, &totdel2);
    struct sembuf sop2[2];
    fprintf(stderr, "[%s] The Supplier has left.\n", GetTimestamp(tmstmp));
    sop2[0].sem_num = 0;
    sop2[1].sem_num = 1;
    sop2[0].sem_op = 1;  sop2[1].sem_op = 1;
    sop2[0].sem_flg = 0; sop2[1].sem_flg = 0;
    if (semop(sem, sop2, 2) == -1){
        perror("semop");
        exit(EXIT_FAILURE);
    }
    return NULL;
}

// Check if characters are delivered and print delivered message
void checkDelivery(int* read1, int* read2, int* totdel1, int* totdel2){
    int val1, val2;

    //If read counter is greater than semaphore's current value, decrement it until they are equal and print delivered message.
    val1 = semctl(sem, 0, GETVAL);
    if(val1 < *read1){
        for(;val1 < *read1; (*read1)--){
            fprintf(stderr, "[%s] Supplier: delivered a '1'. Post-delivery amounts: %d x '1', %d x '2'.\n", GetTimestamp(tmstmp), ++(*totdel1), *totdel2);
        }
    }
    val2 = semctl(sem, 1, GETVAL);
    if(val2 < *read2){
        for(;val2 < *read2; (*read2)--){
            fprintf(stderr, "[%s] Supplier: delivered a '2'. Post-delivery amounts: %d x '1', %d x '2'.\n", GetTimestamp(tmstmp), *totdel1, ++(*totdel2));
        }
    }
}

void* ConsumerThread(void* p){
    int id = (intptr_t) p;
    char tmstmp[50];
    struct sembuf sop[2];

    for(int i = 0; i < N; i++){
        //Wait for characters.
        fprintf(stderr, "[%s] Consumer-%d at iteration %d (waiting). Current amounts: %d x '1', %d x '2'.\n",
        GetTimestamp(tmstmp), id, i, semctl(sem, 0, GETVAL), semctl(sem, 1, GETVAL));
        sop[0].sem_num = 0;
        sop[1].sem_num = 1;
        sop[0].sem_op = sop[1].sem_op = -1;
        sop[0].sem_flg = sop[1].sem_flg = 0;
        if (semop(sem, sop, 2) == -1){
            perror("semop");
            exit(EXIT_FAILURE);
        }
        //Print consumed message
        fprintf(stderr, "[%s] Consumer-%d at iteration %d (consumed). Post-consumption amounts: %d x '1', %d x '2'.\n",
        GetTimestamp(tmstmp), id, i, semctl(sem, 0, GETVAL), semctl(sem, 1, GETVAL));
    }
    fprintf(stderr, "[%s] Consumer-%d has left\n", GetTimestamp(tmstmp), id);
    return NULL;
}















