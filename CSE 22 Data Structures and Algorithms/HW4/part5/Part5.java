
import java.util.Scanner;

public class Part5{
    public static void main(String[] args) {
        int arrSize;
        Scanner buf = new Scanner(System.in);
        System.out.println("HW 4 - Part 5 Program");
        System.out.print("Enter length of array: ");
        arrSize = buf.nextInt();
        char[] arr =  new char[arrSize];
        arr = fillArr(arr, 0, arrSize);
        printArr(arr, 0, arrSize);
        System.out.println();
        part5(arr, arrSize);
        buf.close();
    }


    /**
     * This is wrapper function for recursive functions chain.
     * @param arr Array
     * @param arrSize Array size Size of array
     */
    public static void part5(char[] arr, int arrSize){
        blockLenControl(arr, arrSize, 3);
    }

    /**
     * This function controls the length of the blocks to be inserted. It is the first function of recursive function chain.
     * @param arr Array
     * @param arrSize Array size Size of array
     * @param blockLen Length of block to be inserted Length of the current block to be inserted. Its current value is 3.
     */
    public static void blockLenControl(char[] arr, int arrSize, int blockLen){
        //Base case: If block length is bigger than array size, return.
        if(blockLen > arrSize){
            return;
        }
        blockNumControl(arr, arrSize, blockLen, 1);  // Call next function
        fillArr(arr, 0, arrSize);                       // Clear array.
        blockLenControl(arr, arrSize, blockLen + 1);    // Increment block lenght and call again.
    }


    /**
     * This function controls the number of blocks at specified length to be inserted. It is the second function of recursive
     * function chain. 
     * @param arr Array
     * @param arrSize Array size
     * @param blockLen Length of block to be inserted 
     * @param blockNum Number of blocks to be inserted. Its initial value is 
     */
    public static void blockNumControl(char[] arr, int arrSize, int blockLen, int blockNum){
        //Base case, if current number of blocks can't fit in array, return.
        if(blockNum*blockLen > arrSize){
            return;
        }
        fillArr(arr, 0, arrSize);
        startFromIndex(arr, arrSize, blockLen, blockNum, 0); //Call next function
        blockNumControl(arr, arrSize, blockLen, blockNum + 1); //Increment number of blocks and call again.
    }

    /**
     * This function controls the starting index of the insertion to array. 
     * For example: -***-***- || --***-*** (Insertion is started in index 1 in first one, index 2 in second one.)
     * @param arr Array
     * @param arrSize Array size
     * @param blockLen Length of block to be inserted
     * @param blockNum Number of blocks to be inserted.
     * @param startIndex Starting index of the insertion. Its initial value is 0.
     */
    public static void startFromIndex(char[] arr, int arrSize, int blockLen, int blockNum, int startIndex){
        //Base case, if no block can fit starting from this index, return 1.
        if(startIndex + blockLen > arrSize){
            return;
        }
        fillArr(arr, 0, arrSize);
        currentBlockControl(arr, arrSize, blockLen, blockNum, 0, startIndex, startIndex);   //Call next function.
        //If blockNum is 1, printing is already done in next functions. return.
        if(blockNum == 1){
            return;
        }
        startFromIndex(arr, arrSize, blockLen, blockNum, startIndex + 1);   //Increment start index and call again.
    }


    /**
     * This function inserts required amount of blocks 
     * @param arr Array
     * @param arrSize Array size
     * @param blockLen Length of block to be inserted
     * @param blockNum Number of blocks to be inserted.
     * @param curBlock Current block that is being inserted. Its initial value is 0.
     * @param startIndex Starting index of insertion.
     * @param callIndex index that will be used to call next functions
     */
    public static void currentBlockControl(char[] arr, int arrSize, int blockLen, int blockNum, int curBlock, int startIndex, int callIndex){
        //Base case, if all blocks are inserted, return.
        if(curBlock >= blockNum){
            return;
        }
        if(curBlock >= 1){
            int last = findLastBlock(arr, arrSize, arrSize-1, blockLen, 0);
            fillArr(arr, last, arrSize);
            callIndex = startIndex + blockLen + curBlock + 1;
        }
        insertInIndex(arr, arrSize, blockLen, blockNum, curBlock, callIndex);     //Call next function
        currentBlockControl(arr, arrSize, blockLen, blockNum, curBlock + 1, startIndex, callIndex);    //Increment current block and call again.
    }


    /**
     * This function tries inserting a block at the all next indexes possible while controlling current block value.
     * @param arr Array
     * @param arrSize Array size
     * @param blockLen Length of block to be inserted
     * @param blockNum Number of blocks to be inserted.
     * @param curBlock Current block that is being inserted. Its initial value is 0.
     * @param index Index of the block to be inserted. Its initial value comes from startIndex function.
     */
    public static void insertInIndex(char[] arr, int arrSize, int blockLen,int blockNum, int curBlock, int index){
        //Base case is when block doesn't fit in array.
        if(index + blockLen > arrSize){
            return;
        }
        //If inserting in this position is successful, increment current block.
        if(insertBlock(arr, arrSize, blockLen, blockNum, index)){  
            curBlock++;
        }
        //If current block is equal to blockNum, delete the last inserted block to make room for next insertions.
        if(curBlock >= blockNum){
            index = findLastBlock(arr, arrSize, arrSize-1, blockLen, 1);    
            arr = fillArr(arr, index, arrSize); 
            if(index == arrSize - blockLen){
                return;     //If block is at the end of the array, return directly.
            }
        }
        insertInIndex(arr, arrSize, blockLen,blockNum,curBlock, index + 1); //Call again by incrementing index.
    }


    /**
     * This function inserts a block at next possible place.
     * @param arr Array
     * @param arrSize Array size
     * @param blockLen Length of block to be inserted
     * @param blockNum Number of blocks to be inserted.
     * @param index
     * @return
     */
    public static boolean insertBlock(char[] arr, int arrSize, int blockLen, int blockNum, int index){
        //Base case: when block doesn't fit in array. return false.
        if(index + blockLen > arrSize){
            return false;
        }

        //If there is available space in this position, insert the block here and print it if it is ready, and return true;
        //Difference between next two block is checking if index is 0, to prevent index out of bounds.
        if(index == 0 && checkSpace(arr, arrSize, blockLen, index, 1)){
            paintArr(arr, index, index + blockLen - 1);
            if(checkIfPrintable(arr, arrSize, 0, blockLen, blockNum, 0)){
                printArr(arr, 0, arrSize);
                System.out.println();
            }
            return true;
        }
        else if(index != 0 && arr[index - 1] == '-' && checkSpace(arr, arrSize, blockLen, index, 1)){
            paintArr(arr, index, index + blockLen - 1);
            if(checkIfPrintable(arr, arrSize, 0, blockLen, blockNum, 0)){
                printArr(arr, 0, arrSize);
                System.out.println();
            }
            return true;
        }
        return insertBlock(arr, arrSize, blockLen, blockNum, index + 1);    //Call again by incrementing index.
    }
    

    /**
     * This is helper function. It fills the array with '-' character starting from index.
     * @param arr Array
     * @param index
     * @param len
     * @return
     */
    public static char[] fillArr(char[] arr, int index, int len){
        if(index >= len){
            return arr;
        }
        arr[index] = '-';
        return fillArr(arr, index + 1, len);
    }

    /**
     * This is helper function. It prints the array starting from the index. 
     * @param arr Array
     * @param index
     * @param len
     */
    public static void printArr(char[] arr, int index, int len){
        if(index == len){
            return;
        }
        System.out.print(arr[index]);
        printArr(arr, index + 1, len);
    }

    /**
     * This is helper function, it checks if this position is available for inserting a new block.
     * @param arr Array
     * @param arrSize Array size
     * @param blockLen Length of block to be inserted
     * @param index Position to be checked.
     * @param count Count of encountered '-' characters. 
     * @return true if insertion available, false if not
     */
    public static boolean checkSpace(char[] arr, int arrSize, int blockLen, int index, int count){
        if(count == blockLen){
            if(arr[index] == '-'){
                return true;
            }
            return false;
        }
        return checkSpace(arr, arrSize, blockLen, index+1, count+1);
    }
    
    /**
     * This function checks if array is ready to be printed, in other words, all blocks are placed. 
     * It basically count number of painted cells and compares it with blockNum*blockLen.
     * @param arr Array
     * @param arrSize Array size
     * @param index
     * @param blockLen Length of block to be inserted
     * @param blockNum Number of blocks to be inserted.
     * @param counter Counts number of '*' characters. Its initial value is 0
     * @return
     */
    public static boolean checkIfPrintable(char[] arr, int arrSize, int index, int blockLen, int blockNum, int counter){
        if(index == arrSize){
            if(blockNum*blockLen == counter){
                return true;
            }
            return false;
        }
        if(arr[index] == '*'){
            counter++;
        }
        return checkIfPrintable(arr, arrSize, index+1, blockLen, blockNum, counter);
    }

    /**
     * This function paints between startIndex and endIndex with '*' character.
     * @param arr Array
     * @param index
     * @param endIndex
     */
    public static void paintArr(char[] arr, int startIndex, int endIndex){
        if(startIndex > endIndex){
            return;
        }
        arr[startIndex] = '*';
        paintArr(arr, startIndex + 1, endIndex);
    }


    /**
     * This function finds the starting index of the last block placed. It counts number of '*' characters
     * encountered and returns index when it's equal to blockLen.
     * @param arr Array
     * @param arrSize Array size
     * @param index Index of array, its initial value is array size - 1
     * @param blockLen Length of block to be inserted
     * @return
     */
    public static int findLastBlock(char[] arr, int arrSize, int index, int blockLen, int counter){
        if(index < 0){
            return 0;
        }
        if(counter == blockLen){
            return index;
        }
        if(arr[index] == '*'){
            counter++;
        }
        return findLastBlock(arr, arrSize, index - 1, blockLen, counter);
    }
}