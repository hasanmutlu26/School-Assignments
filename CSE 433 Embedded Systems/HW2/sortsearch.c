#include <stdio.h>



int sortsearch(int arr[], int size, int target) {
    int i, j, temp, go;

    while(!go);

    i = 0;
    while ( i < size - 1) {
        j = 0;
        while(j < size - i - 1) {
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
            j++;
        }
        i++;
    }


    int left = 0;
    int right = size - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            return 1;
        }
        if (arr[mid] < target) {
            left = mid + 1;
        } 
        else {
            right = mid - 1;
        }
    }
    
    return 0;
}

int main() {
    int size, target;
    
    printf("Enter the size of the array: ");
    scanf("%d", &size);
    
    int arr[size];
    
    printf("Enter the elements of the array:\n");
    for (int i = 0; i < size; i++) {
        scanf("%d", &arr[i]);
    }
    
    printf("Enter the number to be searched: ");
    scanf("%d", &target);
    
    
    // Perform binary search
    int result = sortsearch(arr, size, target);
    
    if (result == 1) {
        printf("The number %d is found in the array.\n", target);
    } else {
        printf("The number %d is not found in the array.\n", target);
    }
    
    return 0;
}
