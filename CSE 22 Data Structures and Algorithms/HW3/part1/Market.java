public class Market extends Building{
    private int openTime;
    private int closeTime;

    public Market(){
        super();
        this.openTime = 0;
        this.closeTime = 0;
    }

    public Market(int position, int length, int height, String owner, int openTime, int closeTime){
        super(position, length, height, owner);
        this.openTime = openTime;
        this.closeTime = closeTime;
    }
    
    public void setOpenTime(int openTime){
        this.openTime = openTime;
    }

    public int getOpenTime(){
        return this.openTime;
    }

    public void setCloseTime(int closeTime){
        this.closeTime = closeTime;
    }

    public int getCloseTime(){
        return this.closeTime;
    }

    @Override
    public String toString(){
        return "--Market--" + super.toString() + "\nOpening Time: " + this.openTime + "\nClosing Time: " + this.closeTime;
    }


    /**
     * This function enables to focus on a building.
     * @return A string that states the close time of the market.
     */
    @Override
    public String focus(){
        return "You focus on this market and see its closing time is " + this.closeTime;
    }

    /*
    @Override
    public boolean isEqual(Land obj){
        if(obj instanceof Market && super.isEqual(obj) && this.openTime == ((Market)obj).openTime && this.closeTime == ((Market)obj).closeTime){
            return true;
        }
        return false;
    }
    */
}