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
     * This function enables to focus on a building.
     * @return A string that states the length of the playground.
     */
    @Override
    public String focus(){
        return "You focus on this playground and see its length is " + this.length;
    }

}