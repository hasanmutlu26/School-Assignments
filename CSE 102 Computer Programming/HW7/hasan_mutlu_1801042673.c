#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define NAMESIZE 10     /*Max size for gate names*/

typedef struct gate{
    char name[NAMESIZE];    /*Gate name.*/
    int type;               /*Gate type. They are defined from 0 to 4.*/
    char input1[NAMESIZE];  /*Name of input1 gate.*/
    char input2[NAMESIZE];  /*Name of input2 gate.*/
    int mem;                /*Memory for FLIPFLOP gate.*/
    int connected;          /*If gate is connected to another gate, this will be 1.
                            If it stays as 0, it means that the output of this gate will be the output of the whole circuit.*/
    int inputvalue;         /*The input value coming into input gate.*/
    struct gate* inp1;      /*Pointer to input1 gate.*/
    struct gate* inp2;      /*Pointer to input2 gate.*/
} gate;

int gateNum(FILE* fp);
void fill(FILE* fp, gate* gatearr);
void connect(gate* gatearr, int size);
void fprintResults(gate* gatearr, FILE* fp, FILE* fp2, int outputgate);
int evaluate(gate* currentGate);

void main(){
    FILE *fp1, *fp2, *fp3;
    gate *gatearr;              /*Gate array.*/
    int size,i;                 /*Size is for gate array's size.*/

    fp1 = fopen("circuit.txt","r+");
    fp2 = fopen("input.txt","r+");
    fp3 = fopen("output.txt","w+");

    size = gateNum(fp1);        /*Number of gates are assigned into size variable.*/
    fseek(fp1, 0, SEEK_SET);    /*fp1 is set back to beginning of file.*/

    gatearr = (gate*)calloc(size,sizeof(gate));     /*An array for gates is created according to number of gates.*/

    fill(fp1,gatearr);          /*Gates are filled with the information given in circuit.txt.*/

    connect(gatearr,size);         /*Gates are connected at each other.*/

    for(i=0;i<size;i++)         /*After connection of gates, the gate that will produce the output of whole circuit is found.*/
        if(gatearr[i].connected==0)
            break;

    /*Results are printed. Index of the output gate is sent to function.*/
    fprintResults(gatearr,fp2,fp3,i);

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    free(gatearr);
}



/*gateNum function returns the number of gates in the circuit. It takes the file pointer to circuit.txt as parameter.*/
int gateNum(FILE* fp){
    char buffer[NAMESIZE];
    int count = 0;

    fscanf(fp, "%s", buffer);   /*The junk INPUT word is read.*/

    /*The input gates are counted until encountering another gate name. That one new type of gate is also counted in this block.*/
    while(  strcmp(buffer, "AND") != 0 &&
            strcmp(buffer, "OR") != 0 &&
            strcmp(buffer, "NOT") != 0 &&
            strcmp(buffer, "FLIPFLOP") != 0   )
    {
        fscanf(fp,"%s", buffer);
        count++;
    }

    /*After that, words are read until EOF and the counter is incremented when encaountered a gate type name.*/
    while(fscanf(fp,"%s", buffer) != EOF){
        if( strcmp(buffer, "AND") == 0 ||
            strcmp(buffer, "OR") == 0 ||
            strcmp(buffer, "NOT") == 0 ||
            strcmp(buffer, "FLIPFLOP") == 0   )
            count++;
    }
    /*The counter is returned.*/
    return count;
}



/*fill function fills the gates according to the information in circuit.txt. It takes file pointer and gate array as parameters.*/
void fill(FILE* fp, gate* gatearr){
    char buffer[NAMESIZE];
    int j=0;
    /*Whole text is covered until EOF.*/
    while(fscanf(fp, "%s", buffer) != EOF){
        /*If INPUT word is read, the input gates will be filled.*/
        if(strcmp(buffer, "INPUT") == 0){
            fscanf(fp, "%s", buffer);               /*Name of the first gate is read manually.*/
            /*The input gate names are read until encountering new gate type name.*/
            while(  strcmp(buffer, "AND") != 0 &&
                    strcmp(buffer, "OR") != 0 &&
                    strcmp(buffer, "NOT") != 0 &&
                    strcmp(buffer, "FLIPFLOP") != 0   )
            {
                gatearr[j].type = 0;                /*Type for input gate is 0.*/
                strcpy(gatearr[j].name, buffer);    /*Gate name is assigned.*/
                j++;                                /*Index is incremented.*/
                fscanf(fp, "%s", buffer);           /*Next word is read from file.*/
            }
        }
        if(strcmp(buffer, "AND") == 0){             /*If AND word is read, gate is filled according to format.*/
            gatearr[j].type = 1;                    /*Type for and gate is 1*/
            fscanf(fp, "%s", buffer);               /*Name, input1 name and input2 name are read and assigned.*/
            strcpy(gatearr[j].name, buffer);
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].input1, buffer);
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].input2, buffer);
            j++;
        }
                                                    /*Next blocks works in same way.*/
        else if(strcmp(buffer, "OR") == 0){
            gatearr[j].type = 2;                    /*Type for or gate is 2*/
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].name, buffer);
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].input1, buffer);
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].input2, buffer);
            j++;
        }

        else if(strcmp(buffer, "NOT") == 0){
            gatearr[j].type = 3;                    /*Type for 'not' gate is 3*/
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].name, buffer);
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].input1, buffer);
            j++;
        }

        else if(strcmp(buffer, "FLIPFLOP") == 0){
            gatearr[j].type = 4;                    /*Type for flipflop gate is 4*/
            gatearr[j].mem = 0;                     /*Initial memory of flipflop is 0*/
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].name, buffer);
            fscanf(fp, "%s", buffer);
            strcpy(gatearr[j].input1, buffer);
            j++;
        }
    }
}

/*This function connects the gates at each other. It takes gate array and gate array size as parameters.*/
void connect(gate* gatearr, int size){
    int i, j;

    for(i=0;i<size;i++)             /*Connection status of all gates are set to 0 firstly.*/
        gatearr[i].connected = 0;
    /*Every gate's name's and input names are compared and connected if necessary.*/
    for(i=0;i<size;i++){
        for(j=0;j<size;j++){
            /*If one gate[i]'s input1 name is same with gate[j]'s name, they are connected.*/
            if(strcmp(gatearr[i].input1,gatearr[j].name) == 0){
                gatearr[i].inp1 = &gatearr[j];
                gatearr[j].connected = 1;   /*Connection status of gate[j] is set to 1.*/
            }
            /*Same with input2 name.*/
            if(strcmp(gatearr[i].input2,gatearr[j].name) == 0){
                gatearr[i].inp2 = &gatearr[j];
                gatearr[j].connected = 1;
            }
        }
    }
}

/*This function reads the input values from input.txt, assigns them to input gates, evaluate the result and fprint it into output.txt file.*/
/*It takes gate array, file pointers for input.txt and output.txt, and index for output gate as parameters.*/
void fprintResults(gate* gatearr, FILE* fp, FILE* fp2, int outputgate){
    int input, i=0;
    /*Until reaching the EOF, input values as much as amount of input gates are read and assigned.*/
    while(fscanf(fp, "%d", &input) != EOF){
        gatearr[i].inputvalue = input;
        i++;
        /*If the next gate's type isn't 0, which means next gate isn't input gate,
        assignment of first line of input values are completed and the evaluate function is called.*/
        if(gatearr[i].type != 0){
            fprintf(fp2,"%d\n", evaluate(&gatearr[outputgate]));  /*Output gate's address is sent to the evaluate function.*/
            i=0;    /*i index is set back to zero.*/
        }
    }
}

/*This function evaluates the results recursively. It takes gate pointer as parameter.*/
int evaluate(gate* currentGate){
    /*The base case, if current gate is input gate, it's inputvalue is returned.*/
    if(currentGate -> type == 0){
        return currentGate -> inputvalue;
    }
    /*If current gate is and gate, it's input's are sent to function and results are returned after && operation.*/
    else if(currentGate -> type == 1){
        return evaluate(currentGate -> inp1) && evaluate(currentGate -> inp2);
    }
    /*If current gate is or gate, it's input's are sent to function and results are returned after || operation.*/
    else if(currentGate -> type == 2){
        return evaluate(currentGate -> inp1) || evaluate(currentGate -> inp2);
    }
    /*If current gate is 'not' gate, it's input is sent to function and inverse of the result is returned.*/
    else if(currentGate -> type == 3){
        return !evaluate(currentGate -> inp1);
    }
    /*If current gate is flipflop.*/
    else if(currentGate -> type == 4){
        /*It's input is sent to function. If input is 1, the memory is flipped and the new value of memory is returned.*/
        if(evaluate(currentGate -> inp1) == 1){
            currentGate -> mem = !(currentGate -> mem);
            return currentGate -> mem;
        }
        /*Else, it means that the input is 0 and the current value in memory is returned without flipping.*/
        else{
            return currentGate -> mem;
        }
    }
}
