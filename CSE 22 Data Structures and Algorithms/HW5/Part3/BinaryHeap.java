import java.util.Comparator;

public class BinaryHeap<E extends Comparable<E>>{
    private Node<E> root;
    private Comparator<E> comparator = null;
    int size = 0;


    public BinaryHeap(){
        root = new Node<E>();
    }

    public BinaryHeap(E data){
        root = new Node<E>();
        root.data = data;
        root.index = size;
        size++;
    }

    protected BinaryHeap(Node<E> root){
        this.root = root;
    }

    public boolean add(E item){
        Node<E> child = new Node<E>();
        size++;
        child.data = item;
        child.index = size - 1;
        if(root.data == null){
            root = child;
            return true;
        }

        Node<E> parent = findWithIndex(root, (child.index-1)/2);
        if(parent.left == null){
            parent.left = child;
        }
        else if(parent.right == null){
            parent.right = child;
        }

        while(parent.index >= 0 && compare(parent.data, child.data) > 0){
            swap(parent, child);
            child = findWithIndex(root, parent.index);
            parent = findWithIndex(root, (child.index - 1) / 2);
        }
        return true;
    }

    private void swap(Node<E> parent, Node<E> child){
        E temp = parent.data;
        parent.data = child.data;
        child.data = temp;
    }

    /**
     * Deletes the root element from the heap.
     * @return Deleted element. Null if heap is empty.
     */
    public E delete(){
        if(isEmpty()){
            return null;
        }
        return delete(root.data);
    }

    /**
     * Returns specified item from the heap.
     * @param target Item to be deleted.
     * @return Deleted item. Null if heap is empty.
     */
    public E delete(E target){
        if(isEmpty()){
            return null;
        }
        BinaryHeap<E> afterDelete = new BinaryHeap<E>();
        E returnVal = null;
        for(int i = 0; i < size; i++){
            Node<E> temp = findWithIndex(root, i);
            if(temp.data.compareTo(target) == 0){
                returnVal = temp.data;
                continue;
            }
            else{
                afterDelete.add(temp.data);
            }
        }
        this.root = afterDelete.root;
        this.size = afterDelete.size;
        return returnVal;
    }

    /**
     * Specifies if heap is empty.
     * @return True if it is empty, false if not.
     */
    public boolean isEmpty(){
        return size == 0;
    }

    public BinaryHeap<E> getLeftSubtree(){
        if(root != null && root.left != null){
            return new BinaryHeap<E>(root.left);
        }
        else{
            return null;
        }
    }

    public BinaryHeap<E> getRightSubtree(){
        if(root != null && root.right != null){
            return new BinaryHeap<E>(root.right);
        }
        else{
            return null;
        }
    }

    public E getData(){
        if(root != null){
            return root.data;
        }
        else{
            return null;
        }
    }

    public boolean isLeaf(){
        return root.left == null && root.right == root.right;
    }

    public BinaryHeap<E> merge(BinaryHeap<E> y){
        BinaryHeap<E> newHeap = new BinaryHeap<E>();
        for(int i = 0; i < this.size; i++){
            newHeap.add(this.findWithIndex(this.root, i).data);
        }
        for(int i = 0; i < y.size; i++){
            newHeap.add(y.findWithIndex(y.root, i).data);
        }
        return newHeap;
    } 

    private Node<E> findWithIndex(Node <E> localRoot, int target) {
        if (localRoot == null)
          return null;
    
        // Compare the target with the data field at the root.
        boolean compResult = target == localRoot.index;
        if (compResult)
          return localRoot;
        else{
            Node<E> leftReturnee = findWithIndex(localRoot.left, target);
            Node<E> rightReturnee = findWithIndex(localRoot.right, target);
            if(leftReturnee != null){
                return leftReturnee;
            }
            else if(rightReturnee != null){
                return rightReturnee;
            }
            else{
                return null;
            }
        }
      }

    private int compare(E left, E right) {
        if (comparator != null) { // A Comparator is defined.
        return comparator.compare(left, right);
        }
        else { // Use left�s compareTo method.
        return left.compareTo(right);
        }
    }

    public String toString() {
        StringBuilder sb = new StringBuilder();
        preOrderTraverse(root, 1, sb);
        return sb.toString();
    }

    private void preOrderTraverse(Node < E > node, int depth, StringBuilder sb){
        if(node == null){
            return;
        }
        for (int i = 1; i < depth; i++) {
            sb.append("-");
        }
        
        sb.append(node.toString());
        sb.append("\n");
        preOrderTraverse(node.left, depth + 1, sb);
        preOrderTraverse(node.right, depth + 1, sb);
    }

    private static class Node<T>{
        T data;
        Node<T> left;
        Node<T> right;
        int index;

        private Node(){
            data = null;
            left = null;
            right = null; 
            index = -1;
        }

        public String toString(){
            return data.toString();
        }

    }
}