public class House extends Building{
    private int numOfRooms;
    private String color;

    public House(){
        super();
        this.numOfRooms = 0;
        this.color = "Unknown";
    }

    public House(int position, int length, int height, String owner, int numOfRooms, String color){
        super(position, length, height, owner);
        this.numOfRooms = numOfRooms;
        this.color = color;
    }

    public void setNumOfRooms(int numOfRooms){
        this.numOfRooms = numOfRooms;
    }

    public int getNumOfRooms(){
        return this.numOfRooms;
    }

    public void setColor(String color){
        this.color = color;
    }

    public String getColor(){
        return this.color;
    }

    @Override
    public String toString(){
        return "--House--" + super.toString() + "\nNumber of Rooms: " + this.numOfRooms + "\nColor: " + this.color;
    }


    /**
     * This function compares equality of two land objects. It is an overriden function from Object class.
     * @param obj object to compare equality.
     * @return true if equal, false if not.
     */
    @Override
    public boolean equals(Object obj){
        if(obj.getClass().getSimpleName() == "House"){
            House objBuf = (House)obj;
            return super.equals(obj) && objBuf.numOfRooms == this.numOfRooms && objBuf.color == this.color;
        }
        return false;
    }

    /**
     * This function enables to focus on a House.
     * @return A string that states its owner.
     */
    @Override
    public String focus(){
        return "You focus on this house and see it's owner is " + this.owner;
    }

    /*
    @Override
    public boolean isEqual(Land obj){
        if(obj instanceof House && super.isEqual(obj) && this.numOfRooms == ((House)obj).numOfRooms && this.color == ((House)obj).color){
            return true;
        }
        return false;
    }
    */
}