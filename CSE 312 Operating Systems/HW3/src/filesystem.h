#include<stdint.h>
#include<time.h>


#define FILESYSTEM_SIZE 16

#define FILETYPE 0
#define DIR 1
#define SYM_LINK 2

#define KB 1024
#define MB 1048576

#define DIRECT 32
#define NAME_LEN 32


#define SIZEOF_SUPERBLOCK sizeof(struct Superblock)
#define SIZEOF_INODE sizeof(struct Inode)
#define SIZEOF_FILE sizeof(struct File)

struct Superblock{
    uint32_t blocksize;
    uint32_t numblocks;
    uint32_t numinodes;
    uint32_t numfiles;
    uint32_t startblock;
    uint32_t startinode;
};

struct Inode{
    uint32_t size;
    time_t lastmodif;
    uint32_t numlinks;
    uint8_t type;

    uint16_t owner; 
    uint16_t group;
    uint16_t protection;

    uint32_t directBlock[DIRECT];
    uint32_t singIndirect;
    uint32_t doubIndirect;
    uint32_t tripIndirect;
};

struct File{
    uint16_t inode;
    char name[NAME_LEN];
};