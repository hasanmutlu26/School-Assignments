#include<stdio.h>

// This C code is a pseudocode that only finds one array's longest increasing subsequence. 
// In assembly, this whole code is executed in a loop such as for(int i = 0; i < numOfArrays, i++)

// Explanation of what weight is is given in the report file

#define N 10    // Size of the array

int array[] = {3,10,7,9,4,11,8,6,12,13};   // The array to find the longest increasing subsequence from
int weight[N];      // weight of array elements, saved according to their indexes
int longest[N];     // array to assign longest increasing subsequence into
int lenght = 1;     // lenght of longest increasing subsequence at any time. its final value will be the size
int endIndex = 0;   // index of the greatest element in longest increasing subsequence


void main(){
    weight[0] = 1;      // weight of first element in the array can only be 1 as there isn't any elements coming before it

    // A loop to cover entire array and find their weights. it starts from 1 because first element's weight is already assigned to 1
    // In this loop,    current element: array[i]
    //                  previous elements: array[j]
    //                  current element's weight: weight[i]
    //                  previous elements weights: weight[j]
    for (int i = 1; i < N; i++){
        // array[i]'s weight is assigned to be 1. It will be incremented if there are smaller array[j] values coming before it
        weight[i] = 1;

        // A loop to cover previous elements of the array, starting from one previous element and ending at the first element
        for (int j = i - 1; j >= 0; j--){

            // If an array[i] element is bigger than any previous element (array[j]), 
            // and if that previous element's weight is greater than or equal to current element, 
            // current element's weight must be 1 bigger, so it is assigned to be.
            if (array[j] < array[i] && weight[j] >= weight[i]){
                weight[i] = weight[j] + 1;
            }
        }

        // If weight of current element is bigger than current lenght, its weight is assigned to the lenght
        // Current element is the greatest element of longest subsequence we found so far, so its index is assigned to endIndex
        if (weight[i] > lenght){
            lenght = weight[i];
            endIndex = i;
        }
    }

    // This loop prints the weight array. It is not included in the assembly code
    for(int i = 0; i < N; i++){
        printf("%d ", weight[i]);
    } 

    // In next loop, the elements of the longest increasing subsequence are assigned into longest[] array in decreasing order
    // k is for indexing longest[] array and also to calculate weight of the next element to assign into the longest[] array
    int k = 0;

    // Loop starts from endIndex, which is the index of the greatest element of subsequence, and goes backwards
    for(int i = endIndex; i >= 0 ; i--){
        // If weight of current element is equal to lenght - k, it means that it is the next element to be assigned into longest[] array
        if(weight[i] == lenght - k){
            longest[k] = array[i];
            k++;        // k is incremented to be ready for assigning next element
        }
    }
    printf("\n\n");

    
    k--;    // k is decremented by one and now contains the index of last element in the longest[] array, 
            // which is also the smallest value in the subsequence    
    
    // Starting from end, longest[] array is printed in reverse order, which is correct order of the subsequence.
    for(int i = k; i >= 0; i--){
        printf("%d ", longest[i]);
    } 

    // Size is printed
    printf("- Size: %d", lenght);

    // printf("\n%d\n", endIndex);
}
