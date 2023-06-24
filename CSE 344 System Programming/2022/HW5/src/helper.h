#include<stdio.h>
#include<stdlib.h>
#include<string.h>

mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

#define PI 3.14

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
