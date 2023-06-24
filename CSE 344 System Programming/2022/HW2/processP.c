#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#define MATRIXNUM 200   //Maximum number of matrices in file;

struct flock lock;
mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

char* inputFilePath;
char* outputFilePath;

int child_pid_table[MATRIXNUM] = {0};   //table of child process pid's
char* env[3];           //Command line arguments array for process R
int count = 1;          //Number of matrices (and child processes)
double matrices[MATRIXNUM][3][3];    //An array to store matrices in.
char lineBuf[MATRIXNUM][200] = {'\0'};   //A buffer to read output file line by line

double frobenius[MATRIXNUM] = {0};      //Table of matrices frobenius norms



int fdin;
int fdout;
char buffer[30];

int spawn(char** envVars);
void signal_handler(int signum);
int readInput();
int readOutput();
void assignMatrices();
void printMatricesInOrder();
void findFrobenius();

int main(int argc, char* argv[]){
    int valid;

    /**Input validity checks.**/
    if(argc != 5){
        printf("Usage: ./processP -i inputFilePath -o outputFilePath\n");
        return 1;
    }
    for(valid = 1; valid < 5; valid++){
        if(strcmp(argv[valid], "-i") == 0){
            inputFilePath = argv[valid + 1];
            break;
        }
    }
    if(valid == 5){
        printf("Usage: ./processP -i inputFilePath -o outputFilePath\n");
        return 1;
    }
    for(valid = 1; valid < 5; valid++){
        if(strcmp(argv[valid], "-o") == 0){
            outputFilePath = argv[valid + 1];
            break;
        }
    }
    if(valid == 5){
        printf("Usage: ./processP -i inputFilePath -o outputFilePath\n");
        return 1;
    }

    //Prepare signal handler
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = signal_handler;
    sigaction(SIGINT, &sa, NULL);


    //Open output file with truncate flag to erase its content if it already exists
    fdout = open(outputFilePath, O_TRUNC | O_WRONLY | O_CREAT, mode);
    if(fdout < 0){
        perror("open");
        return 1;
    }
    close(fdout);

    //Read input file. 
    if(readInput()){
        printf("Error while reading.\n");
        return 1;
    }
    
    //Wait for all child processes to finish their jobs
    for(int i = 0; i < count; i++){
        int cpid = wait(NULL);
        if(cpid == -1){
            perror("wait");
            return 1;
        }
    }

    //Read output file
    if(readOutput()){
        printf("Error while reading output file.\n");
        return 1;
    }

    //Assign matrices to arrays.
    assignMatrices();

    //Print matrices in order of their processes numbers.
    printMatricesInOrder();

    //Find and print closest matrices.
    findFrobenius();
    
}

/**
 * This function spawns a child process.
 * @param envVars Array of command line arguments to be sent to process R_i
 * @return 0 if successful, 1 if an error has occured.
 */
int spawn(char** envVars){
    pid_t child_pid;
    child_pid = fork();
    if(child_pid < 0){
        perror("fork");
        return 1;
    }
    if(child_pid != 0){
        child_pid_table[count - 1] = child_pid;     //Save child pid to table.
        return child_pid;
    }
    else{
        execvp("./processR", envVars);
        printf("Error.");
        return 1;
    }
    return 0;
}

/**
 * This function handles the SIGINT signal.
 * @param signum signal id
 */
void signal_handler(int signum){
    if(signum == SIGINT){
        printf("\nSIGINT signal is catched.\nProgram is terminating.\n");

        //Send SIGINT signal to all child processes and wait for their termination.
        for(int i = 0; i < count; i++){
            kill(child_pid_table[i], SIGINT);
            int cpid = wait(child_pid_table + i);
            if(cpid == -1){
                perror("wait");
            }
        }

        //Close files and remove output file and exit.
        close(fdin);
        close(fdout);
        remove(outputFilePath);
        _exit(1);
    }
}

/**
 * This function reads input from input file.
 * @return 0 if successful, 1 if an error has occured.
 */
int readInput(){
    int bytes_read;
    fdin = open(inputFilePath, O_RDONLY, mode);
    if(fdin == -1){
        perror("open");
        return 1;
    }

    //Lock the input file.
    lock.l_type = F_WRLCK;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_whence = SEEK_CUR;
    fcntl(fdin, F_SETLKW, &lock);
    
    printf("Process P reading %s\n", inputFilePath);

    //Read 30 bytes and spawn a child process.
    do{
        memset(buffer, '\0', 30);
        bytes_read = read(fdin, buffer, 30);
        if(bytes_read == -1){
            perror("read");
            return 1;
        }
        if(bytes_read < 30){
            break;
        }

        /*Save output file path, input buffer and number(i) of process to be created into env[]
        It will be the command line arguments of process R_i */
        env[0] = outputFilePath;
        env[1] = buffer;
        char intBuf[10];
        snprintf(intBuf,10, "%d", count);
        env[2] = intBuf;
        spawn(env);     //Spawn child process.
        count++;
    }while(bytes_read == 30);
    count--;
    lock.l_type = F_UNLCK;      //Unlock file
    fcntl(fdin, F_SETLKW, &lock);

    if(close(fdin) == -1){
        perror("close");
        return 1;
    }
    return 0;
}


/**
 * This function reads the output file and assigns every line to lineBuf array.
 * @return 0 if successful, 1 if an error has occured.
 */
int readOutput(){
    fdin = open(outputFilePath, O_RDONLY, mode);
    if(fdin < 0){
        perror("open");
        return 1;
    }
    lock.l_type = F_WRLCK;
    fcntl(fdin, F_SETLKW, &lock);
    for(int i = 0; i < count; i++){
        memset(lineBuf[i], '\0', 200);
        memset(buffer, '\0', 30);
        while(1){
            if(read(fdin, buffer, 1) == -1){
                perror("read");
                return 1;
            }
            strcat(lineBuf[i], buffer);
            if(buffer[0] == '\n'){
                break;
            }
        }
    }
    return 0;
}

/**
 * This function parses the matrices in lineBuf arrays and assigns values to matrices array.
 */
void assignMatrices(){
    int num;
    double x, y, z;
    char* token;
    for(int i = 0; i < count; i++){
        token = lineBuf[i];
        sscanf(token, "%d", &num);
        while(*token != ' '){
            token++;
        }
        for(int j = 0; j < 3; j++){
            sscanf(token, "%lf %lf %lf", &x, &y, &z);
            matrices[num-1][j][0] = x;
            matrices[num-1][j][1] = y;
            matrices[num-1][j][2] = z;
            token++;
            for(int k = 0; k < 3; ){
                token++;
                if(*token == ' '){
                    k++;
                }
            }
        }
    }
}

/**
 * This function prints matrices in order, as "Created R_i with ...."
 */
void printMatricesInOrder(){
    for(int i = 0; i < count; i++){
        printf("Created R_%d with ", i+1);
        printf("(%.3f,%.3f,%.3f),", matrices[i][0][0], matrices[i][0][1], matrices[i][0][2]);
        printf("(%.3f,%.3f,%.3f),", matrices[i][1][0], matrices[i][1][1], matrices[i][1][2]);
        printf("(%.3f,%.3f,%.3f)\n", matrices[i][2][0], matrices[i][2][1], matrices[i][2][2]);
    }
    printf("Reached EOF, collecting outputs from %s\n", outputFilePath);
}

/**
 * This function calculates Frobenius norm's of matrices and prints the closest two matrices.
 */
void findFrobenius(){
    int closest1;
    int closest2;
    for(int i = 0; i < count; i++){
        for(int j = 0; j < 3; j++){
            frobenius[i] += matrices[i][j][0]*matrices[i][j][0];
            frobenius[i] += matrices[i][j][1]*matrices[i][j][1];
            frobenius[i] += matrices[i][j][2]*matrices[i][j][2];
        }
        frobenius[i] = sqrt(frobenius[i]);
    }
    closest1 = 0;
    closest2 = 1;
    double closestDistance = frobenius[0] - frobenius[1];
    if(closestDistance < 0){
        closestDistance = closestDistance * (-1.0);
    }

    for(int i = 0; i < count; i++){
        for(int j = 0; j < count; j++){
            if(i == j){
                continue;
            }
            double distance = frobenius[i] - frobenius[j];
            if(distance < 0){
                distance = distance * (-1.0);
            }
            if(distance < closestDistance){
                closest1 = i;
                closest2 = j;
                closestDistance = distance;
            }
        }
    }
    printf("The closest 2 matrices are R_%d and R_%d, and their distance is %f.\n", closest1+1, closest2+1, closestDistance);
}


