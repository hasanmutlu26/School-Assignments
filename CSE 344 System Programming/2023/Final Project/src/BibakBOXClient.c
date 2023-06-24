#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include "filesync.h"

int server_socket;

void signal_handler(int signum)
{
    printf("Received SIGINT signal. Closing client connection.\n");
    sprintf(buf, "[%s] Received SIGINT signal. Closing client connection.\n", GetTimestamp(timebuffer));
    write(logFile_fd, buf, strlen(buf));
    close(server_socket);
    close(logFile_fd);
    exit(0);
}

int main(int argc, char *argv[])
{
    if (argc < 3 || argc > 4)
    {
        printf("Usage: %s [dirName] [portNumber] [ipAddr(optional)]\n", argv[0]);
        return 1;
    }

    const char *directory = argv[1];
    int port_number = atoi(argv[2]);
    const char *ip_address = argc == 4 ? argv[3] : "127.0.0.1";

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);

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

    sprintf(buf, "[%s] Directory %s is opened.\n", GetTimestamp(timebuffer), directory);
    write(logFile_fd, buf, strlen(buf));

    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    // CREATE SERVER SOCKET
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Failed to create socket");
        exit(1);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr = inet_addr(ip_address);

    // CONNECT
    if (connect(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        perror("Failed to connect to server");
        exit(1);
    }

    printf("Waiting for connection to server.\n");
    sprintf(buf, "[%s] Waiting for connection to server.\n", GetTimestamp(timebuffer));
    write(logFile_fd, buf, strlen(buf));

    // Receive connected message from server.
    ssize_t bytes_received = recv(server_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received == -1)
    {
        perror("Failed to receive connected message from server");
        close(server_socket);
        exit(1);
    }
    else if (bytes_received == 0)
    {
        printf("Server disconnected\n");
        sprintf(buf, "[%s] Server disconnected.\n", GetTimestamp(timebuffer));
        write(logFile_fd, buf, strlen(buf));
        close(server_socket);
        exit(1);
    }

    buffer[bytes_received] = '\0';
    printf("Connected message from server: %s", buffer);
    sprintf(buf, "[%s] Connected to server.\n", GetTimestamp(timebuffer));
    write(logFile_fd, buf, strlen(buf));

    time_t latest_mod;
    time_t new_mod;
    time_t server_latest_mod;
    time_t server_new_mod;

    // Make initial synchronization.
    server_latest_mod = receive_directory(server_socket, directory);
    latest_mod = get_latest_modification_time(directory);
    printf("First synchronization is made from server.\n");
    sprintf(buf, "[%s] First synchronization is made from server\n", GetTimestamp(timebuffer));
    write(logFile_fd, buf, strlen(buf));

    while (1)
    {
        if (server_latest_mod == -1)
        {
            perror("Error receiving directory");
            break;
        }

        new_mod = get_latest_modification_time(directory);

        // Send latest modification time
        if (send(server_socket, &new_mod, sizeof(new_mod), 0) == -1)
        {
            perror("Failed to send message to server");
            break;
        }

        // Receive latest modification time
        ssize_t bytes_received = recv(server_socket, &server_new_mod, sizeof(server_new_mod), 0);
        if (bytes_received == -1)
        {
            perror("Failed to receive response from server");
            break;
        }
        else if (bytes_received == 0)
        {
            printf("Server disconnected\n");
            sprintf(buf, "[%s] Server disconnected.\n", GetTimestamp(timebuffer));
            write(logFile_fd, buf, strlen(buf));
            break;
        }

        // Compare and send/receive files accordingly
        time_t my_difference = new_mod - latest_mod;
        time_t serv_difference = server_new_mod - server_latest_mod;

        if (my_difference == 0 && serv_difference == 0)
        {
            sleep(1);
            continue;
        }

        else
        {
            if (serv_difference > my_difference)
            {
                printf("There is change in server side.\n");
                sprintf(buf, "[%s] There is change in server side. Synchronizing from server..\n", GetTimestamp(timebuffer));
                write(logFile_fd, buf, strlen(buf));
                server_latest_mod = receive_directory(server_socket, directory);
                latest_mod = get_latest_modification_time(directory);
            }
            else if (my_difference > serv_difference)
            {
                printf("There is change in client side.\n");
                sprintf(buf, "[%s] There is change in client side. Synchronizing to server..\n", GetTimestamp(timebuffer));
                write(logFile_fd, buf, strlen(buf));
                server_latest_mod = send_directory(server_socket, directory, directory);
                latest_mod = new_mod;
            }
        }
    }

    close(server_socket);
    close(logFile_fd);

    return 0;
}
