#include<stdio.h>

#define SIZE 512    /*Size of the grid.*/

FILE *fp1, *fp2;    /*Because this is a memory consuming project, I used global variables.*/
int sealevel;       /*Sealevel is the level of sea, first integer in input file.*/
int currentId=1;    /*currentId will be used assigning Id's.*/

/*The struct for cell.*/
typedef struct cell{
    int id;         /*id variable will hold id of the cell or it's state, whether it is checked or it is water.*/
    int level;      /*level will hold terrain level.*/
}cell;

void readInput(cell* arr);
int evaluate(cell* a);

void main(){
    /*The cell array is created with additional edges. The initial values of cell is set to 0. id=0 will indicate that it is water.*/
    /*In this way, the additional edges we created behave like they are water cells.*/
    /*When the recursive evaluate function reaches these edges, base case condition will be provided.*/
    cell arr[SIZE+2][SIZE+2] = {0};
    int i,j;

    fp1 = fopen("input.txt", "r+");
    fp2 = fopen("output.txt", "w+");


    fscanf(fp1,"%d", &sealevel);        /*Sea level value is read.*/

    readInput(&arr[0][0]);              /*the array is sent to read input function.*/


    /*Every member of the grid is sent to evaluate function.*/
    for(i=1;i<=SIZE;i++){
        for(j=1;j<=SIZE;j++){
            if(evaluate(&arr[i][j]))    /*If function returns 1, it means that assigning id's of current island is completed.*/
                currentId++;            /*currentId is incremented.*/
        }
    }

    fprintf(fp2,"%d\n", currentId-1);   /*Number of islands is fprinted.*/

    currentId = 0;                      /*currentId is set to 0 for fprinting coordinates.*/

    /*All cells in the grid is covered.*/
    for(i=1;i<=SIZE;i++)
        for(j=1;j<=SIZE;j++)
            /*If current cell's id is greater than currentId, it means that this is the first coordinate of this island.*/
            if(arr[i][j].id > currentId){
                fprintf(fp2,"%d %d\n", i-1,j-1);    /*Because the first cell in the grid must be (0,0), one less than i and j are printed.*/
                currentId++;                        /*currentId is incremented to search for next island's id.*/
            }
}


/*This function reads the input from file. It takes a pointer for cell array as parameter.*/
void readInput(cell* arr){
    int i,j, buffer;
    /*It reads values from input file as much as SIZE*SIZE.*/
    for(i=1;i<=SIZE;i++){
        for(j=1;j<=SIZE;j++){
            fscanf(fp1,"%d", &buffer);              /*Value is read into a buffer and assigned to current cell.*/
            ((arr+(i*(SIZE+2)))+j)->level = buffer;
            if(buffer > sealevel)                   /*If terrain level is higher than sea level, id of current cell is set to 101.*/
                ((arr+(i*(SIZE+2)))+j)->id = 101;   /*id=101 will indicate that this cell is unchecked when evaluate function comes.*/
        }                                           /*It is stated in the PDF that maximum number of islands is 100. So I chose 101 as unchecked cell id.*/
    }
}


/*This function assigns island id's recursively. It takes a cell pointer as parameter.*/
int evaluate(cell* c){
    /*The base case, if the id of current cell is smaller than or equal to currentId,
    it means that either this cell is water or already checked and assigned before.*/
    if((c->id) <= currentId){
        return 0;
    }
    /*Else, currentId is assigned to current cell's id and evaluate functions are called for all four directions.*/
    else{
        c->id = currentId;
        evaluate(c+1);          /*The cell on the right.*/
        evaluate(c-1);          /*The cell on the left.*/
        evaluate(c+(SIZE+2));   /*By adding as much as the size of array, we reach to the cell below.*/
        evaluate(c-(SIZE+2));   /*By extracting as much as the size of array, we reach to the cell above.*/
        return 1;               /*1 is returned to inform main function that the assigning of current island's id's are completed.*/
    }
}
