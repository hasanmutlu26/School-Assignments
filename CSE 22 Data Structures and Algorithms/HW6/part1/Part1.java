import java.util.Random;

public class Part1{
    static final int SMALL = 100;
    static final int MEDIUM = 1000;
    static final int LARGE = 10000;
    public static void main(String[] args) {
        HashMapChain<String, String> smallArrHashChain = new HashMapChain<String, String>();
        HashMapChain<String, String> mediumArrHashChain = new HashMapChain<String, String>();
        HashMapChain<String, String> largeArrHashChain = new HashMapChain<String, String>();
        CombinedHashMap<String, String> smallArrCombinedHash = new CombinedHashMap<String, String>();
        CombinedHashMap<String, String> mediumArrCombinedHash = new CombinedHashMap<String, String>();
        CombinedHashMap<String, String> largeArrCombinedHash = new CombinedHashMap<String, String>();
        String[][] smallArr = new String[SMALL][2];
        String[][] mediumArr = new String[MEDIUM][2];
        String[][] largeArr = new String[LARGE][2];

        long smallChainAvg = 0;
        long smallChainAccessAvg = 0;
        long smallChainAccessAvgNonExt = 0;
        long smallChainDeleteAvg = 0; 
        long mediumChainAvg = 0;
        long mediumChainAccessAvg = 0;
        long mediumChainAccessAvgNonExt = 0;
        long mediumChainDeleteAvg = 0; 
        long largeChainAvg = 0;
        long largeChainAccessAvg = 0;
        long largeChainAccessAvgNonExt = 0;
        long largeChainDeleteAvg = 0; 
        long smallCombinedAvg = 0;
        long smallCombinedAccessAvg = 0;
        long smallCombinedAccessAvgNonExt = 0;
        long smallCombinedDeleteAvg = 0;
        long mediumCombinedAvg = 0;
        long mediumCombinedAccessAvg = 0;
        long mediumCombinedAccessAvgNonExt = 0;
        long mediumCombinedDeleteAvg = 0;
        long largeCombinedAvg = 0;
        long largeCombinedAccessAvg = 0;
        long largeCombinedAccessAvgNonExt = 0;
        long largeCombinedDeleteAvg = 0;

        long startTime;
        long endTime;

        System.out.println("Part1.1 Test: HashMapChain Test:");
        for(int i = 0; i < SMALL; i++){
            smallArr[i][0] = randomStringGenerator();
            smallArr[i][1] = randomStringGenerator();
        }
        for(int i = 0; i < SMALL; i++){
            smallArrHashChain.put(smallArr[i][0], smallArr[i][1]);
        }
        System.out.println("100 random String/String key/value pairs are put in table.");
        System.out.println("Some random values from the map:");
        System.out.println("\nKey and Value should be: " + smallArr[25][0] + " - " + smallArr[25][1]);
        System.out.println("Key: " + smallArr[25][0] + " \nValue in table: " + smallArrHashChain.get(smallArr[25][0]));
        System.out.println("\nKey and Value should be: " + smallArr[50][0] + " - " + smallArr[50][1]);
        System.out.println("Key: " + smallArr[50][0] + " \nValue in table: " + smallArrHashChain.get(smallArr[50][0]));
        System.out.println("\nKey and Value should be: " + smallArr[75][0] + " - " + smallArr[75][1]);
        System.out.println("Key: " + smallArr[75][0] + " \nValue in table: " + smallArrHashChain.get(smallArr[75][0]));

        System.out.println("\nTest Delete:");
        System.out.println("Delete elements with key " + smallArr[25][0] + " and " + smallArr[50][0]);
        smallArrHashChain.remove(smallArr[25][0]);
        smallArrHashChain.remove(smallArr[50][0]);
        System.out.println("Values after deletion: ");
        System.out.println("Key: " + smallArr[25][0] + "- Value in table: " + smallArrHashChain.get(smallArr[25][0]));
        System.out.println("Key: " + smallArr[50][0] + "- Value in table: " + smallArrHashChain.get(smallArr[50][0]));

        System.out.println("\nSize of the table: " + smallArrHashChain.size());
        
        System.out.println("-----------------------");
        System.out.println("Part1.2 Test: CombinedHashMap Test:");
        for(int i = 0; i < SMALL; i++){
            smallArrCombinedHash.put(smallArr[i][0], smallArr[i][1]);
        }
        System.out.println("Same 100 random String/String key/value pairs are put in table.");
        System.out.println("Some random values from the map:");
        System.out.println("\nKey and Value should be: " + smallArr[25][0] + " - " + smallArr[25][1]);
        System.out.println("Key: " + smallArr[25][0] + " \nValue in table: " + smallArrCombinedHash.get(smallArr[25][0]));
        System.out.println("\nKey and Value should be: " + smallArr[50][0] + " - " + smallArr[50][1]);
        System.out.println("Key: " + smallArr[50][0] + " \nValue in table: " + smallArrCombinedHash.get(smallArr[50][0]));
        System.out.println("\nKey and Value should be: " + smallArr[75][0] + " - " + smallArr[75][1]);
        System.out.println("Key: " + smallArr[75][0] + " \nValue in table: " + smallArrCombinedHash.get(smallArr[75][0]));

        System.out.println("\nTest Delete:");
        System.out.println("Delete elements with key " + smallArr[25][0] + " and " + smallArr[50][0]);
        smallArrCombinedHash.remove(smallArr[25][0]);
        smallArrCombinedHash.remove(smallArr[50][0]);
        System.out.println("Values after deletion: ");
        System.out.println("Key: " + smallArr[25][0] + "- Value in table: " + smallArrCombinedHash.get(smallArr[25][0]));
        System.out.println("Key: " + smallArr[50][0] + "- Value in table: " + smallArrCombinedHash.get(smallArr[50][0]));

        System.out.println("\nSize of the table: " + smallArrCombinedHash.size());

        System.out.println("\n\nPart1.3: Performance tests:");


        for(int j = 0; j < 100; j++){
            if(j % 10 == 0)
                System.out.print("%" + j + " - ");
            for(int i = 0; i < SMALL; i++){
                smallArr[i][0] = randomStringGenerator();
                smallArr[i][1] = randomStringGenerator();
            }
    
            for(int i = 0; i < MEDIUM; i++){
                mediumArr[i][0] = randomStringGenerator();
                mediumArr[i][1] = randomStringGenerator();
            }
    
            for(int i = 0; i < LARGE; i++){
                largeArr[i][0] = randomStringGenerator();
                largeArr[i][1] = randomStringGenerator();
            }
    
            //

            startTime = System.nanoTime();
            for(int i = 0; i < SMALL; i++){
                smallArrHashChain.put(smallArr[i][0], smallArr[i][1]);
            }
            endTime = System.nanoTime();
            smallChainAvg += (endTime - startTime) / 1000;

            startTime = System.nanoTime();
            for(int i = 0; i < MEDIUM; i++){
                mediumArrHashChain.put(mediumArr[i][0], mediumArr[i][1]);
            }
            endTime = System.nanoTime();
            mediumChainAvg += (endTime - startTime) / 1000;

            startTime = System.nanoTime();
            for(int i = 0; i < LARGE; i++){
                largeArrHashChain.put(largeArr[i][0], largeArr[i][1]);
            }
            endTime = System.nanoTime();
            largeChainAvg += (endTime - startTime) / 1000;

            //

            startTime = System.nanoTime();
            smallArrHashChain.get(smallArr[25][0]);
            endTime = System.nanoTime();
            smallChainAccessAvg += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            mediumArrHashChain.get(mediumArr[250][0]);
            endTime = System.nanoTime();
            mediumChainAccessAvg += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            largeArrHashChain.get(largeArr[2500][0]);
            endTime = System.nanoTime();
            largeChainAccessAvg += (endTime - startTime);// / 1000;

            //

            startTime = System.nanoTime();
            smallArrHashChain.get("nonext");
            endTime = System.nanoTime();
            smallChainAccessAvgNonExt += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            mediumArrHashChain.get("nonext");
            endTime = System.nanoTime();
            mediumChainAccessAvgNonExt += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            largeArrHashChain.get("nonext");
            endTime = System.nanoTime();
            largeChainAccessAvgNonExt += (endTime - startTime);// / 1000;

            //

            startTime = System.nanoTime();
            smallArrHashChain.remove(smallArr[25][0]);
            endTime = System.nanoTime();
            smallChainDeleteAvg += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            mediumArrHashChain.remove(mediumArr[250][0]);
            endTime = System.nanoTime();
            mediumChainDeleteAvg += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            largeArrHashChain.remove(largeArr[2500][0]);
            endTime = System.nanoTime();
            largeChainDeleteAvg += (endTime - startTime);// / 1000;

            /*-*/

            startTime = System.nanoTime();
            for(int i = 0; i < SMALL; i++){
                smallArrCombinedHash.put(smallArr[i][0], smallArr[i][1]);
            }
            endTime = System.nanoTime();
            smallCombinedAvg += (endTime - startTime) / 1000;

            startTime = System.nanoTime();
            for(int i = 0; i < MEDIUM; i++){
                mediumArrCombinedHash.put(mediumArr[i][0], mediumArr[i][1]);
            }
            endTime = System.nanoTime();
            mediumCombinedAvg += (endTime - startTime) / 1000;

            startTime = System.nanoTime();
            for(int i = 0; i < LARGE; i++){
                largeArrCombinedHash.put(largeArr[i][0], largeArr[i][1]);
            }
            endTime = System.nanoTime();
            largeCombinedAvg += (endTime - startTime) / 1000;

            //

            startTime = System.nanoTime();
            smallArrCombinedHash.get(smallArr[25][0]);
            endTime = System.nanoTime();
            smallCombinedAccessAvg += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            mediumArrCombinedHash.get(mediumArr[250][0]);
            endTime = System.nanoTime();
            mediumCombinedAccessAvg += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            largeArrCombinedHash.get(largeArr[2500][0]);
            endTime = System.nanoTime();
            largeCombinedAccessAvg += (endTime - startTime);// / 1000;

            //

            startTime = System.nanoTime();
            smallArrCombinedHash.get("nonext");
            endTime = System.nanoTime();
            smallCombinedAccessAvgNonExt += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            mediumArrCombinedHash.get("nonext");
            endTime = System.nanoTime();
            mediumCombinedAccessAvgNonExt += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            largeArrCombinedHash.get("nonext");
            endTime = System.nanoTime();
            largeCombinedAccessAvgNonExt += (endTime - startTime);// / 1000;

            //

            startTime = System.nanoTime();
            smallArrCombinedHash.remove(smallArr[25][0]);
            endTime = System.nanoTime();
            smallCombinedDeleteAvg += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            mediumArrCombinedHash.remove(mediumArr[250][0]);
            endTime = System.nanoTime();
            mediumCombinedDeleteAvg += (endTime - startTime);// / 1000;

            startTime = System.nanoTime();
            largeArrCombinedHash.remove(largeArr[2500][0]);
            endTime = System.nanoTime();
            largeCombinedDeleteAvg += (endTime - startTime);// / 1000;
        }
        
        System.out.println();
        System.out.println();

        smallChainAvg /= 100;
        smallChainAccessAvg /= 100;
        smallChainAccessAvgNonExt /= 100;
        smallChainDeleteAvg /= 100; 
        mediumChainAvg /= 100;
        mediumChainAccessAvg /= 100;
        mediumChainAccessAvgNonExt /= 100;
        mediumChainDeleteAvg /= 100; 
        largeChainAvg /= 100;
        largeChainAccessAvg /= 100;
        largeChainAccessAvgNonExt /= 100;
        largeChainDeleteAvg /= 100; 
        smallCombinedAvg /= 100;
        smallCombinedAccessAvg /= 100;
        smallCombinedAccessAvgNonExt /= 100;
        smallCombinedDeleteAvg /= 100;
        mediumCombinedAvg /= 100;
        mediumCombinedAccessAvg /= 100;
        mediumCombinedAccessAvgNonExt /= 100;
        mediumCombinedDeleteAvg /= 100;
        largeCombinedAvg /= 100;
        largeCombinedAccessAvg /= 100;
        largeCombinedAccessAvgNonExt /= 100;
        largeCombinedDeleteAvg /= 100;

        System.out.println("Running Results:");
        System.out.println("Results of HashMapChain for small array: ");
        System.out.println("Average time of putting all elements: " + smallChainAvg + " microseconds.");
        System.out.println("Average access time to existing element: " + smallChainAccessAvg + " nanoseconds");
        System.out.println("Average access time for non-existing element: " + smallChainAccessAvgNonExt+ " nanoseconds");
        System.out.println("Average delete time: " + smallChainDeleteAvg + " nanoseconds");
        System.out.println();
        System.out.println("Results of HashMapChain for medium array: ");
        System.out.println("Average time of putting all elements: " + mediumChainAvg + " microseconds.");
        System.out.println("Average access time to existing element: " + mediumChainAccessAvg + " nanoseconds");
        System.out.println("Average access time for non-existing element: " + mediumChainAccessAvgNonExt+ " nanoseconds");
        System.out.println("Average delete time: " + mediumChainDeleteAvg + " nanoseconds");
        System.out.println();
        System.out.println("Results of HashMapChain for large array: ");
        System.out.println("Average time of putting all elements: " + largeChainAvg + " microseconds.");
        System.out.println("Average access time to existing element: " + largeChainAccessAvg + " nanoseconds");
        System.out.println("Average access time for non-existing element: " + largeChainAccessAvgNonExt+ " nanoseconds");
        System.out.println("Average delete time: " + largeChainDeleteAvg + " nanoseconds");

        System.out.println("\n\nResults of CombinedHashMap for small array: ");
        System.out.println("Average time of putting all elements: " + smallCombinedAvg + " microseconds.");
        System.out.println("Average access time to existing element: " + smallCombinedAccessAvg + " nanoseconds");
        System.out.println("Average access time for non-existing element: " + smallCombinedAccessAvgNonExt+ " nanoseconds");
        System.out.println("Average delete time: " + smallCombinedDeleteAvg + " nanoseconds");
        System.out.println();
        System.out.println("Results of CombinedHashMap for medium array: ");
        System.out.println("Average time of putting all elements: " + mediumCombinedAvg + " microseconds.");
        System.out.println("Average access time to existing element: " + mediumCombinedAccessAvg + " nanoseconds");
        System.out.println("Average access time for non-existing element: " + mediumCombinedAccessAvgNonExt+ " nanoseconds");
        System.out.println("Average delete time: " + mediumCombinedDeleteAvg + " nanoseconds");
        System.out.println();
        System.out.println("Results of CombinedHashMap for large array: ");
        System.out.println("Average time of putting all elements: " + largeCombinedAvg + " microseconds.");
        System.out.println("Average access time to existing element: " + largeCombinedAccessAvg + " nanoseconds");
        System.out.println("Average access time for non-existing element: " + largeCombinedAccessAvgNonExt+ " nanoseconds");
        System.out.println("Average delete time: " + largeCombinedDeleteAvg + " nanoseconds");
        
 

    }

    private static String randomStringGenerator(){
        int leftLimit = 48; // numeral '0'
        int rightLimit = 122; // letter 'z'
        int targetStringLength = 10;
        Random random = new Random();

        String generatedString = random.ints(leftLimit, rightLimit + 1)
        .filter(i -> (i <= 57 || i >= 65) && (i <= 90 || i >= 97))
        .limit(targetStringLength)
        .collect(StringBuilder::new, StringBuilder::appendCodePoint, StringBuilder::append)
        .toString();

        return generatedString;
    }
}