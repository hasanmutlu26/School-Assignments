#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include "filesync.h"

#define MAX_CLIENTS 1024

pthread_mutex_t mutex;
pthread_cond_t cond;
int active_threads = 0;
int thread_pool_size;
int client_sockets[MAX_CLIENTS];
int server_socket, client_socket;
char *directory;

int clientCounter = 1;

void signal_handler(int signum)
{
    printf("Received SIGINT signal. Shutting down the server.\n");
    sprintf(buf, "[%s] SERVER: Received SIGINT signal. Shutting down the server.\n", GetTimestamp(timebuffer));
    write(logFile_fd, buf, strlen(buf));

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        int client_socket = client_sockets[i];
        if (client_socket > 0)
        {
            close(client_socket);
        }
    }

    close(server_socket);
    close(logFile_fd);
    exit(0);
}

void *handle_client(void *arg)
{
    int index = *(int *)arg;
    int client_socket;
    int myClient;
    time_t latest_mod;
    time_t new_mod;
    time_t client_latest_mod;
    time_t client_new_mod;

    while (1)
    {
        // WAIT FOR CLIENT
        pthread_mutex_lock(&mutex);
        while (client_sockets[index] == 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        client_socket = client_sockets[index];
        pthread_mutex_unlock(&mutex);

        ssize_t bytes_received;

        // Send connected message to client
        const char *connected_message = "Connected to server\n";
        if (send(client_socket, connected_message, strlen(connected_message), 0) == -1)
        {
            perror("Failed to send connected message to client");
            break;
        }

        myClient = clientCounter;
        printf("Worker%d: Client%d is connected to server.\n", index, myClient);
        clientCounter++;

        sprintf(buf, "[%s] Worker%d: Client%d is connected to server.\n", GetTimestamp(timebuffer), index, myClient);
        write(logFile_fd, buf, strlen(buf));

        // Make initial synchronization to the client
        client_latest_mod = send_directory(client_socket, directory, directory);
        latest_mod = get_latest_modification_time(directory);
        printf("Worker%d: First synchronization is made to client%d.\n", index, myClient);

        sprintf(buf, "[%s] Worker%d: First synchronization is made to client%d.\n", GetTimestamp(timebuffer), index, myClient);
        write(logFile_fd, buf, strlen(buf));

        while (1)
        {
            if (client_latest_mod == -1)
            {
                perror("Error sending directory");
                break;
            }

            new_mod = get_latest_modification_time(directory);

            // Receive latest modification time of client
            bytes_received = recv(client_socket, &client_new_mod, sizeof(client_new_mod), 0);

            if (bytes_received == -1)
            {
                perror("Failed to receive data from client");
                break;
            }
            else if (bytes_received == 0)
            {
                printf("Worker%d: Client%d disconnected\n", index, myClient);
                sprintf(buf, "[%s] Worker%d: Client%d disconnected\n", GetTimestamp(timebuffer), index, myClient);
                write(logFile_fd, buf, strlen(buf));
                break;
            }

            // Send latest modification time to client
            if (send(client_socket, &new_mod, sizeof(new_mod), 0) == -1)
            {
                perror("Failed to send response to client");
                break;
            }

            // Compare differences and send-receive data if there is change in either side.
            time_t my_difference = new_mod - latest_mod;
            time_t cli_difference = client_new_mod - client_latest_mod;

            if (my_difference == 0 && cli_difference == 0)
            {
                sleep(1);
                continue;
            }

            else
            {
                if (cli_difference > my_difference)
                {
                    sprintf(buf, "[%s] Worker%d: There is change in client%d. Synchronizing...\n", GetTimestamp(timebuffer), index, myClient);
                    write(logFile_fd, buf, strlen(buf));
                    client_latest_mod = receive_directory(client_socket, directory);
                    latest_mod = get_latest_modification_time(directory);
                }
                else if (my_difference > cli_difference)
                {
                    sprintf(buf, "[%s] Worker%d: There is change in server side. Synchronizing to client%d...\n", GetTimestamp(timebuffer), index, myClient);
                    write(logFile_fd, buf, strlen(buf));
                    client_latest_mod = send_directory(client_socket, directory, directory);
                    latest_mod = new_mod;
                }
            }
        }

        close(client_socket);

        pthread_mutex_lock(&mutex);
        client_sockets[index] = 0;
        active_threads--;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        printf("Usage: %s [directory] [threadPoolSize] [portNumber]\n", argv[0]);
        return 1;
    }

    directory = argv[1];
    thread_pool_size = atoi(argv[2]);
    int port_number = atoi(argv[3]);

    if (thread_pool_size > MAX_CLIENTS)
    {
        printf("Thread pool size can't be greater than %d\n", MAX_CLIENTS);
        exit(1);
    }

    if (thread_pool_size == 0 || port_number == 0)
    {
        printf("Invalid [threadPoolSize] [portNumber]");
        exit(1);
    }

    // Open/create directory
    struct stat st;
    if (stat(directory, &st) == 0)
    {
        if (S_ISDIR(st.st_mode))
        {
            printf("Directory '%s' already exists.\n", directory);
        }
        else
        {
            printf("A file with the name '%s' already exists. Please provide a different directory name.\n", directory);
            return 1;
        }
    }
    else
    {
        if (mkdir(directory, 0777) == 0)
        {
            printf("Directory '%s' created successfully.\n", directory);
        }
        else
        {
            printf("Failed to create directory '%s'.\n", directory);
            return 1;
        }
    }

    sprintf(logfile_name, "%s/%s", directory, LOGFILENAME);
    logFile_fd = open(logfile_name, O_CREAT | O_TRUNC | O_WRONLY, 0666);

    sprintf(buf, "[%s] SERVER: Directory %s is opened.\n", GetTimestamp(timebuffer), directory);
    write(logFile_fd, buf, strlen(buf));

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length;

    // CREATE SERVER SOCKET
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Failed to create socket");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // BIND
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Failed to bind");
        exit(1);
    }

    // LISTEN
    if (listen(server_socket, thread_pool_size) == -1)
    {
        perror("Failed to listen");
        exit(1);
    }

    pthread_t thread_pool[thread_pool_size];
    int thread_indices[thread_pool_size];
    int i;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    for (i = 0; i < thread_pool_size; i++)
    {
        thread_indices[i] = i;
        pthread_create(&thread_pool[i], NULL, handle_client, (void *)&thread_indices[i]);
    }

    printf("Server started.\n");

    sprintf(buf, "[%s] SERVER: Server started.\n", GetTimestamp(timebuffer));
    write(logFile_fd, buf, strlen(buf));

    while (1)
    {
        client_address_length = sizeof(client_address);

        // ACCEPT
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1)
        {
            perror("Failed to accept client connection");
            exit(1);
        }

        // CHECK POOL
        pthread_mutex_lock(&mutex);
        while (active_threads == thread_pool_size)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        for (i = 0; i < thread_pool_size; i++)
        {
            if (client_sockets[i] == 0)
            {
                client_sockets[i] = client_socket;
                break;
            }
        }

        active_threads++;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}
