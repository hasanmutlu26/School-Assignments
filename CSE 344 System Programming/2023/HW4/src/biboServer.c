#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>
#include "queue.h"
#include "utils.h"

int logFile_fd;
int maxClients;

int clientCounter;

int counter, *workerlist, *clientNums;

sem_t *sv_sem;

pthread_t *worker_threads;
pthread_cond_t *workerConds;
pthread_mutex_t *workerMutex;
pthread_mutex_t critMutex;

struct Queue *queue;

char *dir_name;

// SIGNAL HANDLER
void signal_handler(int signal)
{
    char buf[MSG_SIZE], timebuffer[MAX_PATH], client_fifo_request[MAX_PATH];
    if (signal == SIGINT)
    {
        unlink(SERVER_FIFO_NAME);
        sem_unlink(SERVER_SEMAPHORE);

        GetTimestamp(timebuffer);
        sprintf(buf, "[%s] SERVER: SIGINT received. Terminating...\n", timebuffer);
        write(logFile_fd, buf, strlen(buf));

        int onQue;
        while (1)
        {
            onQue = deQueue(queue);
            if (onQue == 0)
            {
                break;
            }
            GetTimestamp(timebuffer);
            sprintf(buf, "[%s] SERVER: SIGINT sent to client PID%d on queue.\n", timebuffer, onQue);
            write(logFile_fd, buf, strlen(buf));

            kill(onQue, SIGINT);
        }

        for (int i = 0; i < maxClients; i++)
        {
            if (workerlist[i] > 0)
            {
                kill(workerlist[i], SIGINT);

                GetTimestamp(timebuffer);
                sprintf(buf, "[%s] SERVER: SIGINT sent to connected client PID%d.\n", timebuffer, workerlist[i]);
                write(logFile_fd, buf, strlen(buf));

                sprintf(client_fifo_request, "%s%d", CLIENT_REQUEST_FIFO, workerlist[i]);
                unlink(client_fifo_request);
            }
            pthread_cancel(worker_threads[i]);
            pthread_cond_destroy(&workerConds[i]);
            pthread_mutex_destroy(&workerMutex[i]);
        }

        pthread_mutex_destroy(&critMutex);

        free(workerlist);
        free(worker_threads);
        free(clientNums);
        free(workerConds);
        free(workerMutex);

        printf("Server stopped by SIGINT\n");

        GetTimestamp(timebuffer);
        sprintf(buf, "[%s] SERVER: Server stopped by SIGINT.\n", timebuffer);
        write(logFile_fd, buf, strlen(buf));

        exit(EXIT_SUCCESS);
    }

    else if (signal == SIGUSR1)
    {
        printf("killServer signal received. Terminating...\n");

        GetTimestamp(timebuffer);
        sprintf(buf, "[%s] SERVER: killServer signal received from a client. Terminating...\n", timebuffer);
        write(logFile_fd, buf, strlen(buf));

        kill(getpid(), SIGINT);
    }
}

void *client_handler(void *arg)
{
    char message[MSG_SIZE],
        client_fifo_response[MAX_PATH], client_fifo_request[MAX_PATH],
        client_semaphore_response[MAX_PATH], client_semaphore_request[MAX_PATH],
        buf[MSG_SIZE], timebuffer[MAX_PATH];

    int client_fd_response, client_fd_request;

    int workerid;
    int cl_pid;

    sem_t *cl_sem_res;

    workerid = atoi(arg);

    while (1)
    {
        while (workerlist[workerid] == 0)
        {
            pthread_cond_wait(&workerConds[workerid], &workerMutex[workerid]);
        }
        cl_pid = workerlist[workerid];
        printf("Client%d connected to worker%d as client%d\n", cl_pid, workerid, clientNums[workerid]);
        sprintf(client_fifo_response, "%s%d", CLIENT_RESPOND_FIFO, cl_pid);
        sprintf(client_fifo_request, "%s%d", CLIENT_REQUEST_FIFO, cl_pid);
        sprintf(client_semaphore_request, "%s%d", CLIENT_SEMAPORE_REQ, cl_pid);
        sprintf(client_semaphore_response, "%s%d", CLIENT_SEMAPORE_RES, cl_pid);
        cl_sem_res = sem_open(client_semaphore_response, 0);
        if (cl_sem_res == SEM_FAILED)
        {
            perror("Error opening client semaphore");
            exit(1);
        }
        sem_t *cl_sem_req;

        // CLIENT RESPONSE FIFO
        mkfifo(client_fifo_request, 0666);

        if ((client_fd_response = open(client_fifo_response, O_WRONLY)) == -1)
        {
            perror("Error opening client response FIFO");
            exit(1);
        }

        // CLIENT REQUEST SEMAPHORE
        cl_sem_req = sem_open(client_semaphore_request, O_CREAT, 0666, 0);
        if (cl_sem_req == SEM_FAILED)
        {
            perror("Error opening server semaphore");
            exit(1);
        }

        printf("Client PID %d connected as client%d\n", cl_pid, clientNums[workerid]);

        GetTimestamp(timebuffer);
        sprintf(buf, "[%s] WORKER%d: Serving Client%d with PID%d\n", timebuffer, workerid, clientNums[workerid], cl_pid);
        write(logFile_fd, buf, strlen(buf));

        // WRITE CONNECTED MESSAGE
        sprintf(message, "connected");
        if (write(client_fd_response, message, sizeof(message)) == -1)
        {
            perror("Error writing to client response FIFO connected message");
            exit(1);
        }

        sem_post(cl_sem_res);

        if ((client_fd_request = open(client_fifo_request, O_RDONLY)) == -1)
        {
            perror("Error opening client request FIFO");
            exit(1);
        }

        while (1)
        {
            // WAIT FOR NEXT COMMAND FROM CLIENT
            sem_wait(cl_sem_req);
            if (read(client_fd_request, message, sizeof(message)) > 0)
            {
                GetTimestamp(timebuffer);
                sprintf(buf, "[%s] WORKER%d: Client%d requested: %s", timebuffer, workerid, clientNums[workerid], message);
                write(logFile_fd, buf, strlen(buf));

                char *cmd = strtok(message, " \n");
                if (cmd == NULL)
                {
                    continue;
                }

                // PROCESS THE COMMAND AND RESPOND TO CLIENT
                if (!strcmp(cmd, "help"))
                {
                    char *arg = strtok(NULL, " \n");
                    display_help(arg, client_fd_response, arg == NULL);
                }

                else if (strcmp(cmd, "list") == 0)
                {
                    list_files(dir_name, client_fd_response);
                }

                else if (strcmp(cmd, "readF") == 0)
                {
                    char *file_name = strtok(NULL, " \n");
                    char *line_str = strtok(NULL, " \n");
                    if (file_name == NULL)
                    {
                        sprintf(message, "Please specify a file name.\n");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                        sprintf(message, "**done**");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                    }
                    else
                    {
                        int line_number = -1;
                        if (line_str != NULL)
                        {
                            line_number = atoi(line_str);
                        }
                        else
                        {
                            line_number = 0;
                        }
                        int retVal;
                        if ((retVal = read_file(dir_name, file_name, line_number, client_fd_response)) != 0)
                        {
                            sprintf(message, "%s\n", strerror(retVal));
                            if (write(client_fd_response, message, sizeof(message)) == -1)
                            {
                                perror("Error writing to client response FIFO");
                                exit(1);
                            }
                            sprintf(message, "**done**");
                            if (write(client_fd_response, message, sizeof(message)) == -1)
                            {
                                perror("Error writing to client response FIFO");
                                exit(1);
                            }
                        }
                    }
                }

                else if (strcmp(cmd, "writeT") == 0)
                {
                    char *file_name = strtok(NULL, " \n");
                    char *line_str = strtok(NULL, " \n");
                    char *text = strtok(NULL, "\n");
                    if (file_name == NULL)
                    {
                        sprintf(message, "Please specify a file name.\n");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                        sprintf(message, "**done**");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                    }

                    else if (line_str == NULL)
                    {
                        sprintf(message, "Please specify line number/some text to write.\n");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                        sprintf(message, "**done**");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                    }

                    else
                    {
                        int line_number = -1;
                        if (text == NULL)
                        {
                            text = line_str;
                            line_number = 0;
                        }

                        else if (line_str != NULL)
                        {
                            line_number = atoi(line_str);
                        }
                        int retVal;
                        if ((retVal = write_file(dir_name, file_name, line_number, text)) != 0)
                            sprintf(message, "%s", strerror(retVal));
                        else
                            sprintf(message, "Write to file is successful.\n");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                        sprintf(message, "**done**");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                    }
                }

                else if (strcmp(cmd, "upload") == 0)
                {
                    char *file_name = strtok(NULL, " \n");
                    if (file_name == NULL)
                    {
                        sprintf(message, "Please specify a file name.\n");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                        sprintf(message, "**done**");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                    }
                    else
                    {
                        char cwd[MAX_PATH];

                        getCwd(cwd, cl_pid);

                        int retVal;
                        if ((retVal = upload_file(dir_name, cwd, file_name)) != 0)
                            sprintf(message, "%s\n", strerror(retVal));
                        else
                            sprintf(message, "Uploading file is successful.\n");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                        sprintf(message, "**done**");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                    }
                }

                else if (strcmp(cmd, "download") == 0)
                {
                    char *file_name = strtok(NULL, " \n");
                    if (file_name == NULL)
                    {
                        printf(message, "Please specify a file name.\n");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                        sprintf(message, "**done**");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                    }
                    else
                    {
                        char cwd[MAX_PATH];

                        getCwd(cwd, cl_pid);

                        int retVal;
                        if ((retVal = download_file(dir_name, cwd, file_name)) != 0)
                            sprintf(message, "%s\n", strerror(retVal));
                        else
                            sprintf(message, "Downloading file is successful.\n");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                        sprintf(message, "**done**");
                        if (write(client_fd_response, message, sizeof(message)) == -1)
                        {
                            perror("Error writing to client response FIFO");
                            exit(1);
                        }
                    }
                }

                else if (!strcmp(cmd, "quit"))
                {
                    sprintf(message, "**done**");
                    if (write(client_fd_response, message, sizeof(message)) == -1)
                    {
                        perror("Error writing to client response FIFO");
                        exit(1);
                    }
                    sem_post(cl_sem_res);

                    GetTimestamp(timebuffer);
                    sprintf(buf, "[%s] WORKER%d: Client%d is quiting.\n", timebuffer, workerid, clientNums[workerid]);
                    write(logFile_fd, buf, strlen(buf));

                    break;
                }

                else
                {
                    sprintf(message, "Unknown command. Enter 'help' to see available commands.\n");
                    if (write(client_fd_response, message, sizeof(message)) == -1)
                    {
                        perror("Error writing to client response FIFO");
                        exit(1);
                    }
                    sprintf(message, "**done**");
                    if (write(client_fd_response, message, sizeof(message)) == -1)
                    {
                        perror("Error writing to client response FIFO");
                        exit(1);
                    }
                }

                GetTimestamp(timebuffer);
                sprintf(buf, "[%s] WORKER%d: Client%d request is handled.\n", timebuffer, workerid, clientNums[workerid]);
                write(logFile_fd, buf, strlen(buf));

                sem_post(cl_sem_res);
            }
        }

        close(client_fd_response);
        close(client_fd_request);

        if (unlink(client_fifo_request) == -1)
        {
            perror("Error removing server FIFO");
            exit(1);
        }

        if (sem_unlink(client_semaphore_request) == -1)
        {
            perror("Error unlinking client request semaphore.");
            exit(1);
        }

        printf("Client%d disconnected from worker%d.\n", clientNums[workerid], workerid);

        GetTimestamp(timebuffer);
        sprintf(buf, "[%s] WORKER%d: Client%d exited.\n", timebuffer, workerid, clientNums[workerid]);
        write(logFile_fd, buf, strlen(buf));

        pthread_mutex_lock(&critMutex);
        workerlist[workerid] = 0;
        counter--;
        pthread_mutex_unlock(&critMutex);
        sem_post(sv_sem);
    }

    return arg;
}

int main(int argc, char *argv[])
{
    char message[MSG_SIZE],
        client_fifo_response[MAX_PATH],
        client_semaphore_response[MAX_PATH],
        logfile_name[MAX_PATH], buf[MSG_SIZE], timebuffer[MAX_PATH];

    int server_fd, client_fd_response;

    int cl_pid;

    sem_t *cl_sem_res;

    queue = createQueue();
    if (argc != 3)
    {
        printf("Usage: %s <dirname> <max. #ofClients>\n", argv[0]);
        exit(1);
    }

    dir_name = argv[1];
    maxClients = atoi(argv[2]);

    if (maxClients <= 0)
    {
        printf("Max # of clients should be higher than 0\n");
        exit(1);
    }

    // OPENING THE DESIRED DIRECTORY
    struct stat st;
    if (stat(dir_name, &st) == -1)
    {
        if (mkdir(dir_name, 0777) == -1)
        {
            perror("Error");
            exit(1);
        }
    }

    sprintf(logfile_name, "%s%s", dir_name, LOGFILE_NAME);
    logFile_fd = open(logfile_name, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (logFile_fd == -1)
    {
        perror("Error: opening logfile.");
        exit(1);
    }

    GetTimestamp(timebuffer);
    sprintf(buf, "[%s] SERVER: Directory %s is opened.\n", timebuffer, dir_name);
    write(logFile_fd, buf, strlen(buf));

    // OPENING SERVER SEMAPHORE
    sv_sem = sem_open(SERVER_SEMAPHORE, O_CREAT, 0666, 0);
    if (sv_sem == SEM_FAILED)
    {
        perror("Error opening server semaphore");
        exit(1);
    }

    // SIGNAL HANDLING
    // signal(SIGINT, signal_handler);
    // signal(SIGUSR1, signal_handler);
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    sleep(1);

    // SERVER FIFO
    if (mkfifo(SERVER_FIFO_NAME, 0666) == -1)
    {
        perror("Error creating server FIFO");
        exit(1);
    }

    worker_threads = malloc(sizeof(pthread_t) * maxClients);
    workerConds = malloc(sizeof(pthread_cond_t) * maxClients);
    workerMutex = malloc(sizeof(pthread_mutex_t) * maxClients);
    workerlist = malloc(sizeof(int) * maxClients);
    clientNums = malloc(sizeof(int) * maxClients);
    char threadMessage[maxClients][16];

    if (pthread_mutex_init(&critMutex, NULL) == -1)
    {
        perror("Error: creating critical setion mutexes");
        exit(1);
    }

    for (int i = 0; i < maxClients; i++)
    {
        workerlist[i] = 0;

        if (pthread_cond_init(&workerConds[i], NULL) == -1)
        {
            perror("Error: Creating condition variables.");
            exit(1);
        }

        if (pthread_mutex_init(&workerMutex[i], NULL) == -1)
        {
            perror("Error: creating worker mutexes");
            exit(1);
        }

        sprintf(threadMessage[i], "%d", i);
        if (pthread_create(&worker_threads[i], NULL, client_handler, threadMessage[i]) == -1)
        {
            perror("Error: creating threads");
            exit(1);
        };
    }
    clientCounter = 0;
    counter = 0;

    printf("biboServer Here %d\n", maxClients);
    printf("Waiting for clients.\n");

    if ((server_fd = open(SERVER_FIFO_NAME, O_RDONLY | O_NONBLOCK)) == -1)
    {
        perror("Error opening server FIFO");
        exit(1);
    }

    GetTimestamp(timebuffer);
    sprintf(buf, "[%s] SERVER: Waiting for clients.\n", timebuffer);
    write(logFile_fd, buf, strlen(buf));

    while (1)
    {
        // ACCEPTING CLIENTS
        sem_wait(sv_sem);
        char con[15];
        if (read(server_fd, message, sizeof(message)) > 0)
        {
            /* extract client PID and message content */
            sscanf(message, "%d %s", &cl_pid, con);

            if (!strcmp(con, "tryConnect") && counter == maxClients)
            {
                sprintf(client_semaphore_response, "%s%d", CLIENT_SEMAPORE_RES, cl_pid);
                cl_sem_res = sem_open(client_semaphore_response, 0);
                if (cl_sem_res == SEM_FAILED)
                {
                    perror("Error opening client semaphore");
                    exit(1);
                }
                printf("Connection request PID %d Queue FULL\n", cl_pid);

                GetTimestamp(timebuffer);
                sprintf(buf, "[%s] SERVER: Client PID%d tried connect but queue is full.\n", timebuffer, cl_pid);
                write(logFile_fd, buf, strlen(buf));

                sprintf(client_fifo_response, "/tmp/client_res_%d", cl_pid);
                if ((client_fd_response = open(client_fifo_response, O_WRONLY)) == -1)
                {
                    perror("Error opening client response FIFO");
                    exit(1);
                }

                sprintf(message, "quit");
                if (write(client_fd_response, message, sizeof(message)) == -1)
                {
                    perror("Error writing to client response FIFO connected message");
                    exit(1);
                }

                sem_post(cl_sem_res);
                close(client_fd_response);
                sem_close(cl_sem_res);

                continue;
            }
            enQueue(queue, cl_pid);
            printf("Client PID %d is on queue.\n", cl_pid);

            GetTimestamp(timebuffer);
            sprintf(buf, "[%s] SERVER: Client PID%d is on queue.\n", timebuffer, cl_pid);
            write(logFile_fd, buf, strlen(buf));
        }

        // SERVING THE NEXT CLIENT FROM THE QUEUE
        if (counter < maxClients)
        {
            cl_pid = deQueue(queue);
            if (cl_pid == 0)
            {
                continue;
            }

            for (int i = 0; i < maxClients; i++)
            {
                if (workerlist[i] == 0)
                {
                    pthread_mutex_lock(&critMutex);
                    workerlist[i] = cl_pid;
                    counter++;
                    clientCounter++;
                    clientNums[i] = clientCounter;
                    pthread_mutex_unlock(&critMutex);
                    pthread_cond_signal(&workerConds[i]);
                    break;
                }
            }
        }

    } /* close the server FIFO */
    close(server_fd);
    sem_close(sv_sem);

    if (unlink(SERVER_FIFO_NAME) == -1)
    {
        perror("Error removing server FIFO");
        exit(1);
    }

    if (sem_unlink(SERVER_SEMAPHORE) == -1)
    {
        perror("Error unlinking server semaphore");
        exit(1);
    }

    free(worker_threads);
    free(workerlist);
    free(clientNums);
    free(workerConds);
    return 0;
}