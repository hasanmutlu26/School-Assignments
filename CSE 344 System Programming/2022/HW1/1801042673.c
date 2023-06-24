#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define STRSIZE 50  // Maximum size of a string
#define MULTICHARSIZE 10    //Maximum number of different characters in multiple character matching

int inputFormatCheck(const char* argv1);
void parse(char* text, char* str1, char* str2);
int replace(char* filepath, char* str1);
int dCheck();
int gCheck(char* str1);


char buffer[STRSIZE] = {'\0'};  //Buffer to read from file
char str1[STRSIZE] = {'\0'};    //String to be replaced
char str2[STRSIZE] = {'\0'};    //String that will replace str1
char tempStr1[STRSIZE] = {'\0'};    //To save str1 when there is combination of multiple character matching and repetition of characters

char dContainer[MULTICHARSIZE] = {'\0'};    //To store different characters in multiple character matching.
int str1IndG;   //Index of character to be repeated in character repetition
int str1IndD;   //Index of character in multiple character matching
int contInd = 0;    //Index for dContainer to choose different characters in multiple character matching
char gChar;     //The character that is repeated

int caseSensitive;  //Boolean like value to check if operation is case sensitive
unsigned int bytes_read;    //Bytes read in read();
char* token;    //A pointer to advance through multiple replacement operations


int main(int argc, char* argv[]){
    token = argv[1];

    //Check if format is correct
    if(argc != 3 || inputFormatCheck(argv[1]) == 0){
        printf("Possible Usages:\n$ %s '/str1/str2/' inputFilePath\n", argv[0]);
        printf("$ %s '/str1/str2/i' inputFilePath\n", argv[0]);
        printf("$ %s '/str1/str2/i;str3/str4/' inputFilePath\n", argv[0]);
        printf("$ %s '/[zs]tr1/str2/i' inputFilePath\n", argv[0]);
        printf("$ %s '/^str1/str2/i' inputFilePath\n", argv[0]);
        printf("$ %s '/str1$/str2/i' inputFilePath\n", argv[0]);
        printf("$ %s '/st*r1/str2/i' inputFilePath\n", argv[0]);
        printf("And arbitrary combinations of the above.\n");
        return 1;
    }
    printf("Input format is correct.\n");

    //The main loop.
    while(token != NULL){
        printf("Replacement is being done.\n");
        int dCheckVal = 0;
        int gCheckVal = 0;
        parse(token, str1, str2);   //Parse the input
        dCheckVal = dCheck();       //Check if there is multiple character matching
        if(dCheckVal == -1){
            return 1;
        }

        //If there is multiple character matching, check for all different characters saved in dContainer
        if(dCheckVal == 1){
            do{
                str1[str1IndD] = dContainer[contInd];
                for(int i = 0; i < strlen(str1); i++){
                    tempStr1[i] = str1[i];
                    
                }
                //If there is also repetition of character, check from 0 to STRSIZE number of repeated character.
                if(gCheckVal = gCheck(tempStr1)){   
                    do{
                        int i;
                        if(replace(argv[2], tempStr1)){
                            printf("Program aborted.\n");
                            return 1;
                        }
                        for(i = strlen(tempStr1); i > str1IndG && i < STRSIZE; i--){
                            tempStr1[i] = tempStr1[i-1];
                        }
                        tempStr1[i] = gChar;
                    }while(strlen(tempStr1) != STRSIZE);
                }
                else if(replace(argv[2], str1)){
                    printf("Program aborted.\n");
                    return 1;
                }
                contInd++;
                memset(tempStr1, '\0', STRSIZE);
            }while(contInd != strlen(dContainer));

            dCheckVal = 0;
            gCheckVal = 0;
        }

        else{
            //If there is only repetition of characters
            gCheckVal = gCheck(str1); 
            if(gCheckVal == 1){
                do{
                    int i;
                    if(replace(argv[2], str1)){
                        printf("Program aborted.\n");
                        return 1;
                    }
                    for(i = strlen(str1); i > str1IndG && i < STRSIZE; i--){
                        str1[i] = str1[i-1];
                    }
                    str1[i] = gChar;
                }while(strlen(str1) != STRSIZE);
            }
            //Else, replace normally.
            else{
                if(replace(argv[2], str1)){
                    printf("Program aborted.\n");
                    return 1;
                }
            }
            
        }
        //Reset all values for next operation.
        memset(tempStr1, '\0', STRSIZE);
        memset(str1, '\0', STRSIZE);
        memset(str2, '\0', STRSIZE);
        memset(dContainer, '\0', MULTICHARSIZE);
        caseSensitive = 1;
        contInd = 0;
    }
    printf("Replacement is finished successfully.\n");
    return 0;
}


/**
 * This function checks if the input format is valid.
 * @param argv1 Takes a pointer to argv[1]
 * @return 1 if valid, 0 if invalid.
 */
int inputFormatCheck(const char* argv1){
    int count = 0;
    if(argv1[0] != '/' || argv1[1] == '/'){
        return 0;
    }
    if(argv1[strlen(argv1)-1] == 'i'){
        if(argv1[strlen(argv1)-2] != '/'){
            return 0;
        }
    }
    for(int i = 0; i < strlen(argv1); i++){
        if(argv1[i] == '/')
            count++;
    }
    if(count % 3 != 0){
        return 0;
    }
    return 1;
}

/**
 * This function parses each replacement operation command. It parses the input
 * to str1 and str2. It also checks if there is line start or line end matching
 * adds necessary newline characters to str1 
 * @param text Pointer to start of the command.
 * @param str1 Pointer to str1
 * @param str2 Pointer to str2
 */
void parse(char* text, char* str1, char* str2){
    int i = 1;
    caseSensitive = 1;
    for(int j = 0; text[i] != '/'; j++){
        str1[j] = text[i];
        i++;
    }
    i++;
    for(int j = 0; text[i] != '/'; j++){
        str2[j] = text[i];
        i++;
    }

    //If there is line end matching, insert newline character at the end of str1
    if(str1[strlen(str1) - 1] == '$'){
        str1[strlen(str1) - 1] = '\n';
        str2[strlen(str2)] = '\n';
    }

    //If there is line start matching, insert newline character at the beginning of str1
    else if(str1[0] == '^'){
        str1[0] = '\n';
        for(int j = strlen(str2) - 1; j >= 0; j--){
            str2[j+1] = str2[j];
        }
        str2[0] = '\n';
    }

    //If there is 'i' at the end, set caseSensitive to 0.
    if(text[++i] == 'i'){
        caseSensitive = 0;
    }

    //If there is ';' at the end, set token to next operation.
    if(token = strchr(token, ';')){
        token++;
    }
    
}

/**
 * This function checks if there is multiple character matching.
 * @return 1 if there is multi character matching, 0 if there isn't, -1 if closing bracket (']') not found.
 */
int dCheck(){
    int i;
    char* iter;
    if(iter = strchr(str1, '[')){
        for( ; *iter != ']' && *iter != '\0'; iter++);

        if(*iter == '\0'){
            printf("Closing bracket (']') not found.\n");
            return -1;
        }

        iter = strchr(str1, '[');
        iter++;
        for(i = 0; *iter != ']'; i++){
            dContainer[i] = *iter;
            iter++;
        }

        iter = strchr(str1, ']');
        
        for(i = 0; str1[i] != '['; i++);
        str1IndD = i;
        str1[str1IndD] = dContainer[contInd];

        iter++;
        for(i = str1IndD + 1; *iter != '\0'; i++){
            str1[i] = *iter;
            iter++;
        }
        str1[i] = '\0';
        return 1;
    }
    else{
        return 0;
    }
}

/**
 * This function checks if there is multiple repetition of a character
 * @param str1 Pointer to str1
 * @return 1 if there is, 0 if there isn't.
 */
int gCheck(char* str1){
    int i;
    char* iter;

    if(iter = strchr(str1, '*')){
        gChar = *(iter-1);
        for(i = 0; i < strlen(str1); i++){
            if(str1[i] == '*'){
                break;
            }
        }
        str1IndG = i;
        for( ; i < strlen(str1); i++){
            str1[i] = str1[i+1];
        }
        for(i = str1IndG - 1; i < strlen(str1); i++){
            str1[i] = str1[i+1];
        }
        str1IndG--;
        return 1;
    }
    else{
        return 0;
    }
}


/**
 * This function replaces str1 with str2
 * @param filepath Pointer to file path
 * @param str1 Pointer to str1
 * @return 1 if there is an error, 0 if there is no issues and replacement is completed.
 */
int replace(char* filepath, char* str1){
    //Open modes for file.
    mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;

    //File lock struct is created and set to write lock. Other values are set to default values. 
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_len = 0;
    lock.l_start = 0;
    lock.l_whence = SEEK_CUR;

    //Input file is opened in read only mode.
    int fd = open(filepath, O_RDONLY, mode);
    if(fd == -1){
        perror("open");
        return 1;
    }
    //Input file is locked.
    fcntl(fd, F_SETLKW, &lock);

    //Output file is created and opened in write only mode. 
    //Once the replacement is finished, input file will be deleted and replaced with output file.
    int fdout = open("out.txt", O_WRONLY | O_CREAT, mode);
    if(fdout == -1){
        perror("open");
        return 1;
    }
    //Output file is locked.
    fcntl(fdout, F_SETLKW, &lock);

    //In this loop, contents in input file are read. If str1 is found, it is written to output file. 
    //Else, whatever found is written to output file. 
    do{
        memset(buffer, '\0', STRSIZE);
        //Read strlen(str1) bytes from file.
        bytes_read = read(fd, buffer, strlen(str1));
        if(bytes_read == -1){
            perror("read");
            return 1;
        }
        if(bytes_read == 0){    //If bytes read is 0, it is EOF.
            break;
        }
        //Check if read string is equal to str1, according to case sensitivity.
        int equal;
        if(caseSensitive){
            equal = (strcmp(str1, buffer) == 0);
        }
        else{
            equal = (strcasecmp(str1, buffer) == 0);
        }
        //If equal, write strlen(str2) bytes of string to output file. 
        if(equal){
            if(write(fdout, str2, strlen(str2)) == -1){
                perror("write");
                return 1;
            }
        }
        //Else, write first character in buffer and seek backwards as much as (-strlen(buffer) + 1)
        //to read next strlen(str1) lenght of string. 
        else{
            if(write(fdout, buffer, 1) == -1){
                perror("write");
                return 1;
            }
            lseek(fd, -strlen(buffer) + 1, SEEK_CUR);
        }
    }while(bytes_read == strlen(str1));

    //If buffer is not empty, write the remaining string in buffer to file.
    if(strlen(buffer) > 1){
        if(write(fdout, buffer+1, strlen(buffer) - 1) == -1){
            perror("write");
            return 1;
        }
    }

    //Unlock the files.
    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);
    fcntl(fdout, F_SETLKW, &lock);

    //Close the files.
    if(close(fd) == -1){
        perror("close");
        return 1;
    }
    if(close(fdout) == -1){
        perror("close");
        return 1;
    }
    
    //Remove input file and replace it with the output file.
    remove(filepath);
    rename("out.txt", filepath);
    return 0;
}

