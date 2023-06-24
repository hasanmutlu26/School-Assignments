#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include "mydup.c"

char buffer[50];

int main(void){
    int fd1 = open("file.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd1 == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    int fd2 = mydup(fd1);
    if(fd2 == -1){
        perror("mydup");
        exit(EXIT_FAILURE);
    }

    int fd3 = mydup2(fd1, 10);
    if(fd3 == -1){
        perror("mydup2");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "fd1 opens 'file.txt', fd2 and fd3 are created by duplicating it.\n");
    fprintf(stderr, "\nWRITE TEST\n");
    fprintf(stderr, "fd1 writes 'Hello ' to the file.\n");

    strcpy(buffer, "Hello ");
    if(write(fd1, buffer, strlen(buffer)) == -1){
        perror("write fd1");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "fd2 writes 'World!\\n' to the file.\n");

    strcpy(buffer, "World!\n");
    if(write(fd2, buffer, strlen(buffer)) == -1){
        perror("write fd2");
        exit(EXIT_FAILURE);
    }

    fprintf(stderr, "fd3 writes 'System Programming.' to the file.\n");

    strcpy(buffer, "System Programming.");
    if(write(fd3, buffer, strlen(buffer)) == -1){
        perror("write fd3");
        exit(EXIT_FAILURE);
    }


    fprintf(stderr, "\nFile descriptor offset positions after write\n");
    fprintf(stderr, "fd1 position: %ld\n", lseek(fd1, 0, SEEK_CUR));
    fprintf(stderr, "fd2 position: %ld\n", lseek(fd2, 0, SEEK_CUR));
    fprintf(stderr, "fd3 position: %ld\n", lseek(fd3, 0, SEEK_CUR));
    fprintf(stderr, "They must be all in same offset position.\n");


    fprintf(stderr, "\nREAD TEST\n");
    fprintf(stderr, "File descriptor offsets are set to the beginning of the file to read.\n");

    if(lseek(fd1, 0, SEEK_SET) == -1){
        perror("lseek fd1");
        exit(EXIT_FAILURE);
    }

    if(lseek(fd2, 0, SEEK_SET) == -1){
        perror("lseek fd2");
        exit(EXIT_FAILURE);
    }

    if(lseek(fd3, 0, SEEK_SET) == -1){
        perror("lseek fd3");
        exit(EXIT_FAILURE);
    }

    
    fprintf(stderr, "fd3 reads 13 bytes from file.\n");
    memset(buffer, '\0', 50);
    if(read(fd3, buffer, 13) == -1){
        perror("read");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "fd3 read: '%s'\n", buffer);
    

    fprintf(stderr, "fd2 reads 7 bytes from file.\n");
    memset(buffer, '\0', 50);
    if(read(fd2, buffer, 7) == -1){
        perror("read");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "fd2 read: '%s'\n", buffer);

    fprintf(stderr, "fd1 reads 12 bytes from file.\n");
    memset(buffer, '\0', 50);
    if(read(fd1, buffer, 12) == -1){
        perror("read");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "fd1 read: '%s'\n", buffer);

    fprintf(stderr, "\nFile descriptor offset positions after read:\n");
    fprintf(stderr, "fd1 position: %ld\n", lseek(fd1, 0, SEEK_CUR));
    fprintf(stderr, "fd2 position: %ld\n", lseek(fd2, 0, SEEK_CUR));
    fprintf(stderr, "fd3 position: %ld\n", lseek(fd3, 0, SEEK_CUR));
    fprintf(stderr, "They must be all in same offset position.\n\n");


    
    

    if(close(fd1) == -1){
        perror("close fd1");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "fd1 is closed.\n");

    if(close(fd2) == -1){
        perror("close fd2");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "fd2 is closed.\n");


    if(close(fd3) == -1){
        perror("close fd3");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "fd3 is closed.\n");


    fprintf(stderr, "\nmydup2 SPECIAL CASE TEST\n");
    fprintf(stderr, "mydup2 is called with fd1 as both oldfd and newfd.\n");
    fprintf(stderr, "File is already closed so the oldfd is not valid.\n");
    fprintf(stderr, "Program must halt with bad file descriptor error.\n");
    int fd4 = mydup2(fd1, fd1);
    if(fd4 == -1){
        perror("mydup2");
    }

    return 0;
}

