import java.util.Random;

public class Part2{

    final static int SMALL = 100;
    final static int MEDIUM = 1000;
    final static int LARGE = 100000;

    public static void main(String[] args) {
      MergeSort<Integer> mergeSorter = new MergeSort<Integer>();
      QuickSort<Integer> quickSorter = new QuickSort<Integer>();
      NewSort<Integer> newSorter = new NewSort<Integer>();
      Integer[] smallArray = new Integer[SMALL];
      Integer[] mediumArray = new Integer[MEDIUM];
      Integer[] largeArray = new Integer[LARGE];
      Integer[] smallArray2 = new Integer[SMALL];
      Integer[] mediumArray2 = new Integer[MEDIUM];
      Integer[] largeArray2 = new Integer[LARGE];
      Integer[] smallArray3 = new Integer[SMALL];
      Integer[] mediumArray3 = new Integer[MEDIUM];
      Integer[] largeArray3 = new Integer[LARGE];

      long mergeSortSmallAvg = 0;
      long mergeSortMediumAvg = 0;
      long mergeSortLargeAvg = 0;
      long quickSortSmallAvg = 0;
      long quickSortMediumAvg = 0;
      long quickSortLargeAvg = 0;
      long newSortSmallAvg = 0;
      long newSortMediumAvg = 0;
      //int newSortLargeAvg = 0;

      long startTime;
      long endTime;

      Random rand = new Random();

      for(int i = 0; i < MEDIUM; i++){
        if(i % 100 == 0){
            System.out.println("Sortings are being done: %" + i/10);
        }
        
        for(int j = 0; j < SMALL; j++){
            smallArray[j] = rand.nextInt(LARGE); 
            smallArray2[j] = smallArray[j];
            smallArray3[j] = smallArray[j];
        }

        for(int j = 0; j < MEDIUM; j++){
            mediumArray[j] = rand.nextInt(LARGE);
            mediumArray2[j] = mediumArray[j];
            mediumArray3[j] = mediumArray[j];
        }
        
        for(int j = 0; j < LARGE; j++){
            largeArray[j] = rand.nextInt(LARGE);
            largeArray2[j] = largeArray[j];
            largeArray3[j] = largeArray[j];
        }
        
        startTime = System.nanoTime();
        mergeSorter.sort(smallArray);
        endTime = System.nanoTime();
        mergeSortSmallAvg += (endTime - startTime) / 1000;
        //System.out.println(mergeSortSmallAvg);

        startTime = System.nanoTime();
        mergeSorter.sort(mediumArray);
        endTime = System.nanoTime();
        mergeSortMediumAvg += (endTime - startTime) / 1000;
        //System.out.println(mergeSortMediumAvg);

        startTime = System.nanoTime();
        mergeSorter.sort(largeArray);
        endTime = System.nanoTime();
        mergeSortLargeAvg += (endTime - startTime) / 1000;
        //System.out.println(mergeSortLargeAvg);

        //
        startTime = System.nanoTime();
        quickSorter.sort(smallArray2);
        endTime = System.nanoTime();
        quickSortSmallAvg += (endTime - startTime) / 1000;
        //System.out.println(quickSortSmallAvg);

        startTime = System.nanoTime();
        quickSorter.sort(mediumArray2);
        endTime = System.nanoTime();
        quickSortMediumAvg += (endTime - startTime) / 1000;
        //System.out.println(quickSortMediumAvg);

        startTime = System.nanoTime();
        quickSorter.sort(largeArray2);
        endTime = System.nanoTime();
        quickSortLargeAvg += (endTime - startTime) / 1000;
        //System.out.println(quickSortLargeAvg);

        //
        startTime = System.nanoTime();
        newSorter.sort(smallArray3);
        endTime = System.nanoTime();
        newSortSmallAvg += (endTime - startTime) / 1000;
        //System.out.println(newSortSmallAvg);

        startTime = System.nanoTime();
        newSorter.sort(mediumArray3);
        endTime = System.nanoTime();
        newSortMediumAvg += (endTime - startTime) / 1000;
        //System.out.println(newSortMediumAvg);

/*        startTime = System.nanoTime();
        newSorter.sort_nonrecursive(largeArray);
        endTime = System.nanoTime();
        newSortLargeAvg += (endTime - startTime) / 1000;
        System.out.println(newSortLargeAvg);*/
      }

      mergeSortSmallAvg /= 1000;
      mergeSortMediumAvg /= 1000;
      mergeSortLargeAvg /= 1000;
      quickSortSmallAvg /= 1000;
      quickSortMediumAvg /= 1000;
      quickSortLargeAvg /= 1000;
      newSortSmallAvg /= 1000;
      newSortMediumAvg /= 1000;

      System.out.println("-------------------------");
      System.out.println("MergeSort small array average: " + mergeSortSmallAvg + " microseconds");
      System.out.println("MergeSort medium array average: " + mergeSortMediumAvg + " microseconds");
      System.out.println("MergeSort large array average: " + mergeSortLargeAvg + " microseconds");
      System.out.println("-------------------------");
      System.out.println("Quicksort small array average: " + quickSortSmallAvg + " microseconds");
      System.out.println("Quicksort medium array average: " + quickSortMediumAvg + " microseconds");
      System.out.println("Quicksort large array average: " + quickSortLargeAvg + " microseconds");
      System.out.println("-------------------------");
      System.out.println("NewSort small array average: " + newSortSmallAvg + " microseconds");
      System.out.println("NewSort medium array average: " + newSortMediumAvg + " microseconds");
      System.out.println("Newsort was so bad for large array that even a single run took seconds.");
      System.out.println("-------------------------");
      


    }

    

}