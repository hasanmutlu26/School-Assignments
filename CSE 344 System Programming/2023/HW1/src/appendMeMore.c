#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
    if(argc < 3 || argc > 4){
        fprintf(stderr, "Usage: %s filename num-bytes [x]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char *filename = argv[1];
    int numBytes = atoi(argv[2]);

    int lseekFlag = 0;
    if(argc == 4 && *argv[3] == 'x'){
        lseekFlag = 1;
    }

    int fileFlags;
    if(lseekFlag){
        fileFlags = O_WRONLY | O_CREAT;
    }
    else{
        fileFlags = O_WRONLY | O_APPEND | O_CREAT;
    }

    int fd = open(filename, fileFlags, S_IWUSR);
    if(fd == -1){
        perror("open");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < numBytes; i++){
        if(lseekFlag){
            if(lseek(fd, 0, SEEK_END) == -1){
                perror("lseek");
                exit(EXIT_FAILURE);
            }
        }
        if(write(fd, "x", 1) == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    if(close(fd) == -1){
        perror("close");
        exit(EXIT_FAILURE);
    }
    
    exit(EXIT_SUCCESS);
}
