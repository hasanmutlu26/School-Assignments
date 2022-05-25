public class Playground extends Land{
    public Playground(){
        super();
        this.height = 2; 
    }

    /**
     * Default height of a playground is 2.
     * @param position
     * @param length
     */
    public Playground(int position, int length){
        super(position, length, 2);
    }

    @Override
    public String toString(){
        return "--Playground--" + super.toString() + "\nDefault height: " + height;
    }

    /**
     * This function compares equality of two land objects. It is an overriden function from Object class.
     * @param obj object to compare equality.
     * @return true if equal, false if not.
     */
    @Override
    public boolean equals(Object obj){
        if(obj.getClass().getSimpleName() == "Playground"){
            return super.equals(obj);
        }
        return false;
    }

    /**
     * This function enables to focus on a building.
     * @return A string that states the length of the playground.
     */
    @Override
    public String focus(){
        return "You focus on this playground and see its length is " + this.length;
    }

}