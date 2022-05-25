public class NewSort <T extends Comparable <T>>{


    /**
     * Wrapper sort function
     * @param table array to be sorted.
     */
    public void sort(T[] table){
        new_sort(table, 0, table.length - 1);
    }

    /**
     * Wrapper of the non-recursive sort function.
     * @param table
     */
    public void sort_nonrecursive(T[] table){
        new_sort_nonrecursive(table, 0, table.length - 1);
    }

    /**
     * Recursive function to sort elements in array
     * @param table array to be sorted
     * @param head head index of array.
     * @param tail tail index of array.
     * @return
     */
    private T[] new_sort(T[] table, int head, int tail){
        int min; 
        int max;
        int min_max[] = new int[2];
        if(head > tail){
            return table;
        }
        else{
            min_max = min_max_finder(table, head, tail);
            min = min_max[0];
            max = min_max[1];
            swap(table, head, min);
            swap(table, tail, max);
            return new_sort(table, head+1, tail-1);
        }
    }

    /**
     * Non-recursive function to sort elements in an array
     * @param table the array
     * @param head head index of the array
     * @param tail tail index of the array
     * @return
     */
    private T[] new_sort_nonrecursive(T[] table, int head, int tail){
        int min; 
        int max;
        int min_max[] = new int[2];
        while(head <= tail){
            min_max = min_max_finder(table, head, tail);
            min = min_max[0];
            max = min_max[1];
            swap(table, head, min);
            swap(table, tail, max);
            head++;
            tail--;
        }
        return table;
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

  /**
   * Finds minimum and maximum elements between two indices in array
   * @param table the array
   * @param head head of array.
   * @param tail tail of array. 
   * @return
   */
  private int[] min_max_finder(T[] table, int head, int tail){
      int min = head;
      int max = head;
      int min_max[] = new int[2];
      for(int i = head; i <= tail; i++){
          if(table[i].compareTo(table[min]) < 0){
            min = i;
          }
          if(table[i].compareTo(table[max]) > 0){
            max = i;
          }
      }
      min_max[0] = min; min_max[1] = max;
      return min_max;
  }
/*
  private int[] min_max(T[] table, int head, int tail){
      int min_max[] = new int[2];
      if(head < tail){
        int uppermin, lowermin, uppermax, lowermax;
        int upper_min_max[] = new int[2];
        int lower_min_max[] = new int[2];
        lower_min_max = min_max(table, head, tail/2);
        upper_min_max = min_max(table, head*2, tail);
        lowermin = lower_min_max[0]; lowermax = lower_min_max[1];
        uppermin = upper_min_max[0]; uppermax = upper_min_max[1];
        if(table[lowermin].compareTo(table[uppermin]) < 0){
            min_max[0] = lowermin;
        }
        else{
            min_max[0] = uppermin;
        }
        if(table[lowermax].compareTo(table[uppermax]) > 0){
            min_max[1] = lowermax;
        }
        else{
            min_max[1] = uppermax;
        }
        return min_max;
      }
      min_max[0] = head;
      min_max[1] = head;
      return min_max;
  }*/

}