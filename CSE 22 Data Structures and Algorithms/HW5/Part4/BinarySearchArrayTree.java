public class BinarySearchArrayTree <E extends Comparable<E>>{
    /**Initial capacity of array. Its value is 15 */
    private static final int INITIAL_CAPACITY = 15;

    /**Macro to represent null branches. -1 means NULL */
    private static final int NULL = -1;

    /**Node array of tree */
    private Node<E>[] tree;

    /**ındex of the root node */
    private int root;

    /**Number of nodes in tree */
    private int size = 0; 

    /**Capacity of tree array */
    private int capacity;

    /**Return value of add operation */
    protected boolean addReturn;

    /**Data of deleted element */
    protected E deleteReturn;

    /**
     * Constructor. Creates an empty tree.
     */
    @SuppressWarnings("unchecked")
    public BinarySearchArrayTree(){
        capacity = INITIAL_CAPACITY;
        tree = new Node[capacity];
        for(int i = 0; i < capacity; i++){
            tree[i] = new Node<E>();
        }
        root = NULL;
        size = 0;
    }

    /**
     * Constructor. Creates a tree as parameter root as root.
     * @param root Root of the tree to be created.
     */
    @SuppressWarnings("unchecked")
    protected BinarySearchArrayTree(Node<E> root){
        capacity = INITIAL_CAPACITY;
        tree = new Node[capacity];
        for(int i = 0; i < capacity; i++){
            tree[i] = new Node<E>();
        }
        this.root = 0;
        tree[this.root] = root;
        size = 1;
    }


    /**
     * Constructor. Creates a tree with data as root.
     * @param data Data of the root.
     */
    @SuppressWarnings("unchecked")
    public BinarySearchArrayTree(E data){
        capacity = INITIAL_CAPACITY;
        tree = new Node[capacity];
        for(int i = 0; i < capacity; i++){
            tree[i] = new Node<E>();
        }
        this.root = 0;
        tree[this.root].data = data;
        size = 1;
    }

    /**
     * A function that returns left subtree as a new tree.
     * @return left subtree by value;
     */
    public BinarySearchArrayTree<E> getLeftSubTree(){
        if(root != NULL && tree[root].left != NULL){
            int leftSubtree = tree[root].left;
            return new BinarySearchArrayTree<>(tree[leftSubtree]);
        }
        return null;
    }

    /**
     * A function that returns right subtree as a new tree.
     * @return right subtree by value;
     */
    public BinarySearchArrayTree<E> getRightSubTree(){
        if(root != NULL && tree[root].right != NULL){
            int rightSubtree = tree[root].right;
            return new BinarySearchArrayTree<>(tree[rightSubtree]);
        }
        return null;
    }

    /**
     * @return The data on root node.
     */
    public E getData(){
        if(root != NULL){
            return tree[root].data;
        }
        return null;
    }

    /**
     * Returns whethet the tree is leaf or not.
     * @return true if it is leaf, false if it isn't
     */
    public boolean isLeaf(){
        int leftSubtree = tree[root].left;
        int rightSubtree = tree[root].right;
        return (leftSubtree == NULL && rightSubtree == NULL);
    }

    /**
     * This function creates a string of the tree as in the format in the book.
     * @return String representation of the tree.
     */
    public String toString(){
        StringBuilder temp = new StringBuilder();
        preOrderTraverse(root, 1, temp);
        return temp.toString();
    }

    /**
     * Helper function for toString() method. It traverses the tree and appends the data to String.
     * @param node Index of the current node.
     * @param depth Depth of the current node in tree.
     * @param temp String buffer.
     */
    private void preOrderTraverse(int node, int depth, StringBuilder temp){
        for(int i = 1; i < depth; i++){
            temp.append("-");
        }
        if(node == NULL){
            temp.append("\n");
            return;
        }
        temp.append(tree[node].toString());
        temp.append("\n");
        int leftSubtree = tree[node].left;
        int rightSubtree = tree[node].right;
        if(leftSubtree == NULL && rightSubtree == NULL){
            return;
        }
        preOrderTraverse(leftSubtree, depth + 1, temp);
        preOrderTraverse(rightSubtree, depth + 1, temp);
        
    }

    /**
     * Searches for the target object in the tree. Wrapper function.
     * @param target Object to be searched for.
     * @return Reference to object, if it exists in tree. Null otherwise.
     */
    public E find(E target){
        return find(root, target);
    }

    /**
     * Searches for the target object in the tree.
     * @param currentRoot Index of the current root.
     * @param target Object to be searched for.
     * @return Reference to object, if it exists in tree. Null otherwise.
     */
    private E find(int currentRoot, E target){
        if(currentRoot == NULL){
            return null;
        }

        int compResult = target.compareTo(tree[currentRoot].data);
        if(compResult == 0){
            return tree[currentRoot].data;
        }
        else if(compResult < 0){
            int leftSubtree = tree[currentRoot].left;
            return find(leftSubtree, target);
        }
        else{
            int rightSubtree = tree[currentRoot].right;
            return find(rightSubtree, target);
        }
    }

    /**
     * Adds a new object to tree, if it doesn't exist already. Wrapper function.
     * @param obj Object to be added.
     * @return True if it is added. False if it isn't.
     */
    public boolean add(E obj){
        if(size == capacity){
            reallocate();
        }
        root = add(root, obj);
        return addReturn;
    }

    /**
     * Adds a new object to tree, if it doesn't exist already.
     * @param currentRoot Index of the current root.
     * @param obj Object to be added.
     * @return New currentRoot value after the object is inserted.
     */
    private int add(int currentRoot, E obj){
        if(currentRoot == NULL){
            addReturn = true;
            size++;
            return addToFirstSpace(obj);
        }
        int compResult = obj.compareTo(tree[currentRoot].data);
        if(compResult == 0){
            addReturn = false;
            return currentRoot;
        }
        else if(compResult < 0){
            tree[currentRoot].left = add(tree[currentRoot].left, obj);
            return currentRoot;
        }
        else{
            tree[currentRoot].right = add(tree[currentRoot].right, obj);
            return currentRoot;
        }
    }

    /**
     * Adds the object to the first empty space in array.
     * @param obj Object to be added.
     * @return Index of the new object that has been added.
     */
    private int addToFirstSpace(E obj){
        int i;
        for(i = 0; i < size; i++){
            if(tree[i].data == null){
                tree[i].data = obj;
                break;
            }
        }
        return i;
    }

    /**
     * Deletes object from tree. Wrapper function.
     * @param target Object to be deleted.
     * @return The deleted object.
     */
    public E delete(E target){
        tree[root] = delete(root, target);
        return deleteReturn;
    }

    /**
     * Deletes object from tree. The deleted object's data is set to null to indicate it is empty.
     * @param currentRoot Index of the current root.
     * @param target Object to be deleted. 
     * @return New currentRoot value after the object is deleted.
     */
    private Node<E> delete(int currentRoot, E target){
        if(currentRoot == NULL){
            deleteReturn = null;
            return tree[currentRoot];
        }

        int compResult = target.compareTo(tree[currentRoot].data);
        if(compResult < 0){
            tree[tree[currentRoot].left] = delete(tree[currentRoot].left, target);
            return tree[currentRoot];
        }
        else if(compResult > 0){
            tree[tree[currentRoot].right] = delete(tree[currentRoot].right, target);
            return tree[currentRoot];
        }
        // target is in current root
        else{
            //If either one side of the currentRoot is null, return the other side.
            deleteReturn = tree[currentRoot].data;
            if(tree[currentRoot].left == NULL){
                int leftChild = tree[currentRoot].left;
                tree[currentRoot].data = null;
                size--;
                return tree[leftChild];
            }
            else if(tree[currentRoot].right == NULL){
                int rightChild = tree[currentRoot].left;
                tree[currentRoot].data = null;
                size--;
                return tree[rightChild];
            }
            // Node being deleted has 2 children
            else{
                //If left child don't have a right child, replace it with current root
                if(tree[tree[currentRoot].left].right == NULL){
                    int leftChild = tree[currentRoot].left;
                    tree[currentRoot].data = tree[tree[currentRoot].left].data;
                    tree[currentRoot].left = tree[tree[currentRoot].left].left;
                    tree[leftChild].data = null;
                    size--;
                    return tree[currentRoot];
                }
                //Else, replace with the largest child.
                else{
                    tree[currentRoot].data = findLargestChild(tree[currentRoot].left);
                    return tree[currentRoot];
                }
            }
        }
    }


    /**
     * Helper function for deleting. Find the largest child and replace it with its parent.
     * @param parent Index of the parent node.
     * @return Deleted item.
     */
    private E findLargestChild(int parent){
        if(tree[tree[parent].right].right == NULL){
            int rightChild = tree[parent].right;
            E returnValue = tree[tree[parent].right].data;
            tree[parent].right = tree[tree[parent].right].left;
            tree[rightChild].data = null;
            size--;
            return returnValue;
        }
        return findLargestChild(tree[parent].right);
    }

    /**
     * Removes an element from the tree.
     * @param target Object to de deleted.
     * @return True if it is deleted, false if not.
     */
    public boolean remove(E target){
        return delete(target) != null;
    }

    /**
     * Determine if an item is in the tree.
     * @param target Item to be searched for.
     * @return True if it exists, false if not.
     */
    public boolean contains(E target){
        return find(target) != null;
    }

    /**
     * Reallocates memory when necessary.
     */
    @SuppressWarnings("unchecked")
    private void reallocate() {
        capacity = 2 * capacity;
        Node<E>[] newData = new Node[capacity];
        for(int i = size; i < capacity; i++){
            newData[i] = new Node<E>();
        }
        System.arraycopy(tree, 0, newData, 0, size);
        tree = newData;
    }


    private static class Node<T>{
        protected T data = null;
        protected int left = -1;
        protected int right = -1;
        
        public String toString(){
            return data.toString();
        }

        public Node(){
            data = null;
            left = NULL;
            right = NULL;
        }
        
    }
}

