public abstract class Building extends Land{
    protected String owner;

    public Building(){
        super();
        this.owner = "Unknown";
    }

    public Building(int position, int length, int height, String owner){
        super(position, length, height);
        this.owner = owner;
    }

    public void setOwner(String owner){
        this.owner = owner;
    }

    public String getOwner(){
        return this.owner;
    }

    @Override
    public String toString(){
        return super.toString() + "\nHeight: " + this.height + "\nOwner: " + this.owner;
    }

    /**
     * This function compares equality of two land objects. It is an overriden function from Object class.
     * @param obj object to compare equality.
     * @return true if equal, false if not.
     */
    @Override
    public boolean equals(Object obj){
        Building objBuf = (Building)obj;
        return super.equals(obj) && this.owner == objBuf.owner;
    }


    /**
     * This function enables to focus on a building.
     * @return Since building is an abstract class, it returns a string that says the building is empty.
     */
    @Override
    public String focus(){
        return "You focus on this building and see it is empty.";
    }
    
    /*
    @Override
    public boolean isEqual(Land obj){
        if(obj instanceof Building && super.isEqual(obj) &&  this.owner == ((Building)obj).owner){
            return true;
        }
        return false;
    }
    */
}