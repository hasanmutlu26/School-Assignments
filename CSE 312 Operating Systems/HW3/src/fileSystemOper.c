#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#include "filesystem.h"

int inputControl(int argc, char* argv[]);
void dir(uint8_t *filesystem, char *path);
void dumpe2fs(uint8_t* filesystem);

int main(int argc, char* argv[]){
    if(inputControl(argc, argv)){
        fprintf(stderr, "Invalid usage.\n");
        exit(EXIT_FAILURE);
    }

    FILE *fp;
    uint8_t *filesystem;
    filesystem = malloc(FILESYSTEM_SIZE*MB);
    fp = fopen(argv[1], "rb");
    if (NULL == fp) {
        fprintf(stderr, "fopen error!\n");
        exit(EXIT_FAILURE);
    }
    if (1 != fread(filesystem, FILESYSTEM_SIZE*MB, 1, fp)) {
        fprintf(stderr, "fread error!\n");
        exit(EXIT_FAILURE);
    }
    fclose(fp);

    if (!strcmp(argv[2], "dir")) {
        if (NULL == argv[3]) {
            fprintf(stderr, "Invalid usage!\n");
            exit(EXIT_FAILURE);
        }
        dir(filesystem, argv[3]);
    }
    if(!strcmp(argv[2], "dumpe2fs")){
        dumpe2fs(filesystem);
    }


    free(filesystem);
}

int inputControl(int argc, char* argv[]){
    if(argc < 3)
        return 1;
    
    if( ((!strcmp(argv[2], "dumpe2fs") && argc == 3) ||
        ((!strcmp(argv[2], "dir") ||
        !strcmp(argv[2], "mkdir") ||
        !strcmp(argv[2], "rmdir") ||
        !strcmp(argv[2], "del")) && argc == 4)) ||
        ((!strcmp(argv[2], "write") ||
        !strcmp(argv[2], "read")) && 
        argc == 5) ){
        return 0;
    }

    return 1;
}

void dir(uint8_t* filesystem, char* path){
    struct Inode *inode = (struct Inode*)(filesystem + SIZEOF_SUPERBLOCK);
    struct Superblock *s = (struct Superblock*)filesystem;
    int num = inode->size / SIZEOF_FILE;
    struct File* file;
    printf("%s %13s %7s %6s\n","Modification Time", "Type", "Size", "Name");
    for(int i = 0; i < num; i++){
        file = (struct File*)(filesystem + s->startblock + i*SIZEOF_FILE);
        time_t t = (time_t)(inode->lastmodif);
        char *timeStr = asctime(gmtime(&t));
        timeStr[strlen(timeStr)-1] = '\0';
        char type[10];
        if(inode->type == FILETYPE)
            strcpy(type, "<FILE>");
        else if(inode->type == DIR)
            strcpy(type, "<DIR>");
        else
            strcpy(type, "<LINK>");
        printf("%s %7s %5u    %s\n", timeStr, type, inode->size, file->name);
    }
}

void dumpe2fs(uint8_t* filesystem){
    struct Superblock *s = (struct Superblock*)filesystem;
    fprintf(stderr, "Block Size     : %uKB\n", s->blocksize);
    fprintf(stderr, "Total Blocks   : %u\n", s->numblocks);
    fprintf(stderr, "Total Inodes   : %u\n", s->numinodes);
    fprintf(stderr, "Files in Block : %u\n", s->numfiles);
}
