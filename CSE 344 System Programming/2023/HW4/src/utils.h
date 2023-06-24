#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <dirent.h>

#define MSG_SIZE 1024
#define MAX_PATH 256

#define SERVER_FIFO_NAME "/tmp/server_fifo"
#define CLIENT_RESPOND_FIFO "/tmp/client_res_"
#define CLIENT_REQUEST_FIFO "/tmp/client_req_"

#define SHARED_WORKERCOUNT "/shrd_workercount"
#define SHARED_WORKERSLIST "/shrd_workerlist"

#define SERVER_SEMAPHORE "/server_semaphore"
#define CLIENT_SEMAPORE_RES "/client_semaphore_res_"
#define CLIENT_SEMAPORE_REQ "/client_semaphore_req_"

#define LOGFILE_NAME "/log.txt"

void display_help(char *cmd, int client_fd_response, int argnull)
{
    char message[MSG_SIZE];
    if (argnull)
    {
        sprintf(message, "Available commands:\nlist readF writeT upload download quit\n");
    }
    else if (!strcmp(cmd, "list"))
    {
        sprintf(message, "list: Display the list of files in the directory\n");
    }
    else if (!strcmp(cmd, "readF"))
    {
        sprintf(message, "readF <file> <line #>: Display the specified line of the file\n");
    }
    else if (!strcmp(cmd, "writeT"))
    {
        sprintf(message, "writeT <file> <line #> <string>: Write to the specified line of the file\n");
    }
    else if (!strcmp(cmd, "upload"))
    {
        sprintf(message, "upload <file>: Upload the specified file to the directory\n");
    }
    else if (!strcmp(cmd, "download"))
    {
        sprintf(message, "download <file>: Download the specified file from the directory\n");
    }
    else if (!strcmp(cmd, "quit"))
    {
        sprintf(message, "quit: Quit the program\n");
    }
    else
    {
        sprintf(message, "Invalid command: %s\n", cmd);
    }

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

void list_files(char *dir_name, int client_fd_response)
{
    DIR *dir = opendir(dir_name);
    struct dirent *entry;
    char message[MSG_SIZE];
    while ((entry = readdir(dir)) != NULL)
    {
        sprintf(message, "%s\n", entry->d_name);
        if (!strcmp(message, ".\n") || !strcmp(message, "..\n"))
            continue;
        if (write(client_fd_response, message, sizeof(message)) == -1)
        {
            perror("Error writing to client response FIFO");
            exit(1);
        }
    }

    sprintf(message, "**done**");
    if (write(client_fd_response, message, sizeof(message)) == -1)
    {
        perror("Error writing to client response FIFO");
        exit(1);
    }
    closedir(dir);
}

int read_file(char *dir_name, char *file_name, int line_number, int client_fd_response)
{
    char file_path[1024];
    sprintf(file_path, "%s/%s", dir_name, file_name);
    int fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        return errno;
    }
    char buf[1024];
    char message[MSG_SIZE];
    int line_count = 0;
    int bytes_read;

    memset(message, '\0', MSG_SIZE);
    int byteCounter = 0;

    while ((bytes_read = read(fd, buf, 1024)) > 0)
    {
        for (int i = 0; i < bytes_read; i++)
        {
            if (line_count == line_number - 1 || line_number == 0)
            {
                strncat(message, &buf[i], 1);
                byteCounter++;

                if (byteCounter == MSG_SIZE || byteCounter == bytes_read)
                {
                    if (write(client_fd_response, message, sizeof(message)) == -1)
                    {
                        perror("Error writing to client response FIFO");
                        exit(1);
                    }
                    byteCounter = 0;
                    memset(message, '\0', MSG_SIZE);
                }
            }
            if (buf[i] == '\n')
            {
                line_count++;
            }
        }
        if (line_number > 0 && line_count >= line_number)
        {
            if (write(client_fd_response, message, sizeof(message)) == -1)
            {
                perror("Error writing to client response FIFO");
                exit(1);
            }
            break;
        }
    }
    sprintf(message, "**done**");
    if (write(client_fd_response, message, sizeof(message)) == -1)
    {
        perror("Error writing to client response FIFO");
        exit(1);
    }
    close(fd);
    return 0;
}

int write_file(char *dir_name, char *file_name, int line_number, char *text)
{
    char file_path[1024];
    sprintf(file_path, "%s/%s", dir_name, file_name);
    int fd = open(file_path, O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        return errno;
    }
    if (flock(fd, LOCK_EX) == -1)
    {
        return errno;
    }
    if (line_number > 0)
    {
        int line_count = 1;
        char buf[1024];
        int bytes_read;
        lseek(fd, 0, SEEK_SET);
        while (line_count < line_number && (bytes_read = read(fd, buf, 1)) > 0)
        {
            if (buf[0] == '\n')
            {
                line_count++;
            }
        }
    }
    else
    {
        lseek(fd, 0, SEEK_END);
    }

    if (strlen(text) == 0)
    {
        text[0] = '\n';
    }
    flock(fd, LOCK_EX);
    write(fd, text, strlen(text));
    flock(fd, LOCK_UN);
    close(fd);
    return 0;
}

int upload_file(char *dir_name, char *cwd_path, char *file_name)
{
    char file_path[1024];
    sprintf(file_path, "%s/%s", cwd_path, file_name);
    int fd = open(file_path, O_RDONLY);
    if (fd == -1)
    {
        return errno;
    }

    sprintf(file_path, "%s/%s", dir_name, file_name);
    int new_fd = open(file_path, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (new_fd == -1)
    {
        close(fd);
        return errno;
    }

    if (flock(new_fd, LOCK_EX) == -1)
    {
        close(fd);
        close(new_fd);
        return errno;
    }

    int nread;
    char buf[1024];
    while ((nread = read(fd, buf, 1024)) > 0)
    {
        flock(new_fd, LOCK_EX);
        if (write(new_fd, buf, nread) == -1)
        {
            flock(new_fd, LOCK_UN);
            close(fd);
            close(new_fd);
            return errno;
        }
        flock(new_fd, LOCK_UN);
    }

    flock(new_fd, LOCK_UN);
    close(fd);
    close(new_fd);

    return 0;
}

void getCwd(char *cwd, int pid)
{
    char proc_path[MAX_PATH];
    snprintf(proc_path, MAX_PATH, "/proc/%d/cwd", pid);

    ssize_t len = readlink(proc_path, cwd, MAX_PATH - 1);
    if (len == -1)
    {
        perror("readlink");
    }
    cwd[len] = '\0';
}

int download_file(char *dir_name, char *dest_path, char *file_name)
{
    char file_path[1024];
    sprintf(file_path, "%s/%s", dir_name, file_name);
    int src_fd = open(file_path, O_RDONLY);
    if (src_fd == -1)
    {
        return errno;
    }

    sprintf(dest_path, "%s/%s", dest_path, file_name);
    int dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_EXCL, 0666);
    if (dest_fd == -1)
    {
        close(src_fd);
        return errno;
    }

    if (flock(src_fd, LOCK_SH) == -1)
    {
        close(src_fd);
        close(dest_fd);
        return errno;
    }

    int nread;
    char buf[1024];
    while ((nread = read(src_fd, buf, 1024)) > 0)
    {
        flock(dest_fd, LOCK_EX);
        if (write(dest_fd, buf, nread) == -1)
        {
            close(src_fd);
            close(dest_fd);
            return errno;
        }
        flock(dest_fd, LOCK_UN);
    }

    flock(src_fd, LOCK_UN);
    close(src_fd);
    close(dest_fd);
    return 0;
}

char *GetTimestamp(char *str)
{
    time_t timer;
    char temp[500];
    time(&timer);
    strcpy(temp, ctime(&timer));
    int k = 0;
    for (int i = 0; i < strlen(temp); ++i)
    {
        if (temp[i] != '\n')
        {
            str[k] = temp[i];
            ++k;
        }
    }
    str[k] = '\0';
    return str;
}
