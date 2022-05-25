import java.util.AbstractList;
import java.util.List;

/**
 * @see LDLinkedList.OneLinkedList
 * @see LDLinkedList.Node
 */
public class LDLinkedList<T> extends AbstractList<T> implements List<T>{
    /**
     * Main linked list.
     */
    private OneLinkedList<T> mainList;

    /**
     * Linked list of deleted items.
     */
    private OneLinkedList<T> binList;       

    public LDLinkedList(){
        mainList = new OneLinkedList<T>();
        binList = new OneLinkedList<T>();
    }    

    public LDLinkedList(T obj){
        mainList = new OneLinkedList<T>(obj);
        binList = new OneLinkedList<T>();
    }

    /**
     * This function returns first object in the list.
     * @return first object in the list.
     */
    public T getFirst(){
        return mainList.getFirst();
    }

    /**
     * This function returns last object in the list.
     * @return last object in the list.
     */
    public T getLast(){
        return mainList.getLast();
    }

    /**
     * This function returns object in specified index in the list.
     * @param index index of the object in the list.
     * @return Object at index
     */
    public T get(int index){
        return mainList.get(index);
    }

    /**
     * This function returns first item in deleted objects list.
     * @return first item in deleted objects list.
     */
    public T getDeletedFirst(){
        return binList.getFirst();
    }

    /**
     * This function returns last item in deleted objects list.
     * @return last item in deleted objects list.
     */
    public T getDeletedLast(){
        return binList.getLast();
    }

    /**
     * This function returns size of the list.
     * @return Size of the list.
     */
    public int size(){
        return mainList.size();
    }

    /**
     * This function returns size of the deleted objects list.
     * @return Size of the deleted objects list.
     */
    public int deletedSize(){
        return binList.size();
    }

    /**
     * This function returns the item in specified index from deleted objects list
     * @param index index of object
     * @return object at the index
     */
    public T getDeleted(int index){
        return binList.get(index);
    }


    /**
     * This function adds an object to the main list. It first checks the existance of the object in the deleted objects list.
     * If it exists, the node in deleted objects list will be transferred to main list. If not, a new object is created and added.
     * @param obj Object to be added to the list.
     * @return always true
     */
    public boolean add(T obj){
        Node<T> temp = binList.includes(obj);
        if(temp != null){
            binList.delete(temp.data);
            mainList.add(temp);
            return true;
        }
        return mainList.add(obj);
    }

    /**
     * This function adds an object to the main list in specified index. It first checks the existance of the object in the deleted objects list.
     * If it exists, the node in deleted objects list will be transferred to main list. If not, a new node is created and added.
     * @param index desired index of the object to be added at.
     * @param obj object to be added.
     */
    @Override
    public void add(int index, T obj){
        Node<T> temp = binList.includes(obj);
        if(temp != null){
            binList.delete(temp.data);
            mainList.add(index, temp);
            return;
        }
        mainList.add(index, obj);
    }

    /**
     * This function removes an object from the list.
     * @param obj object to be removed.
     * @return true if list contains the specified object and the object is deleted. false otherwise.
     */
    @Override
    public boolean remove(Object obj){
        Node<T> temp = mainList.delete(obj);
        if(temp != null){
            return binList.add(temp);
        }
        return false;
    }

    /**
     * This function removes the object at the specified index from the list.
     * @param index index of the object to be deleted
     * @return the deleted object.
     */
    public T remove(int index){
        Node<T> temp = mainList.delete(index);
        if(temp != null){
            binList.add(temp);
            return temp.data;
        }
        return null;
    }


    /**
     * This inner class is kind of a double linked list. It has the ability to not only add object to the list, but also 
     * add nodes that already has object data as well. This feature enables LDLinkedList class to transfer same exact node 
     * from main list to deleted list and vice versa. 
     */
    private static class OneLinkedList<T>{
        private Node<T> head = null;
        private Node<T> tail = null;
        private int size = 0;
    
        private OneLinkedList(){
            size = 0;
        }
    
        private OneLinkedList(T Obj){
            add(Obj);
        }
    
        private T getFirst(){
            return head.data;
        }
    
        private T getLast(){
            return tail.data;
        }
    
        private int size(){
            return size;
        }
    
        /**
         * This function returns the object at the specified position.
         * @param ind index of the object.
         * @return the object at the index ind
         * @throws IndexOutOfBoundsException if the index is out ouf list's bounds.
         */
        private T get(int ind){
            if(ind < 0 || ind >= size){
                throw new IndexOutOfBoundsException(Integer.toString(ind));
            }
            Node<T> iter = head;
            for(int i = 0; i < ind; i++){
                iter = iter.next;
            }
            return iter.data;
        }
    
        /**
         * This function adds a new object at the end of the list by creating a new node.
         * @param obj Object to be added 
         * @return true always. 
         */
        private boolean add(T obj){
            if(size == 0){
                head = new Node<T>(obj);
                tail = head;
                size++;
                return true;
            }
            add(size, obj);
            return true;
        }
    
        /**
         * This function adds a node at the end of the linked list.
         * @param node Node to be added.
         * @return true always.
         */
        private boolean add(Node<T> node){
            if(size == 0){
                head = node;
                tail = head;
                size++;
                return true;
            }
            add(size, node);
            return true;
        }
    
    
        /**
         * This function adds a new object to the list at the desired index by creating a new node.
         * @param index Desired index of the object to be added.
         * @param obj Object to be added.
         * @throws IndexOutOfBoundsException if the index is out ouf list's bounds.
         */
        private void add(int index, T obj){
            if(index < 0 || index > size){
                throw new IndexOutOfBoundsException(Integer.toString(index));
            }
            if(index == 0){
                head.prev = new Node<T>(null, obj, head);
                head = head.prev;
                size++;
                if(size == 0){
                    head.next = null;
                    tail = head;
                }
                return;
            }
            if(index == size){
                tail.next = new Node<T>(tail, obj, null);
                tail = tail.next;
                size++;
                return;
            }
            Node<T> iter = head;
            for(int i = 0; i < index && iter != null; i++){
                iter = iter.next;
            }
            Node<T> temp = new Node<T>(iter.prev, obj, iter);
            iter.prev.next = temp;
            iter.prev = temp;
            size++;
        }
    
        /**
         * This function adds a new node to the list at the desired index.
         * @param index Desired index of the node to be added.
         * @param node Node to be added.
         * @throws IndexOutOfBoundsException if the index is out ouf list's bounds.
         */
        private void add(int index, Node<T> node){
            if(index < 0 || index > size){
                throw new IndexOutOfBoundsException(Integer.toString(index));
            }
            node.next = null;
            node.prev = null;
            if(index == 0){
                node.next = head;
                head.prev = node;
                head = node;
                size++;
                return;
            }
            if(index == size){
                tail.next = node;
                node.prev = tail;
                tail = node;
                size++;
                return;
            }
            Node<T> iter = head;
            for(int i = 0; i < index && iter != null; i++){
                iter = iter.next;
            }
            node.prev = iter.prev;
            node.next = iter;
            iter.prev.next = node;
            iter.prev = node;
            size++;
        }
    
        /**
         * This function deletes an object from the list if it exists.
         * @param obj Object to be deleted.
         * @return The deleted node containing the object as data, if exists. If it doesn't exist, it returns null;
         */
        private Node<T> delete(Object obj){
            Node<T> iter = head;
            if(obj.equals(head.data)){
                Node<T> temp = head;
                head = head.next;
                //head.prev = null;
                size--;
                return temp;
            }
            if(obj.equals(tail.data)){
                Node<T> temp = tail;
                tail = tail.prev;
                tail.next = null;
                size--;
                return temp;
            }
            for(int i = 0; i < size && iter != null; i++){
                if(obj.equals(iter.data)){
                    iter.next.prev = iter.prev;
                    iter.prev.next = iter.next;
                    size--;
                    return iter;
                }
                iter = iter.next;
            }
            return null;
        }
        
        /**
         * This function checks whether an object is in the list.
         * @param obj Object to be searched.
         * @return The node of the object, if exists. If it doesn't exist, it returns null;
         */
        private Node<T> includes(T obj){
            Node<T> iter = head;
            for(int i = 0; i < size; i++){
                if(obj.equals(iter.data)){
                    return iter;
                }
            }
            return null;
        }
    
        /**
         * This function deletes object at specified position from list
         * @param index index of the object to be deleted
         * @return The node of the object, if exists.
         * @throws IndexOutOfBoundsException if the index is out ouf list's bounds.
         */
        private Node<T> delete(int index){
            if(index < 0 || index > size){
                throw new IndexOutOfBoundsException(Integer.toString(index));
            }
            if(index == 0){
                Node<T> temp = head;
                head = head.next;
                head.prev = null;
                size--;
                return temp;
            }
            if(index == size - 1){
                Node<T> temp = head;
                tail = tail.prev;
                tail.next = null;
                size--;
                return temp;
            }
            Node<T> iter = head;
            for(int i = 0; i < index && iter != null; i++){
                iter = iter.next;
            }
            iter.prev.next = iter.next;
            iter.next.prev = iter.prev;
            size--;
            return iter;
        }
    
    
    }
    
    /**
     * This class is node class for lists.
     */
    private static class Node<T>{
        private Node<T> prev;
        private T data;
        private Node<T> next;

        private Node(){
            prev = null;
            data = null;
            next = null;
        }

        private Node(T obj){
            prev = null;
            data = obj;
            next = null;
        }

        private Node(Node<T> prev, T data, Node<T> next){
            this.prev = prev;
            this.data = data;
            this.next = next;
        }

        private Node(Node<T> copy){
            this.prev = copy.prev;
            this.data = copy.data;
            this.next = copy.next;
        }
    }
}