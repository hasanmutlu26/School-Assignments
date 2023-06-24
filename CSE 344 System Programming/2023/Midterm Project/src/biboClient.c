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
#include <semaphore.h>

#include "utils.h"

int server_fd, client_fd_response, client_fd_request;
char message[MSG_SIZE],
    client_fifo_response[MAX_PATH], client_fifo_request[MAX_PATH],
    client_semaphore_response[MAX_PATH], client_semaphore_request[MAX_PATH];

void signal_handler(int signal)
{
    unlink(client_fifo_response);

    sem_unlink(client_semaphore_response);

    printf("Client stopped by SIGINT. Server may be terminated.\n");

    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
    sem_t *sv_sem, *cl_sem_req, *cl_sem_res;

    if (argc < 3 || (!strcmp(argv[1], "Connect") && !strcmp(argv[1], "tryConnect")))
    {
        printf("Usage: %s <Connect/tryConnect> ServerPID\n", argv[0]);
        return 1; // Exit program with error status
    }

    int server_pid = atoi(argv[2]);


    if (server_pid <= 0)
    {
        printf("Invalid server PID.\n");
        exit(1);
    }

    // signal(SIGINT, signal_handler);
    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    if ((server_fd = open(SERVER_FIFO_NAME, O_WRONLY)) == -1)
    {
        perror("Error opening server FIFO");
        exit(1);
    }

    sprintf(client_fifo_response, "%s%d", CLIENT_RESPOND_FIFO, getpid());
    sprintf(client_fifo_request, "%s%d", CLIENT_REQUEST_FIFO, getpid());
    sprintf(client_semaphore_response, "%s%d", CLIENT_SEMAPORE_RES, getpid());
    sprintf(client_semaphore_request, "%s%d", CLIENT_SEMAPORE_REQ, getpid());

    if (mkfifo(client_fifo_response, 0666) == -1)
    {
        perror("Error creating client FIFO");
        exit(1);
    }

    cl_sem_res = sem_open(client_semaphore_response, O_CREAT, 0666, 0);
    if (cl_sem_res == SEM_FAILED)
    {
        perror("Error opening client response semaphore");
        exit(1);
    }

    sv_sem = sem_open(SERVER_SEMAPHORE, 0);
    if (sv_sem == SEM_FAILED)
    {
        perror("Error opening server semaphore");
        exit(1);
    }

    sprintf(message, "%d %s", getpid(), argv[1]);
    if (write(server_fd, message, sizeof(message)) == -1)
    {
        perror("Error writing to server FIFO");
        exit(1);
    }

    sem_post(sv_sem);

    close(server_fd);

    printf("Waiting for Que..\n");

    if ((client_fd_response = open(client_fifo_response, O_RDONLY)) == -1)
    {
        perror("Error opening client FIFO");
        exit(1);
    }
    sem_wait(cl_sem_res);

    if (read(client_fd_response, message, sizeof(message)) > 0)
    {
        if (!strncmp(message, "connected", strlen("connected")))
        {
            printf("Connection established.\n");
        }

        else if (!strncmp(message, "quit", 4))
        {
            puts("Queue is full. Exiting.");

            close(client_fd_response);

            if (unlink(client_fifo_response) == -1)
            {
                perror("Error removing client FIFO");
                exit(1);
            }

            sem_close(sv_sem);

            if (sem_unlink(client_semaphore_response) == -1)
            {
                perror("Error unlinking client semaphore.");
                exit(1);
            }

            exit(0);
        }
    }

    cl_sem_req = sem_open(client_semaphore_request, 0);
    if (cl_sem_req == SEM_FAILED)
    {
        perror("Error opening client request semaphore");
        exit(1);
    }

    if ((client_fd_request = open(client_fifo_request, O_WRONLY)) == -1)
    {
        perror("Error opening client request FIFO");
        exit(1);
    }

    int quitFlag = 0;
    while (1)
    {
        printf("> ");
        fgets(message, MSG_SIZE, stdin);

        if (!strncmp(message, "killServer", 10))
        {
            printf("Server is closing.\n");
            kill(server_pid, SIGUSR1);
            break;
        }

        if (write(client_fd_request, message, sizeof(message)) == -1)
        {
            perror("Error writing to server FIFO");
            exit(1);
        }

        sem_post(cl_sem_req);

        quitFlag = !strncmp(message, "quit", 4);

        sem_wait(cl_sem_res);

        while (1)
        {
            if (read(client_fd_response, message, sizeof(message)) > 0)
            {
                if (!strcmp(message, "**done**"))
                {
                    break;
                }
                printf("%s", message);
            }
        }
        printf("\n");

        if (quitFlag)
        {
            break;
        }
    }

    close(client_fd_response);

    if (unlink(client_fifo_response) == -1)
    {
        perror("Error removing client FIFO");
        exit(1);
    }

    sem_close(cl_sem_req);
    sem_close(cl_sem_res);
    sem_close(sv_sem);

    if (sem_unlink(client_semaphore_response) == -1)
    {
        perror("Error unlinking client response semaphore.");
        exit(1);
    }

    return 0;
}
