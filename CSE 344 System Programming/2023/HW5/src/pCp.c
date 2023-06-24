#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <semaphore.h>
#include <signal.h>
#include "FileEntryQueue.h"

struct Queue *queue;

int buffer_size = 0;
int num_consumers = 0;

pthread_t producer;
pthread_t* consumers;

pthread_mutex_t crit_mutex;
pthread_mutex_t prnt_mutex;
sem_t buffer_sem;
sem_t cons_sem;
int done = 0;
int num_files_copied = 0;
int num_directories_copied = 0;
unsigned long long total_bytes_copied = 0;

void signal_handler(int signal)
{
    pthread_cancel(producer);
    pthread_join(producer, NULL);

    for (int i = 0; i < num_consumers; i++)
    {
        pthread_cancel(consumers[i]);
        pthread_join(consumers[i], NULL);
    }
    

    printf("SIGINT received. Terminating...\n");
    printf("Files copied: %d\n", num_files_copied);
    printf("Directories copied: %d\n", num_directories_copied);
    printf("Total bytes copied: %llu\n", total_bytes_copied);


    destroy_queue(queue);
    free(queue);

    sem_post(&buffer_sem);
    sem_post(&cons_sem);

    sem_close(&buffer_sem);
    sem_close(&cons_sem);
    sem_destroy(&buffer_sem);
    sem_destroy(&cons_sem);
    
    free(consumers);

    exit(1);
}

int copy_file(FileEntry file)
{
    struct stat file_stat;
    fstat(file.src_fd, &file_stat);

    ssize_t bytes_read, bytes_written;
    char buffer[4096];

    while ((bytes_read = read(file.src_fd, buffer, sizeof(buffer))) > 0)
    {
        bytes_written = write(file.dest_fd, buffer, bytes_read);
        if (bytes_written == -1)
        {
            perror("Writing to destination file");
            break;
        }
        total_bytes_copied += bytes_written;
    }

    if (bytes_read == -1)
    {
        perror("Reading from the source file");
        return 1;
    }

    close(file.src_fd);
    close(file.dest_fd);
    num_files_copied++;
    return 0;
}

void *producer_thread(void *arg)
{

    char src_dir[MAX_FILE_NAME_LENGTH];
    char dest_dir[MAX_FILE_NAME_LENGTH];

    strcpy(src_dir, ((char **)arg)[0]);
    strcpy(dest_dir, ((char **)arg)[1]);


    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    FileEntry file;

    dir = opendir(src_dir);
    if (dir == NULL)
    {
        perror("Opening source directory");
        done = 1;
        return NULL;
    }

    mkdir(dest_dir, 0666);

    while ((entry = readdir(dir)) != NULL)
    {

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(file.src_file, strlen(src_dir) + strlen(entry->d_name) + 2, "%s/%s", src_dir, entry->d_name);
        snprintf(file.dest_file, strlen(dest_dir) + strlen(entry->d_name) + 2, "%s/%s", dest_dir, entry->d_name);

        if (lstat(file.src_file, &file_stat) == -1)
        {
            perror("Getting file information");
            continue;
        }

        if (S_ISREG(file_stat.st_mode) || S_ISFIFO(file_stat.st_mode))
        {
            sem_wait(&buffer_sem);
            file.src_fd = open(file.src_file, O_RDONLY);
            if (file.src_fd == -1)
            {
                perror("Opening source file");
                continue;
            }

            file.dest_fd = open(file.dest_file, O_WRONLY | O_CREAT | O_TRUNC, file_stat.st_mode);
            if (file.dest_fd == -1)
            {
                perror("Opening/creating destination file");
                close(file.src_fd);
                sem_post(&buffer_sem);
                continue;
            }

            
            pthread_mutex_lock(&crit_mutex);
            if (enqueue(queue, file) == 1)
            {
                printf("Error: Queue capacity is full.\n");     //This will never be printed.
            }
            printf("Producer put file %s in buffer.\n", file.src_file);
            pthread_mutex_unlock(&crit_mutex);

            sem_post(&cons_sem);
        }
        else if (S_ISDIR(file_stat.st_mode))
        {
            char *sub_dirs[2];
            sub_dirs[0] = strdup(file.src_file);
            sub_dirs[1] = strdup(file.dest_file);
            producer_thread((void **)sub_dirs);
            free(sub_dirs[0]);
            free(sub_dirs[1]);
            num_directories_copied++;
        }
    }

    closedir(dir);


    return NULL;
}

void *consumer_thread(void *arg)
{
    int cons_id = atoi(arg);

    while (1)
    {
        pthread_mutex_lock(&prnt_mutex);
        printf("Consumer%d is waiting for an item.\n", cons_id);
        pthread_mutex_unlock(&prnt_mutex);

        sem_wait(&cons_sem);

        if (isEmpty(queue) && done)
        {
            break;
        }

        pthread_mutex_lock(&crit_mutex);
        FileEntry file = dequeue(queue);
        pthread_mutex_unlock(&crit_mutex);

        if (file.src_fd == -1)
        {
            continue;
        }

        
        sem_post(&buffer_sem);

        pthread_mutex_lock(&prnt_mutex);
        printf("Consumer%d is copying %s\n", cons_id, file.src_file);
        pthread_mutex_unlock(&prnt_mutex);
        copy_file(file);
        pthread_mutex_lock(&prnt_mutex);
        printf("Consumer%d copied '%s'\n", cons_id, file.src_file);
        pthread_mutex_unlock(&prnt_mutex);
    }

    pthread_mutex_lock(&prnt_mutex);
    printf("Consumer%d ended.\n", cons_id);
    pthread_mutex_unlock(&prnt_mutex);

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        printf("Usage: %s <buffer_size> <num_consumers> <src_dir> <dest_dir>\n", argv[0]);
        return 1;
    }

    buffer_size = atoi(argv[1]);
    num_consumers = atoi(argv[2]);
    char *src_dir = argv[3];
    char *dest_dir = argv[4];

    if (buffer_size <= 0 || num_consumers <= 0)
    {
        printf("Buffer size and number of consumers must be positive integers.\n");
        return 1;
    }

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    queue = createQueue(buffer_size);

    if (sem_init(&buffer_sem, 0, buffer_size) < 0)
    {
        perror("Initializing buffer semaphore.");
        exit(1);
    }

    if (sem_init(&cons_sem, 0, 0) < 0)
    {
        perror("Initializing buffer semaphore.");
        exit(1);
    }

    if (pthread_mutex_init(&crit_mutex, NULL) == -1)
    {
        perror("Creating critical section mutex");
        exit(1);
    }

    if (pthread_mutex_init(&prnt_mutex, NULL) == -1)
    {
        perror("Error: creating critical setion mutexes");
        exit(1);
    }

    char *dirs[2] = {src_dir, dest_dir};
    if (pthread_create(&producer, NULL, producer_thread, (void *)dirs) < 0)
    {
        perror("Creating producer thread");
        exit(1);
    }

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    consumers = malloc(sizeof(pthread_t)*num_consumers);

    char cons_num[num_consumers][10];
    for (int i = 0; i < num_consumers; i++)
    {
        sprintf(cons_num[i], "%d", i);
        if (pthread_create(&consumers[i], NULL, consumer_thread, cons_num[i]) < 0)
        {
            perror("Creating consumer thread");
            exit(1);
        }
    }

    pthread_join(producer, NULL);

    done = 1;
    printf("Main producer ended.\n");

    for (int i = 0; i < num_consumers; i++)
    {
        sem_post(&cons_sem);
    }

    for (int i = 0; i < num_consumers; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    gettimeofday(&end_time, NULL);
    double execution_time = (end_time.tv_sec - start_time.tv_sec) +
                            (end_time.tv_usec - start_time.tv_usec) / 1000000.0;

    printf("Total execution time: %.2f seconds\n", execution_time);
    printf("Files copied: %d\n", num_files_copied);
    printf("Directories copied: %d\n", num_directories_copied);
    printf("Total bytes copied: %llu\n", total_bytes_copied);

    destroy_queue(queue);

    sem_close(&buffer_sem);
    sem_close(&cons_sem);
    sem_destroy(&buffer_sem);
    sem_destroy(&cons_sem);
    
    free(consumers);

    return 0;
}
