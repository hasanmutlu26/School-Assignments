#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h> /* For portability */
#include <sys/sem.h>


union semun { /* Used in calls to semctl() */
    int val;
    struct semid_ds * buf;
    unsigned short * array;
    #if defined(__linux__)
    struct seminfo * __buf;
    #endif
};

mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

char * GetTimestamp(char * str)
{
    time_t timer;
    char temp[500];
    time(&timer);
    // printf("%s", ctime(&timer));
    strcpy(temp, ctime(&timer));
    int k = 0;
    for(int i = 0; i < strlen(temp); ++i)
    {
        if(temp[i] != '\n'){
            str[k] = temp[i];
            ++k;
        }
    }
    str[k] = '\0';
    return str;
}

