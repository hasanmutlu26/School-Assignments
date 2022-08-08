#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>
#include"filesystem.h"


int blockSize;
char* filePath;

int numblocks;

int inputControl(int argc, char* argv[]);

// x*in + x*blocksize*KB = FILESYSTEM_SIZEMB - superblock
//x = (16MB - superblock) / (in + blocksize*KB)

int main(int argc, char* argv[]){
    if(inputControl(argc, argv)){
        fprintf(stderr, "Usage: ./makeFileSystem blocksize file\n");
        fprintf(stderr, "Example: ./makeFileSystem 4 mySystem.dat\n");
        exit(EXIT_FAILURE);
    }
    printf("Blocksize: %d\n", blockSize);
    numblocks = (FILESYSTEM_SIZE*MB - SIZEOF_SUPERBLOCK) / (SIZEOF_INODE + blockSize*KB);
    printf("superblock: %ld\ninode: %ld\n", SIZEOF_SUPERBLOCK, SIZEOF_INODE);
    printf("numblocks: %d\n", numblocks);

    printf("inode bytes: %ld\n", numblocks*SIZEOF_INODE);
    printf("blocks bytes: %d\n", numblocks*blockSize*KB);

    uint8_t* filesystem;
    filesystem = malloc(FILESYSTEM_SIZE*MB);
    for(long i = 0; i < FILESYSTEM_SIZE*MB; i++){
        filesystem[i] = 0;
    }
    struct Superblock *super = (struct Superblock*)filesystem;
    super->blocksize = blockSize;
    super->numblocks = numblocks;
    super->numinodes = numblocks;
    super->numfiles = (blockSize * KB) / SIZEOF_FILE;
    super->startinode = SIZEOF_SUPERBLOCK;
    super->startblock = SIZEOF_SUPERBLOCK + numblocks*SIZEOF_INODE;
    
    printf("filescount: %d\n", super->numfiles);
    struct Inode *root = (struct Inode*)(filesystem + super->startinode);
    root->numlinks = 1;
    root->type = DIR;
    root->size = 3*SIZEOF_FILE;
    root->lastmodif = time(NULL);
    root->directBlock[0] = 0;

    struct File* f1 = (struct File*)(filesystem + super->startblock);
    f1->inode = 0;
    strcpy(f1->name, ".");

    struct File* f2 = (struct File*)(filesystem + super->startblock + SIZEOF_FILE);
    f2->inode = 0;
    strcpy(f2->name, "..");

    struct File* f3 = (struct File*)(filesystem + super->startblock + 2*SIZEOF_FILE);
    f3->inode = 0;
    strcpy(f3->name, "bin");

    FILE *fp = fopen(filePath, "wb+");
    if (NULL == fp) {
        perror("fopen");
    }
    if (1 != fwrite(filesystem, FILESYSTEM_SIZE*MB, 1, fp)) {
        fprintf(stderr, "fwrite error!\n");
        return EXIT_FAILURE;
    }
    fclose(fp);

    free(filesystem);
    return EXIT_SUCCESS;
}

int inputControl(int argc, char* argv[]){
    if(argc != 3)
        return 1;
    blockSize = atoi(argv[1]);
    if(blockSize <= 0)
        return 1;
    filePath = argv[2];
    return 0;
}