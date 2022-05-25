import java.util.ArrayList;

public class HashMapChain<K, V>{
    /** The table */
    private ArrayList<BinarySearchTree> table;

    /** Length of table **/
    private int tableLength;

    /** The number of keys */
    private int numKeys;

    /** The capacity */
    private static final int CAPACITY = 51;

    /** The maximum load factor */
    private static final double LOAD_THRESHOLD = 0.7;
    

    /**
     * Constructor
     */
    public HashMapChain(){
        numKeys = 0;
        table = new ArrayList<BinarySearchTree>(CAPACITY);
        tableLength = CAPACITY;
        for(int i = 0; i < tableLength; i++){
            table.add(new BinarySearchTree());
        }
    }

    /**
     * Method get for class HashMapChain.
     * @param key The key being searched
     * @return The value associated with this key if found; otherwise, null
     */
    @SuppressWarnings("unchecked")
    public V get(Object key){
        int index = key.hashCode() % tableLength;
        Entry<K, V> temp = new Entry<K, V>((K) key, null);
        if (index < 0)
            index += tableLength;
        if (table.get(index) == null)
            return null; // key is not in the table.

        // Search the list at table.get(index) to find the key.
        temp = table.get(index).find(temp);
        if(temp != null){
            return table.get(index).find(temp).value;
        }
        return null;
    }

    /**
     * Method put for class HashMapChain. This key-value pair is inserted in the table and numKeys is incremented. 
     * If the key is already in the table, its value is changed to the argument value and numKeys is not changed.
     * @param key   The key of item being inserted
     * @param value The value for this key
     * @return The old value associated with this key if found; otherwise, null
     */
    public V put(K key, V value){
        int index = key.hashCode() % tableLength;
        if (index < 0)
            index += tableLength;
        if (table.get(index) == null){
            // Create a new linked list at table.get(index).
            table.add(index, new BinarySearchTree());
        }

        Object temp = table.get(index).root;

        // assert: key is not in the table, add new item.
        V addReturn = table.get(index).add(new Entry<K, V>(key, value));

        if(temp == null){
            numKeys++;
            if (numKeys > (LOAD_THRESHOLD * tableLength))
                rehash();
        }
        
        return addReturn;
    }

    /**
     * Method remove for HashMapChain. The item with this key value is deleted from the table.
     * @param key Key of item to be deleted.
     * @return Value of the item that is deleted, if key was found; null otherwise.
     */
    public V remove(K key) {
        int index = key.hashCode() % tableLength;
        if (index < 0)
          index += tableLength;
        if (table.get(index) == null)
          return null; // Key not in table

        Entry<K,V> temp = table.get(index).delete(new Entry<K,V>(key, null));

        return temp.value; // Key not in table
      }

    /** Returns the number of entries in the map */
    public int size(){
        return numKeys;
    }

    /** Returns true if empty */
    public boolean isEmpty(){
        return numKeys == 0;
    }

    /**
     * Expands table size when loadFactor exceeds LOAD_THRESHOLD
     * post: the size of table is doubled and is an
     * odd integer. Each non-deleted entry from the original
     * table is reinserted into the expanded table.
     * The value of numKeys is reset to the number of items
     * actually inserted; numDeletes is reset to 0.
     */
    public void rehash(){
        // Save a reference to oldTable
        ArrayList<BinarySearchTree> oldTable = table;
        // Double capacity of this table
        
        table = new ArrayList<BinarySearchTree>(2 * tableLength+ 1);
        int oldtableLength = tableLength;
        tableLength = 2 * tableLength + 1;
        for(int i = 0; i < tableLength; i++){
            table.add(new BinarySearchTree());
        }
        // Reinsert all items in oldTable into expanded table.
        numKeys = 0;
        for (int i = 0; i < oldtableLength; i++){
            if (oldTable.get(i) != null){
                while(oldTable.get(i).root != null){
                    this.put(oldTable.get(i).root.data.key, oldTable.get(i).root.data.value);
                    oldTable.get(i).delete(new Entry<K,V>(oldTable.get(i).root.data.key, oldTable.get(i).root.data.value));
                }
            }
        }
    }


    /** Contains key-value pairs for a hash table. */
    private static class Entry<K, V>{

        /** The key */
        private K key;

        /** The value */
        private V value;

        /**
         * Creates a new key-value pair.
         * 
         * @param key   The key
         * @param value The value
         */
        public Entry(K key, V value){
            this.key = key;
            this.value = value;
        }

        public int compareTo(Entry<K, V> e){
            int eHash = e.key.hashCode();
            int myHash = key.hashCode();
            return myHash - eHash;
        }
    }

    /**
     * Special BinarySearchTree only to serve this HashMapChain.
     */
    private class BinarySearchTree{
        // Data Fields

        protected Node<Entry<K, V>> root;
        /** Return value from the public add method. */
        protected V addReturn;

        /** Return value from the public delete method. */
        protected Entry<K, V> deleteReturn;

        // Methods
        /**
         * Starter method find.
         * pre: The target object must implement
         * the Comparable interface.
         * 
         * @param target The Comparable object being sought
         * @return The object, if found, otherwise null
         */
        public Entry<K, V> find(Entry<K, V> target){
            return find(root, target);
        }

        /**
         * Recursive find method.
         * 
         * @param localRoot The local subtree�s root
         * @param target    The object being sought
         * @return The object, if found, otherwise null
         */
        private Entry<K, V> find(Node<Entry<K, V>> localRoot, Entry<K, V> target){
            if (localRoot == null)
                return null;

            // Compare the target with the data field at the root.
            int compResult = target.compareTo(localRoot.data);
            if (compResult == 0)
                return localRoot.data;
            else if (compResult < 0)
                return find(localRoot.left, target);
            else
                return find(localRoot.right, target);
        }

        /**
         * Starter method add.
         * pre: The object to insert must implement the
         * Comparable interface.
         * 
         * @param item The object being inserted
         * @return true if the object is inserted, false
         *         if the object already exists in the tree
         */
        public V add(Entry<K, V> item){
            root = add(root, item);
            return addReturn;
        }

        /**
         * Recursive add method.
         * post: The data field addReturn is set true if the item is added to
         * the tree, false if the item is already in the tree.
         * 
         * @param localRoot The local root of the subtree
         * @param item      The object to be inserted
         * @return The new local root that now contains the
         *         inserted item
         */
        private Node<Entry<K, V>> add(Node<Entry<K, V>> localRoot, Entry<K, V> item){
            if (localRoot == null){
                // item is not in the tree � insert it.
                return new Node<Entry<K, V>>(item);
            } else if (item.compareTo(localRoot.data) == 0 && item.key.equals(localRoot.data.key)){
                    addReturn = localRoot.data.value;
                    localRoot.data = item;
                    return localRoot;
                
            } else if (item.compareTo(localRoot.data) < 0){
                // item is less than localRoot.data
                localRoot.left = add(localRoot.left, item);
                return localRoot;
            } else{
                // item is greater than localRoot.data
                localRoot.right = add(localRoot.right, item);
                return localRoot;
            }
        }

        /**
         * Starter method delete.
         * post: The object is not in the tree.
         * 
         * @param target The object to be deleted
         * @return The object deleted from the tree
         *         or null if the object was not in the tree
         * @throws ClassCastException if target does not implement
         *                            Comparable
         */
        public Entry<K, V> delete(Entry<K, V> target){
            root = delete(root, target);
            return deleteReturn;
        }

        /**
         * Recursive delete method.
         * post: The item is not in the tree;
         * deleteReturn is equal to the deleted item
         * as it was stored in the tree or null
         * if the item was not found.
         * 
         * @param localRoot The root of the current subtree
         * @param item      The item to be deleted
         * @return The modified local root that does not contain
         *         the item
         */
        private Node<Entry<K, V>> delete(Node<Entry<K, V>> localRoot, Entry<K, V> item){
            if (localRoot == null){
                // item is not in the tree.
                deleteReturn = null;
                return localRoot;
            }

            // Search for item to delete.
            int compResult = item.compareTo(localRoot.data);
            if (compResult < 0){
                // item is smaller than localRoot.data.
                localRoot.left = delete(localRoot.left, item);
                return localRoot;
            } else if (compResult > 0){
                // item is larger than localRoot.data.
                localRoot.right = delete(localRoot.right, item);
                return localRoot;
            } else{
                // item is at local root.
                deleteReturn = localRoot.data;
                if (localRoot.left == null){
                    // If there is no left child, return right child
                    // which can also be null.
                    return localRoot.right;
                } else if (localRoot.right == null){
                    // If there is no right child, return left child.
                    return localRoot.left;
                } else{
                    // Node being deleted has 2 children, replace the data
                    // with inorder predecessor.
                    if (localRoot.left.right == null){
                        // The left child has no right child.
                        // Replace the data with the data in the
                        // left child.
                        localRoot.data = localRoot.left.data;
                        // Replace the left child with its left child.
                        localRoot.left = localRoot.left.left;
                        return localRoot;
                    } else{
                        // Search for the inorder predecessor (ip) and
                        // replace deleted node�s data with ip.
                        localRoot.data = findLargestChild(localRoot.left);
                        return localRoot;
                    }
                }
            }
        }


        /**
         * Find the node that is the
         * inorder predecessor and replace it
         * with its left child (if any).
         * post: The inorder predecessor is removed from the tree.
         * 
         * @param parent The parent of possible inorder
         *               predecessor (ip)
         * @return The data in the ip
         */
        private Entry<K, V> findLargestChild(Node<Entry<K, V>> parent){
            // If the right child has no right child, it is
            // the inorder predecessor.
            if (parent.right.right == null){
                Entry<K, V> returnValue = parent.right.data;
                parent.right = parent.right.left;
                return returnValue;
            } else{
                return findLargestChild(parent.right);
            }
        }

        protected class Node<E>{
            // Data Fields
            /** The information stored in this node. */
            protected E data;

            /** Reference to the left child. */
            protected Node<E> left;

            /** Reference to the right child. */
            protected Node<E> right;

            // Constructors
            /**
             * Construct a node with given data and no children.
             * 
             * @param data The data to store in this node
             */
            public Node(E data){
                this.data = data;
                left = null;
                right = null;
            }

            // Methods
            /**
             * Return a string representation of the node.
             * 
             * @return A string representation of the data fields
             */
            public String toString(){
                return data.toString();
            }
        }

    }
}
