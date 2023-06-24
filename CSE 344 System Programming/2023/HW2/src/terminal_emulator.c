#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define MAXCOMMANDS 20
#define COMMANDLENGTH 1000
#define LOGFILENAME "log.txt"

char * GetTimestamp(char * str)
{
    time_t timer;
    char temp[500];
    time(&timer);
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

void handleSignal(int sig){
    printf("\nReceived signal SIGINT. Exiting.\n");
    exit(0);
}


void logChilds(int pids, char *command){
    char tmstmp[100];
    FILE *log_file = fopen(LOGFILENAME, "a");
    fprintf(log_file, "[%s]\t%d\t%s\n", GetTimestamp(tmstmp), pids, command);
    fclose(log_file);
}

void execute_commands(char *commands[], int numCommands){
    int pids[numCommands];
    int pfds[numCommands][2];
    int status;

    for(int i = 0; i < numCommands; i++){
        if (pipe(pfds[i]) == -1){
            perror("pipe");
            exit(1);
        }
    }


    for (int i = 0; i < numCommands; i++){
        pids[i] = fork();
        if (pids[i] == -1){
            perror("fork");
            exit(1);
        }
        else if (pids[i] == 0){
            if(numCommands > 1 && i == 0){
                if(pfds[0][1] != STDOUT_FILENO){
                    if(dup2(pfds[0][1], STDOUT_FILENO) == -1){
                        perror("dup2 0");
                        exit(1);
                    }
                    if(close(pfds[0][1]) == -1){
                        perror("close 0");
                        exit(1);
                    }
                }
            }
            else if(numCommands > 1 && i == numCommands - 1){
                if(pfds[i-1][0] != STDIN_FILENO){
                    if(dup2(pfds[i-1][0], STDIN_FILENO) == -1){
                        perror("dup2 last");
                        exit(1);
                    }
                    if(close(pfds[i-1][0]) == -1){
                        perror("close last");
                        exit(1);
                    }
                }
            }
            else if(numCommands > 1){
                if(pfds[i-1][0] != STDIN_FILENO){
                    if(dup2(pfds[i-1][0], STDIN_FILENO) == -1){
                        perror("dup2 in mid");
                        exit(1);
                    }
                    if(close(pfds[i-1][0]) == -1){
                        perror("close in mid");
                        exit(1);
                    }
                }
                if(pfds[i][1] != STDOUT_FILENO){
                    if(dup2(pfds[i][1], STDOUT_FILENO) == -1){
                        perror("dup2 out mid");
                        exit(1);
                    }
                    if(close(pfds[i][1]) == -1){
                        perror("close out mid");
                        exit(1);
                    }
                }
            }
            if (execl("/bin/sh", "sh", "-c", commands[i],  (char *) NULL) == -1){
                perror("execl");
                exit(1);
            }
            exit(0);
        }
        else
        {
            close(pfds[i][1]); 
        }
    }

    for (int i = 0; i < numCommands; i++){
        if (waitpid(pids[i], &status, 0) == -1){
            perror("waitpid");
        }
        logChilds(pids[i], commands[i]);
    }
}

int main(int argc, char *argv[]){
    if (argc != 1){
        printf("Usage: ./terminal_emulator\n");
        return 1;
    }

    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handleSignal;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa, NULL);


    FILE *log_file = fopen(LOGFILENAME, "w");
    fprintf(log_file, "Time\t\t\t\t\t\tPID\t\tCommand\n--------------------------------------------\n");
    fclose(log_file);



    char buffer[MAXCOMMANDS * COMMANDLENGTH + 1];
    while (1){
        printf("~$ ");
        if (fgets(buffer, sizeof(buffer), stdin) == NULL){
            perror("fgets");
            return 1;
        }
        if (strcmp(buffer, ":q\n") == 0){
            printf("Exiting.\n");
            return 0;
        }
        int numCommands = 0;
        char *commands[MAXCOMMANDS];
        commands[0] = &buffer[0];
        for (int i = 0; buffer[i] != '\0'; i++){
            if (buffer[i] == '|'){
                numCommands++;
                buffer[i] = '\0';
                i++;
                
                while(buffer[i] == ' '){
                    i++;
                }
                if(numCommands + 1 >= MAXCOMMANDS){
                    break;
                }
                commands[numCommands] = &buffer[i];
            }
        }
        numCommands++;
        if (numCommands > MAXCOMMANDS){
            printf("Too many commands. Maximum is %d.\n", MAXCOMMANDS);
            continue;
        }

        execute_commands(commands, numCommands);
    }
}