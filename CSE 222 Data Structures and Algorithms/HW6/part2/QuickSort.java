public class QuickSort <T extends Comparable <T>>{

  /** Sort the table using the quicksort algorithm.
      @param table The array to be sorted
   */
  public void sort(T[] table){
    quickSort(table, 0, table.length - 1);
  }

  /** Sort a part of the table using the quicksort algorithm.
      @param table The array to be sorted
      @param first The index of the low bound
      @param last The index of the high bound
   */
  private void quickSort(T[] table, int first, int last){
    if (first < last){ // There is data to be sorted.
      //System.out.println(first + " " + last);
      int pivIndex = partition(table, first, last);

      if(pivIndex - first <= last - (pivIndex + 1)){
        quickSort(table, first, pivIndex - 1);
      }
      else{
        quickSort(table, pivIndex + 1, last);
      }

      

      
    }
  }

  /** Partition the table so that values from first to pivIndex
       are less than or equal to the pivot value, and values from
       pivIndex to last are greater than the pivot value.
       @param table The table to be partitioned
       @param first The index of the low bound
       @param last  The index of the high bound
       @return The location of the pivot value
   */
  private int partition(T[] table, int first, int last){

    T pivot = table[first];
    int up = first;
    int down = last;
    do{

      while ( (up < last) && (pivot.compareTo(table[up]) >= 0)){
        up++;
      }

      while (pivot.compareTo(table[down]) < 0){
        down--;
      }

      if (up < down){ 
        swap(table, up, down);
      }
    }
    while (up < down); // Repeat while up is left of down.

    swap(table, first, down);

    return down;
  }

  /** Swap the items in table[i] and table[j].
    @param table The array that contains the items
    @param i The index of one item
    @param j The index of the other item
   */
  private void swap(T[] table, int i, int j){
    T temp = table[i];
    table[i] = table[j];
    table[j] = temp;
  }

}
