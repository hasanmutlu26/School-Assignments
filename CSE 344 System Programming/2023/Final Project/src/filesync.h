#define _XOPEN_SOURCE 700

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/select.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>

#define BUFFER_SIZE 1024

#define LOGFILENAME "logfile.txt"
int logFile_fd;
char logfile_name[BUFFER_SIZE];
char buf[BUFFER_SIZE];
char timebuffer[BUFFER_SIZE];

struct file_info
{
    int isDir;
    long file_size;
    char file_name[512];
};

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

time_t get_latest_modification_time(const char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        return -1;
    }

    struct dirent *entry;
    struct stat st0;
    if (stat(path, &st0) == -1)
    {
        perror("stat");
        return -1;
    }
    time_t latest_time = st0.st_mtime;

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, LOGFILENAME) == 0)
        {
            continue;
        }

        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(full_path, &st) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(st.st_mode))
        {
            time_t subdirectory_time = get_latest_modification_time(full_path);
            if (subdirectory_time > latest_time)
            {
                latest_time = subdirectory_time;
            }
        }
        else
        {
            if (st.st_mtime > latest_time)
            {
                latest_time = st.st_mtime;
            }
        }
    }

    closedir(dir);

    return latest_time;
}

void compareFiles(const char *path1, const char *path2)
{

    if (!strcmp(path1, LOGFILENAME) || !strcmp(path2, LOGFILENAME))
    {
        return;
    }

    FILE *file1, *file2;
    file1 = fopen(path1, "rb");
    file2 = fopen(path2, "rb");

    // if (file1 == NULL || file2 == NULL) {
    //     printf("Error opening files: %s, %s\n", path1, path2);
    //     return;
    // }

    if (file1 == NULL)
    {
        printf("File deleted: %s\n", path1);
        fclose(file2);
        return;
    }

    else if (file2 == NULL)
    {
        printf("File created: %s\n", path2);

        sprintf(buf, "[%s] File created: %s.\n", GetTimestamp(timebuffer), path2);
        write(logFile_fd, buf, strlen(buf));

        fclose(file1);
        return;
    }

    char buffer1[1024];
    char buffer2[1024];
    size_t bytesRead1, bytesRead2;

    do
    {
        bytesRead1 = fread(buffer1, 1, sizeof(buffer1), file1);
        bytesRead2 = fread(buffer2, 1, sizeof(buffer2), file2);

        if (bytesRead1 != bytesRead2 || memcmp(buffer1, buffer2, bytesRead1) != 0)
        {
            printf("File updated: %s\n", path2);

            sprintf(buf, "[%s] File updated: %s.\n", GetTimestamp(timebuffer), path2);
            write(logFile_fd, buf, strlen(buf));

            fclose(file1);
            fclose(file2);
            return;
        }
    } while (bytesRead1 > 0 && bytesRead2 > 0);

    fclose(file1);
    fclose(file2);
}

void compareDirectories(const char *dirPath1, const char *dirPath2)
{

    DIR *dir1;
    struct dirent *entry1;
    struct stat stat1;
    char path1[1024], path2[1024];

    dir1 = opendir(dirPath1);

    if (dir1 == NULL)
    {
        printf("Error opening directory: %s\n", dirPath1);
        return;
    }

    while ((entry1 = readdir(dir1)) != NULL)
    {
        snprintf(path1, sizeof(path1), "%s/%s", dirPath1, entry1->d_name);
        snprintf(path2, sizeof(path2), "%s/%s", dirPath2, entry1->d_name);

        if (stat(path1, &stat1) == -1)
        {
            printf("Error getting file information: %s\n", path1);
            continue;
        }

        if (S_ISDIR(stat1.st_mode))
        {
            if (strcmp(entry1->d_name, ".") != 0 && strcmp(entry1->d_name, "..") != 0 && strcmp(entry1->d_name, LOGFILENAME) != 0)
            {
                compareDirectories(path1, path2);
            }
        }
        else if (S_ISREG(stat1.st_mode))
        {
            compareFiles(path1, path2);
        }
    }

    closedir(dir1);

    DIR *dir2;
    struct dirent *entry2;
    char path2Only[1024];

    dir2 = opendir(dirPath2);

    if (dir2 == NULL)
    {
        return;
    }

    while ((entry2 = readdir(dir2)) != NULL)
    {
        snprintf(path2Only, sizeof(path2Only), "%s/%s", dirPath2, entry2->d_name);

        if (!strcmp(entry2->d_name, LOGFILENAME))
        {
            continue;
        }

        snprintf(path1, sizeof(path1), "%s/%s", dirPath1, entry2->d_name);
        if (access(path1, F_OK) == -1)
        {
            printf("File deleted: %s\n", path2Only);

            sprintf(buf, "[%s] File deleted: %s.\n", GetTimestamp(timebuffer), path2);
            write(logFile_fd, buf, strlen(buf));
        }
    }

    closedir(dir2);
}

void remove_directory(const char *path)
{
    struct stat st;
    if (stat(path, &st) == -1)
    {
        perror("stat");
        return;
    }

    if (!S_ISDIR(st.st_mode))
    {
        fprintf(stderr, "Not a directory: %s\n", path);
        return;
    }

    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        struct stat entry_st;
        if (stat(full_path, &entry_st) == -1)
        {
            perror("stat");
            continue;
        }

        if (S_ISDIR(entry_st.st_mode))
        {
            remove_directory(full_path);
        }
        else
        {
            if (remove(full_path) != 0)
            {
                perror("remove");
                continue;
            }
        }
    }

    closedir(dir);

    if (rmdir(path) != 0)
    {
        perror("rmdir");
        return;
    }
}

#include <stdio.h>

int copyFile(const char *sourceFile, const char *destinationFile)
{
    FILE *source = fopen(sourceFile, "rb");
    if (source == NULL)
    {
        printf("Failed to open source file: %s\n", sourceFile);
        return 1;
    }

    FILE *destination = fopen(destinationFile, "wb");
    if (destination == NULL)
    {
        printf("Failed to create destination file: %s\n", destinationFile);
        fclose(source);
        return 1;
    }

    int ch;
    while ((ch = fgetc(source)) != EOF)
    {
        fputc(ch, destination);
    }

    fclose(source);
    fclose(destination);

    return 0;
}

void send_file(int client_socket, const char *file_path, const char *main_dir)
{
    FILE *file = fopen(file_path, "rb");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    struct file_info fileinf;

    fseek(file, 0, SEEK_END);
    fileinf.file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    strcpy(fileinf.file_name, file_path + strlen(main_dir) + 1);

    if (send(client_socket, &fileinf, sizeof(fileinf), 0) == -1)
    {
        perror("Failed to send file size to client");
        fclose(file);
        return;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (send(client_socket, buffer, bytes_read, 0) == -1)
        {
            perror("Failed to send file contents to client");
            fclose(file);
            return;
        }
    }

    fclose(file);
}

time_t send_directory(int client_socket, const char *main_dir, const char *dir_path)
{
    DIR *dir = opendir(dir_path);
    if (dir == NULL)
    {
        perror("Failed to open directory");
        return -1;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, LOGFILENAME) == 0)
        {
            continue;
        }

        char entry_path[BUFFER_SIZE];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", dir_path, entry->d_name);

        struct stat file_stat;
        if (stat(entry_path, &file_stat) == -1)
        {
            perror("Failed to get file status");
            continue;
        }

        if (S_ISREG(file_stat.st_mode))
        {
            send_file(client_socket, entry_path, main_dir);
        }
        else if (S_ISDIR(file_stat.st_mode))
        {
            struct file_info fileinf;
            fileinf.isDir = 1;
            strcpy(fileinf.file_name, entry_path + strlen(main_dir) + 1);

            if (send(client_socket, &fileinf, sizeof(fileinf), 0) == -1)
            {
                perror("Failed to send directory to client");
                return -1;
            }

            send_directory(client_socket, main_dir, entry_path);
        }
    }

    if (!strcmp(main_dir, dir_path))
    {
        struct file_info fileinf;
        fileinf.isDir = 0;
        fileinf.file_size = -1;

        if (send(client_socket, &fileinf, sizeof(fileinf), 0) == -1)
        {
            perror("Failed to send file size to client");
            return -1;
        }

        time_t myLatest = get_latest_modification_time(dir_path);
        time_t otherLatest;

        if (send(client_socket, &myLatest, sizeof(myLatest), 0) == -1)
        {
            perror("Failed to send latest modification time");
            return -1;
        }

        if (recv(client_socket, &otherLatest, sizeof(otherLatest), 0) == -1)
        {
            perror("Failed to receive latest modification time");
            return -1;
        }

        closedir(dir);

        return otherLatest;
    }

    return 0;
}

int receive_file(int server_socket, const char *dir_name)
{
    struct file_info fileinf;
    if (recv(server_socket, &fileinf, sizeof(fileinf), 0) == -1)
    {
        perror("Failed to receive file size from server");
        return 1;
    }

    if (fileinf.isDir == 1)
    {
        char file_path[1024];
        snprintf(file_path, sizeof(file_path), "%s/%s", dir_name, fileinf.file_name);
        mkdir(file_path, 0777);
        return 0;
    }

    long file_size = fileinf.file_size;

    if (file_size == -1)
    {
        return -1;
    }

    char file_path[1024];
    snprintf(file_path, sizeof(file_path), "%s/%s", dir_name, fileinf.file_name);

    FILE *file = fopen(file_path, "wb");
    if (file == NULL)
    {
        perror("Failed to create file");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    size_t bytes_received;
    int bytes_expected = sizeof(buffer);
    while (file_size > 0)
    {
        if (file_size < sizeof(buffer))
            bytes_expected = file_size;

        bytes_received = recv(server_socket, buffer, bytes_expected, 0);
        if (bytes_received == -1)
        {
            perror("Failed to receive file contents from server");
            fclose(file);
            return 1;
        }

        fwrite(buffer, 1, bytes_received, file);
        file_size -= bytes_received;
    }

    fclose(file);
    return 0;
}

time_t receive_directory(int server_socket, const char *dir_path)
{
    char tempfilepath[512];
    snprintf(tempfilepath, sizeof(tempfilepath), "%s-tmp", dir_path);

    mkdir(tempfilepath, 0777);

    while (1)
    {
        if (receive_file(server_socket, tempfilepath) == -1)
            break;
    }

    compareDirectories(tempfilepath, dir_path);

    char logpath1[BUFFER_SIZE];
    char logpath2[BUFFER_SIZE];
    snprintf(logpath1, sizeof(logpath1), "%s/%s", dir_path, LOGFILENAME);
    snprintf(logpath2, sizeof(logpath2), "%s/%s", tempfilepath, LOGFILENAME);

    copyFile(logpath1, logpath2);
    close(logFile_fd);

    remove_directory(dir_path);

    if (rename(tempfilepath, dir_path) != 0)
    {
        perror("rename");
        return EXIT_FAILURE;
    }

        logFile_fd = open(logfile_name, O_APPEND | O_WRONLY, 0666);

    time_t myLatest = get_latest_modification_time(dir_path);
    time_t otherLatest;

    if (recv(server_socket, &otherLatest, sizeof(otherLatest), 0) == -1)
    {
        perror("Failed to receive latest modification time");
        return -1;
    }

    if (send(server_socket, &myLatest, sizeof(myLatest), 0) == -1)
    {
        perror("Failed to send latest modification time");
        return -1;
    }

    return otherLatest;
}
