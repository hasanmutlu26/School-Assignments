public class Part1{

    /**
     * Driver function.
     * @param args
     */
    public static void main(String[] args) {
        System.out.println("Homework 4 Part 1 Driver Function");
        System.out.println();
        String str1 = new String("hereheretemptemptempheretemphere");
        String search1 = new String("here");
        System.out.println("String: " + str1);
        System.out.println("Searched String: " + search1);
        System.out.println("Index of 0th occurance: " + part1Wrapper(str1, search1, 0));
        System.out.println("Index of 1st occurance: " + part1Wrapper(str1, search1, 1));
        System.out.println("Index of 2nd occurance: " + part1Wrapper(str1, search1, 2));
        System.out.println("Index of 3th occurance: " + part1Wrapper(str1, search1, 3));
        System.out.println("Index of 4th occurance: " + part1Wrapper(str1, search1, 4));
        System.out.println("Index of 5th occurance: " + part1Wrapper(str1, search1, 5));

        System.out.println();
        String str2 = new String("123456789123456789123456789");
        String search2 = new String("2345");
        System.out.println("String: " + str2);
        System.out.println("Searched String: " + search2);
        System.out.println("Index of 1st occurance: " + part1Wrapper(str2, search2, 1));
        System.out.println("Index of 2nd occurance: " + part1Wrapper(str2, search2, 2));
        System.out.println("Index of 3th occurance: " + part1Wrapper(str2, search2, 3));
        System.out.println("Index of 4th occurance: " + part1Wrapper(str2, search2, 4));

        System.out.println();
        String str3 = new String("aaaaaa");
        String search3 = new String("aa");
        System.out.println("String: " + str3);
        System.out.println("Searched String: " + search3);
        System.out.println("Index of 1st occurance: " + part1Wrapper(str3, search3, 1));
        System.out.println("Index of 2nd occurance: " + part1Wrapper(str3, search3, 2));
        System.out.println("Index of 3th occurance: " + part1Wrapper(str3, search3, 3));
        System.out.println("Index of 4th occurance: " + part1Wrapper(str3, search3, 4));
        System.out.println("Index of 5th occurance: " + part1Wrapper(str3, search3, 5));
        System.out.println("Index of 6th occurance: " + part1Wrapper(str3, search3, 6));

    }

    /**
     * This function is a wrapper for main recursive function.
     * @param biggerString The big string to search in.
     * @param givenString The substring to be searched inside bigger string.
     * @param ith Desired ith occurance of the givenString.
     * @return Index of the ith occurance of givenString in biggerString.
     */
    public static int part1Wrapper(String biggerString, String givenString, int ith){
        if(ith < 1){
            return -1;
        }
        return part1Recursive(biggerString, givenString, ith, 0, 0);
    }


    /**
     * This function is the main recursive function of part1.
     * @param biggerString The big string to search in.
     * @param givenString The substring to be searched inside bigger string.
     * @param ith Desired ith occurance of the givenString.
     * @param nth Current count of occurances of the givenString. Its initial value is 0.
     * @param index Index of biggerString to start searching from Its initial value is 0.
     * @return Index of the ith occurance of givenString in biggerString.
     */
    public static int part1Recursive(String biggerString, String givenString, int ith, int nth, int index){

        /**
         * The base case is when the index reaches to the end of biggerString. If it does, -1 is returned as
         * the givenString doesn’t occur in the biggerString or the number of occurences is less than i.
         */
        if(index + givenString.length() -1 >= biggerString.length()){
            return -1;
        }
        
        /**
         * A substring with length of givenString's length is generated and compared with givenString.
         * If they are equal, nth is incremented.
         * If nth is equal to ith, we reached our base case and found the ith occurance. Index is returned.
         * Else, index position is incremented and function is called again.
         */
        String buf = biggerString.substring(index, index + givenString.length());
        if(givenString.equals(buf)){
            nth++;
            if(ith == nth){
                return index;
            }
            else{
                return part1Recursive(biggerString, givenString, ith, nth, index + 1);
            }
        }

        /**
         * Else, the function is called again by incrementing the index position.
         */
        return part1Recursive(biggerString, givenString, ith, nth, index + 1);
    }
}