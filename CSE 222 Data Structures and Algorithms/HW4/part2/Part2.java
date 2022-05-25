public class Part2{

    /**
     * Driver function.
     * @param args
     */
    public static void main(String[] args) {
        System.out.println("Homework 4 Part 2 Driver Function");
        System.out.println();
        
        int[] arr1 = new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        System.out.println("Array 1: {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}");
        System.out.println("Size: 10");
        System.out.println("Number of elements between -1 and 15:   " + part2Wrapper(arr1, 10, -1, 15));
        System.out.println("Number of elements between 2 and 9:     " + part2Wrapper(arr1, 10, 2, 9));
        System.out.println("Number of elements between 3 and 8:     " + part2Wrapper(arr1, 10, 3, 8));
        System.out.println("Number of elements between 4 and 5:     " + part2Wrapper(arr1, 10, 4, 5));
        System.out.println();

        int[] arr2 = new int[]{1, 3, 6, 8, 11, 13, 17, 20, 25, 28, 35, 40, 42, 51, 55};
        System.out.println("Array 2: {1, 3, 6, 8, 11, 13, 17, 20, 25, 28, 35, 40, 42, 51, 55}");
        System.out.println("Size: 15");
        System.out.println("Number of elements between 0 and 70:    " + part2Wrapper(arr2, 15, 0, 70));
        System.out.println("Number of elements between 0 and 10:    " + part2Wrapper(arr2, 15, 0, 10));
        System.out.println("Number of elements between 2 and 15:    " + part2Wrapper(arr2, 15, 2, 15));
        System.out.println("Number of elements between 10 and 50:   " + part2Wrapper(arr2, 15, 10, 50));
        System.out.println("Number of elements between 30 and 40:   " + part2Wrapper(arr2, 12, 30, 40));
        System.out.println("Number of elements between 21 and 24:   " + part2Wrapper(arr2, 12, 21, 24));
    }

    /**
     * This function is wrapper function for part2. In this function, appropriate indexes of num1 and num2 are found by
     * calling recursive function and result is found by subtraction. 
     * @param arr Sorted integer array.
     * @param size Size of the array.
     * @param num1 Smaller integer number.
     * @param num2 Larger integer number.
     * @return Number of elements between num1 and num2 in the array. 
     */
    public static int part2Wrapper(int[] arr, int size, int num1, int num2){
        int lower = 0;
        int upper = size;
        int num2Return = part2(arr, size, num2, upper, lower, 1);
        int num1Return = part2(arr, size, num1, upper, lower, 0);
        //System.out.println("Num2 Return: " + num2Return + "\nNum1 Return: " + num1Return);
        return num2Return - num1Return - 1;     //-1 is to disclude extra counted element.
    }

    /**
     * This is main recursive function for part2. In this function, appropriate index of a number in array is found and returned.
     * It does binary search.
     * What is appropriate index? Suppose the array is {1,3,5,7}
     * For num == 3, appropriate index is index of 3 always, as it exist in array.
     * For num == 4, 
     *      if boundtype is 0, appropriate index is index of 5, which is 2
     *      if boundtype is 1, appropriate index is index of 3, which is 1
     * @param arr Integer array
     * @param size Size of the array.
     * @param num Number to search in array.
     * @param upper Current upper possible index.
     * @param lower Current lower possible index. These two's average will be the current index.
     * @param boundType If the number is the smaller number in calling function, it is 0, which is lower bound. Else, it is 1, which is upper bound.
     * @return Appropriate index of num in the array.
     */
    public static int part2(int[] arr, int size, int num, int upper, int lower, int boundType){
        int index = (upper + lower)/2;  //Current index is found.
        
        //Base case: If index is 0.
        if(index == 0){
            if(num >= arr[index]){      //If num >= currentElement, 0 is returned (first element's index)
                return 0;
            }
            return -1;      //Else, -1 is returned to include first element too.
        }

        //Base case: Current element is last element.
        if(index == size - 1){
            if(num > arr[index]){       //If num > currentElement, size is returned to include all elements. 
                return size;
            }
            return index;       //Else, index of last element is returned.
        }

        //Base case: If num is equal to current element, index is returned directly.
        if(num == arr[index]){
            return index;
        }

        //Base case: If num is between current and previous element, index or index-1 is returned
        //depending on the boundType
        if(num < arr[index] && arr[index - 1] < num){
            if(boundType == 1){
                return index;
            }
            return index - 1;
        }

        //Base case: If num is between current and next element, index or index+1 is returned
        //depending on the boundType
        if(num > arr[index] && arr[index + 1] > num){
            if(boundType == 1){
                return index + 1;
            }
            return index;
        }

        //Else, upper and lower are modified according to num's position and
        //interval is narrowed by half. Function is called again.
        if(num < arr[index]){
            upper = index;
        }
        else if(num > arr[index]){
            lower = index;
        }
        return part2(arr, size, num, upper, lower, boundType);
    }

}

