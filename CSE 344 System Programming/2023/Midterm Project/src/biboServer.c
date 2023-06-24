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
#include "queue.h"
#include "utils.h"

int logFile_fd;
int server_fd, client_fd_response, client_fd_request;
int count_fd, workers_fd;
int cl_pid, maxClients;
int isChild;

int clientCounter = 0;

int *counter, *workerlist;

char message[MSG_SIZE],
    client_fifo_response[MAX_PATH], client_fifo_request[MAX_PATH],
    client_semaphore_response[MAX_PATH], client_semaphore_request[MAX_PATH],
    logfile_name[MAX_PATH], buf[MSG_SIZE], timebuffer[MAX_PATH];

sem_t *sv_sem, *cl_sem_res;

struct Queue *queue;

// SIGNAL HANDLER
void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        if (isChild == 1)
        {
            GetTimestamp(timebuffer);
            sprintf(buf, "[%s] WORKER%d: SIGINT received. SIGINT sent to client%d\n", timebuffer, getpid(), clientCounter);
            write(logFile_fd, buf, strlen(buf));

            kill(cl_pid, SIGINT);
        }
        if (!isChild)
        {
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
        }

        unlink(SERVER_FIFO_NAME);
        unlink(client_fifo_request);

        sem_unlink(SERVER_SEMAPHORE);
        sem_unlink(client_semaphore_request);

        shm_unlink(SHARED_WORKERCOUNT);
        shm_unlink(SHARED_WORKERSLIST);

        printf("Server stopped by SIGINT\n");

        exit(EXIT_SUCCESS);
    }

    else if (signal == SIGUSR1)
    {
        printf("killServer signal received. Terminating...\n");

        GetTimestamp(timebuffer);
        sprintf(buf, "[%s] SERVER: killServer signal received from a client. Terminating...\n", timebuffer);
        write(logFile_fd, buf, strlen(buf));

        for (int i = 0; i < maxClients; i++)
        {
            if (workerlist[i] != 0)
            {
                kill(workerlist[i], SIGINT);
            }
        }

        kill(getpid(), SIGINT);
    }
}

int main(int argc, char *argv[])
{
    queue = createQueue();
    isChild = 0;
    if (argc != 3)
    {
        printf("Usage: %s <dirname> <max. #ofClients>\n", argv[0]);
        exit(1);
    }

    char *dir_name = argv[1];
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

    // A SHARED MEMORY ALLOCATED TO HOLD NUMBER OF ACTIVE WORKERS
    char shrd_count[MAX_PATH];
    strcpy(shrd_count, SHARED_WORKERCOUNT);

    count_fd = shm_open(shrd_count, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (count_fd == -1)
    {
        perror("Opening shared memory.");
        exit(1);
    }

    if (ftruncate(count_fd, sizeof(int)) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    counter = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, count_fd, 0);
    if (counter == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    *counter = 0;

    // A SHARED MEMORY ALLOCATED TO HOLD THE LIST OF ACTIVE WORKERS
    char shrd_workerlist[MAX_PATH];
    strcpy(shrd_workerlist, SHARED_WORKERCOUNT);

    workers_fd = shm_open(shrd_workerlist, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (workers_fd == -1)
    {
        perror("Opening shared memory.");
        exit(1);
    }

    if (ftruncate(workers_fd, sizeof(int) * maxClients) == -1)
    {
        perror("ftruncate");
        exit(1);
    }

    workerlist = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, workers_fd, 0);
    if (workerlist == MAP_FAILED)
    {
        perror("mmap");
        exit(1);
    }

    for (int i = 0; i < maxClients; i++)
    {
        workerlist[i] = 0;
    }

    // SERVER FIFO
    if (mkfifo(SERVER_FIFO_NAME, 0666) == -1)
    {
        perror("Error creating server FIFO");
        exit(1);
    }

    printf("biboServer Here %d\n", maxClients);
    printf("Waiting for clients.\n");

    if ((server_fd = open(SERVER_FIFO_NAME, O_RDONLY)) == -1)
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

            if (!strcmp(con, "tryConnect") && *counter == maxClients)
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

                continue;
            }
            enQueue(queue, cl_pid);
            printf("Client PID %d is on queue.\n", cl_pid);

            GetTimestamp(timebuffer);
            sprintf(buf, "[%s] SERVER: Client PID%d is on queue.\n", timebuffer, cl_pid);
            write(logFile_fd, buf, strlen(buf));
        }

        // SERVING THE NEXT CLIENT FROM THE QUEUE
        if (*counter < maxClients)
        {
            cl_pid = deQueue(queue);
            if (cl_pid == 0)
            {
                continue;
            }
            clientCounter++;

            (*counter)++;

            int childPid = fork();
            if (childPid == 0)
            {
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
                isChild = 1;

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

                printf("Client PID %d connected as client%d\n", cl_pid, clientCounter);

                GetTimestamp(timebuffer);
                sprintf(buf, "[%s] WORKER%d: Serving Client%d with PID%d\n", timebuffer, getpid(), clientCounter, cl_pid);
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
                        sprintf(buf, "[%s] WORKER%d: Client%d requested: %s", timebuffer, getpid(), clientCounter, message);
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
                                read_file(dir_name, file_name, line_number, client_fd_response);
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
                            printf("Client%d disconnected.\n", clientCounter);
                            sem_post(cl_sem_res);

                            GetTimestamp(timebuffer);
                            sprintf(buf, "[%s] WORKER%d: Client%d is quiting.\n", timebuffer, getpid(), clientCounter);
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
                        sprintf(buf, "[%s] WORKER%d: Client%d request is handled.\n", timebuffer, getpid(), clientCounter);
                        write(logFile_fd, buf, strlen(buf));

                        sem_post(cl_sem_res);
                    }
                }

                (*counter)--;

                close(client_fd_response);
                close(client_fd_request);
                close(count_fd);

                sem_post(sv_sem);

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

                for (int i = 0; i < maxClients; i++)
                {
                    if (workerlist[i] == getpid())
                    {
                        workerlist[i] = 0;
                        break;
                    }
                }

                GetTimestamp(timebuffer);
                sprintf(buf, "[%s] WORKER%d: Client%d exited. Terminating..\n", timebuffer, getpid(), clientCounter);
                write(logFile_fd, buf, strlen(buf));

                exit(0);
            }
            else
            {
                for (int i = 0; i < maxClients; i++)
                {
                    if (workerlist[i] == 0)
                    {
                        workerlist[i] = childPid;
                        break;
                    }
                }
            }
        }
    }

    /* close the server FIFO */
    close(server_fd);
    close(count_fd);
    sem_close(sv_sem);

    if (shm_unlink(SHARED_WORKERCOUNT) == -1)
    {
        perror("Error unlinking worker count shared memory.");
        exit(1);
    }

    if (shm_unlink(SHARED_WORKERSLIST) == -1)
    {
        perror("Error unlinking worker list shared memory.");
        exit(1);
    }

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

    return 0;
}
