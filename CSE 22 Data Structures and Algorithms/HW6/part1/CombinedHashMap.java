
public class CombinedHashMap <K, V>{
    private Entry<K, V> table[];

    private int numKeys;

    private int PRIME;

    /** The capacity */
    private static final int CAPACITY = 51;

    /** The maximum load factor */
    private static final double LOAD_THRESHOLD = 0.7;

    /**
     * Constructor
     */
    @SuppressWarnings("unchecked")
    public CombinedHashMap(){
        table = new Entry[CAPACITY];
        for(int i = 0; i < CAPACITY; i++){
            table[i] = new Entry<K,V>();
        }
    }

    /**
     * Method get for CombinedHashMap. 
     * @param key Key of the item to get from hash table.
     * @return The itemy with the key, if exists; otherwise null.
     */
    public V get(Object key){
        int index = key.hashCode() % table.length;
        if (index < 0)
            index += table.length;
        if (table[index].value == null)
            return null;
        int i = 0;
        while(true){
            if(table[index].key == null)
                return null;
            if(table[index].key.equals(key)){
                return table[index].value;
            }
            if(i > table.length){
                int j;
                for(j = 0; j < table.length; j++){
                    if(table[j].key.equals(key)){
                        return table[j].value;
                    }
                }
                if(j >= table.length){
                    return null;
                }
            }
            if(table[index].next != -1){
                index = table[index].next;
            }
            else{
                index = (hash1(key.hashCode()) + i*hash2(key.hashCode())) % table.length;
                if (index < 0)
                    index += table.length;
                i++;
            }
        }
        
    }

    /**
     * Method put for CombinedHashMap. It adds a new entry to hash table.
     * @param key Key of the new entry
     * @param value Value of the new entry
     * @return If an item with this key already exists, that item is updated and its 
     * former value is returned. Null otherwise.
     */
    public V put(K key, V value){
        int index = key.hashCode() % table.length;
        if (index < 0)
            index += table.length;
        
        int i = 0; 
        int prevIndex = index;
        while(true){
            if (index < 0)
                index += table.length;
            if(i > table.length){
                boolean flag = false;
                int j;
                for(j = 0; j < table.length; j++){
                    if(table[j].value == null){
                        table[j].key = key;
                        table[j].value = value;
                        table[j].next = -1;
                        numKeys++;
                        flag = true;
                    }
                    if(flag)
                        break;
                }
                if(j == table.length)
                    rehash();
            }
            if(table[index].value == null){
                table[index].key = key;
                table[index].value = value;
                table[index].next = -1;
                if(table[prevIndex].next == -1 && prevIndex != index)
                    table[prevIndex].next = index;
                numKeys++;
                break;
            }
            else if(table[index].key.equals(key)){
                V returnVal = table[index].value;
                table[index].value = value;
                return returnVal;
            }
            else{
                prevIndex = index;
                index = (hash1(key.hashCode()) + i*hash2(key.hashCode())) % table.length;
                if(index == prevIndex)
                    index = 0;
                if (index < 0)
                    index += table.length;
                i++;
            }
            //System.out.println("index: " + index);
        }
        if (numKeys > (LOAD_THRESHOLD * table.length))
            rehash();
        return null;
    }
    

    /**
     * Removes an item from the table.
     * @param key Key of the item to be removed.
     * @return The value of the removed item, if exists; otherwise null.
     */
    public V remove(K key){
        int index = key.hashCode() % table.length;
        V returnVal = null;
        if (index < 0)
            index += table.length;
        if (table[index].value == null)
            return null;
        
        while(true){
            if(table[index].key.equals(key)){
                if(table[index].next != -1){
                    int temp = table[index].next;
                    returnVal = table[index].value;
                    table[index].value = table[temp].value;
                    table[index].key = table[temp].key;
                    table[index].next = table[temp].next;
                    table[temp].value = null;
                    table[temp].key = null;
                    table[temp].next = -1;
                    if(table[index].next == index){
                        table[index].next = -1;
                    }
                    if(table[temp].next == temp){
                        table[temp].next = -1;
                    }
                }
                else{
                    returnVal = table[index].value;
                    table[index].value = null;
                }
                numKeys--;
                break;
            }
            else if(table[index].next != -1){
                index = table[index].next;
            }
            else{
                return null;
            }
        }
        return returnVal;
    }


    /**
     * Returns number of entries in table.
     * @return Number of entries.
     */
    public int size(){
        return numKeys;
    }

    /**
     * isEmpty method for CombinedHashMap
     * @return true if empty, false otherwise.
     */
    public boolean isEmpty(){
        return numKeys == 0;
    }

    /**
     * Method rehash for CombinedHashMap. 
     */
    @SuppressWarnings("unchecked")
    public void rehash(){
        Entry<K, V>[] oldTable = table;
        table = new Entry[2*table.length+1];
        for(int i = 0; i < table.length; i++){
            table[i] = new Entry<K,V>();
        }
        numKeys = 0;
        for(int i = 0; i < oldTable.length; i++){
            if(oldTable[i].value != null){
                this.put(oldTable[i].key, oldTable[i].value);
                oldTable[i].value = null;
                oldTable[i].key = null;
            }
        }

    }

    /**
     * Hash1 function
     * @param key
     * @return
     */
    private int hash1(int key){
        return key*table.length;
    }

    /**
     * Hash2 function
     * @param key
     * @return
     */
    private int hash2(int key){
        PRIME = table.length - 1;
        while(isPrime(PRIME))
            PRIME--;
        return PRIME - (key % PRIME);
    }

    /**
     * isPrime function to help hash2
     * @param num
     * @return
     */
    private boolean isPrime(int num){
        for(int i = 2; i <= num; i++){
            if(num % i == 0)
                return false;
        }
        return true;
    }

    /** Contains key-value pairs for a hash table. */
    private static class Entry<K, V>{

        /** The key */
        private K key;

        /** The value */
        private V value;

        /**Index of the next colliding item. */
        private int next;

        public Entry(){
            this.key = null;
            this.value = null;
            this.next = -1;
        }
    }
}