
public abstract class Land{
    protected int position;
    protected int length;
    protected int height;

    /**
     * Default constructor. Default position, length and height properties are 0.
     */
    public Land(){
        this.position = 0;
        this.length = 0;
        this.height = 0;
    }

    /**
     * @param position Position
     * @param length Length
     */
    public Land(int position, int length, int height){
        this.position = position;
        this.length = length;
        if(height > 50){
            System.out.println("Maximum height of a building can be 50.");
            this.height = 50;
        }
        else{
            this.height = height;
        }
    }

    public void setPosition(int position){
        this.position = position;
    }

    public int getPosition(){
        return this.position;
    }

    public void setLength(int length){
        this.length = length;
    }

    public int getLength(){
        return this.length;
    }

    public void setHeight(int height){
        this.height = height;
    }

    public int getHeight(){
        return this.height;
    }

    @Override
    public String toString(){
        return "\nPosition: " + this.position + "\nLength: " + this.length; 
    }

    /**
     * This function enables to focus on the land
     * @return Since the land is an abstract class, it returns a string that says the land is empty.
     */
    public String focus(){
        return "You focus on this land and see it is empty.";
    }

    
    /*
    public boolean isEqual(Land obj){
        if(this.position == obj.position && this.height == obj.height && this.length == obj.length){
            return true;
        }
        return false;
    }
    */
}