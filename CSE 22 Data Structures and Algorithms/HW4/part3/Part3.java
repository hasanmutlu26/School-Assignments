public class Part3{
    
    /**
     * Driver function.
     * @param args
     */
    public static void main(String[] args) {
        System.out.println("Homework 4 Part 3 Driver Function");
        System.out.println();

        
        int[] arr2 = new int[]{1, 9, 2, 8, 3, 7, 6, 4, 5, 5};
        System.out.println("Array: {1, 9, 2, 8, 3, 7, 6, 4, 5, 5}");
        System.out.println("Searched sum: 10");
        part3Wrapper(arr2, 10, 10);

        System.out.println();
        int[] arr1 = new int[]{1, 2, 3, 4, 5, 4, 3, 2, 1, 7, 3, 5};
        System.out.println("Array: {1, 2, 3, 4, 5, 4, 3, 2, 1, 7, 3, 5}");
        System.out.println("Searched sum: 15");
        part3Wrapper(arr1, 12, 15);

        System.out.println();
        int[] arr3 = new int[]{11,-1,12,-1,-1};
        System.out.println("Array: {11,-1,12,-1,-1}");
        System.out.println("Searched sum: 10");
        part3Wrapper(arr3, 5, 10);
    }

    
    /**
     * Wrapper function for part3. It calls outer recursive function and prints number of subarrays found.
     * @param arr Integer array.
     * @param size Size of the array.
     * @param sum Searched sum.
     */
    public static void part3Wrapper(int[] arr, int size, int sum){
        int found = part3_1(arr, size, sum, 0);
        if(found == 0){
            System.out.println("No subarray was found.");
            return;
        }
        System.out.println(found + " subarray found.");
    }

    /**
     * This is outer recursive function. In this function, inner recursive function is called starting from every possible index.
     * @param arr Integer array.
     * @param size Size of array.
     * @param sum Searched sum.
     * @param index Current index in the search. Its initial value is 0. 
     * @return Number of subarrays found.
     */
    public static int part3_1(int[] arr, int size, int sum, int index){
        int found = 0;

        //Base case: If index is equal to size, we are finished.
        if(index == size){
            return found;
        }

        //First element is added to currentSum, inner function is called and returnee is assigned to found.
        //Then, this function is called again by incrementing the index.
        int currentSum = arr[index];
        found += part3_2(arr, size, sum, index, index + 1, currentSum, 0);
        return found + part3_1(arr, size, sum, index + 1);

    } 


    /**
     * This is the inner recursive function. In this function, equality of current sum and searched sum is compared
     * and output is printed if necessary.
     * @param arr Integer array.
     * @param size Size of array.
     * @param sum Searched sum.
     * @param startIndex Start index of current subarray.
     * @param index Current index.
     * @param currentSum Current sum of elements in the current subarray.
     * @param found Number of found subarrays so far. Its initial value is 0.
     * @return Number of subarrays found.
     */
    public static int part3_2(int[] arr, int size, int sum, int startIndex, int index, int currentSum, int found){

        //Base case: If index is greater than size, return.
        if(index > size){
            return found;
        }

        //If currentSum is equal to sum, we found a subarray. Current element is not added to current sum yet
        //so index of previous element is needed.
        if (currentSum == sum) {
            int prevIndex = index - 1;
            System.out.print("Sum found in indexes " + startIndex+ " and " + prevIndex + ": {");
            printSubArray(arr, startIndex, prevIndex);
            System.out.println("}");
            found++;
        }

        //Base case: If currentSum is greated than sum or index is equal to size, continue no more. 
        if (index == size){
            return found;
        }

        //Else, currentsum is incremented by current element and function is called again by incrementing index.
        currentSum += arr[index];
        return part3_2(arr, size, sum, startIndex, index+1, currentSum, found);
    }

    //Print subarray
    public static void printSubArray(int[] arr, int index, int endIndex){
        if(index == endIndex){
            System.out.print(arr[index]);
            return;
        }
        System.out.print(arr[index] + ", ");
        printSubArray(arr, index+1, endIndex);
    }
}
