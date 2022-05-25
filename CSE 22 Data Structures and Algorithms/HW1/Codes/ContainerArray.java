public class ContainerArray<T>{
    private T arr[];
    private int size;

    /**
     * Constructor. Size is 0 as default.
     */
    public ContainerArray(){
        size = 0;
    }

    /**
     * This function adds a new entry to the ContainerArray
     * @param obj The entry to be added to the ContainerArray
     */
    @SuppressWarnings("unchecked")
    public void add(T obj){
        T[] temp = (T[]) new Object[++size];
        
        for(int i = 0; i < size - 1; i++){
            temp[i] = arr[i];
        }
        
        temp[size-1] = obj;
        arr = (T[]) new Object[size];

        for(int i = 0; i < size; i++){
            arr[i] = temp[i];
        }
    }

    /**
     * This function deletes an entry at the index of ind from the ContainerArray.
     * @param ind Index of the entry to be deleted.
     * @return True if successful, false if index is incorrect.
     */
    public boolean delete(int ind){
        if(ind < 0 || ind > size - 1 || size < 0){
            System.out.println("Error: Incorrect index.");
            return false;
        }
        for(int i = ind; i < size - 1; i++){
            arr[i] = arr[i+1];
        }
        arr[--size] = null;
        return true;
    }

    /**
     * @return Current size of the ContainerArray.
     */
    public int size(){
        return this.size;
    }

    /**
     * @param i Index of the entry.
     * @return The entry at the index of i.
     */
    public T get(int i){
        return this.arr[i];
    }

    

    /**
     * This function returns list of its objects toString() properties.
     * @return List of its objects toString() properties.
     */
    @Override
    public String toString(){
        String temp = "";
        if(size == 0){
            return "Empty";
        }
        //temp += ("Size of array: " + size + "\n");
        for(int i = 0; i < size - 1; i++){
            temp += ((i+1) + ":" + arr[i].toString() + "\n\n");
        }
        temp += ((size) + ":" + arr[size-1].toString());
        return temp;
    }
    
    /**
     * This function checks if an object is already in the ContainerArray.
     * @param obj The object to be checked
     * @return True if object exists, false if not.
     
    public boolean consist(T obj){
        for(int i = 0; i < size; i++){
            if(arr[i].equals(obj))
                return true;
        }
        return false;
    }
    */
}