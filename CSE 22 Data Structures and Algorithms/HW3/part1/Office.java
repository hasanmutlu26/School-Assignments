public class Office extends Building{
    private String jobType;

    public Office(){
        super();
        this.jobType = "Unknown";
    }

    public Office(int position, int length, int height, String owner, String jobType){
        super(position, length, height, owner);
        this.jobType = jobType;
    }

    public void setJobType(String jobType){
        this.jobType = jobType;
    }

    public String getJobType(){
        return this.jobType;
    }

    @Override
    public String toString(){
        return "--Office--" + super.toString() + "\nJob Type: " + this.jobType;
    }

    /**
     * This function enables to focus on a building.
     * @return A string that states job type of the office. 
     */
    @Override
    public String focus(){
        return "You focus on this office and see its job type is " + this.jobType; 
    }
/*
    @Override
    public boolean isEqual(Land obj){
        if(obj instanceof Office && super.isEqual(obj) && this.jobType == ((Office)obj).jobType){
            return true;
        }
        return false;
    }
    */
}