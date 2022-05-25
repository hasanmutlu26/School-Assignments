import java.util.LinkedList;

public class Street{
    private int length;
    private int remaninigLeft;
    private int remaninigRight;
    private LinkedList<Land> RHS;
    private LinkedList<Land> LHS;

    public Street(){
        this.length = 0;
        this.RHS = new LinkedList<Land>();
        this.LHS = new LinkedList<Land>();
    }

    /**
     * Street constructor.
     * @param length Length of the street to be created.
     */
    public Street(int length){
        if(length > 150){
            System.out.println("The length of the street can't be more than 150!");
            this.length = 150;
        } 
        else{
            this.length = length;
        }
        this.remaninigLeft = length;
        this.remaninigRight = length;
        this.RHS = new LinkedList<Land>();
        this.LHS = new LinkedList<Land>();
    }

    public void setLength(int length){
        this.length = length;
    }

    public int getLength(){
        return this.length;
    }

    public int getRemainigLeft(){
        return this.remaninigLeft;
    }

    public int getRemainingRight(){
        return this.remaninigRight;
    }

    /**
     * This function adds a new building on the street if available.
     * @param side Side of the street the building will be added to. 1 for left and 2 for right.
     * @param obj The land object to be added. 
     * @return True if successful, false if not.
     */
    public boolean add(int side, Land obj){
        if(side != 1 && side != 2){
            System.out.println("Error: The side is invalid");
            return false;
        }

        if(side == 1 && checkAvailability(side, obj.getPosition(), obj.getLength())){
            LHS.add(obj);
            this.remaninigLeft -= obj.getLength();
            return true;
        }
        else if(side == 2 && checkAvailability(side, obj.getPosition(), obj.getLength())){
            RHS.add(obj);
            this.remaninigRight -= obj.getLength();
            return true;
        }
        return false;
    }

    /**
     * This function deletes a building from the street if possible.
     * @param side Side of the street the building will be deleted from. 1 for left and 2 for right side.
     * @param ind The address of the building to be deleted.
     * @return True if deletion is successful, false if not successful.
     */
    public boolean delete(int side, int ind){
        if(side != 1 && side != 2){
            System.out.println("Error: The side is invalid");
            return false;
        }

        if(ind < 1){
            System.out.println("Error: Address info is invalid.");
            return false;
        }

        if(side == 1 && ind <= LHS.size()){
            LHS.remove(ind-1);
            return true;
        }
        else if(side == 2 && ind <= RHS.size()){
            RHS.remove(ind-1);
            return true;
        }
        
        else{
            System.out.println("Error: Address info is invalid.");
            return false;
        }
    }


    /**
     * This function checks if the place on a street is available for a new building.
     * @param side Side of the street to check from. 1 for left and 2 for right.
     * @param position Position of a new building that is checked.
     * @param length Length of a new building that is checked.
     * @return True if available, false if not available or the place is invalid
     */
    private boolean checkAvailability(int side, int position, int length){
        int endpoint = position + length;
        int tempPos;
        int tempEnd;

        if(position > this.length || endpoint > this.length){
            System.out.println("Error: The building is out of bounds.");
            return false;
        }

        if(position < 0 || length < 0){
            System.out.println("Error: Position or lenght can't be minus.");
            return false;
        }

        if(length == 0){
            System.out.println("Error: Length can't be 0.");
            return false;
        }

        if(side == 1){
            for(int i = 0; i < LHS.size(); i++){
                tempPos = LHS.get(i).getPosition();
                tempEnd = tempPos + LHS.get(i).getLength();
                if((position > tempPos && position < tempEnd) || (tempPos > position && tempPos < endpoint)){
                    System.out.println("Error: This position is already occupied.");
                    return false;
                }
            }
            return true;
        }
        if(side == 2){
            for(int i = 0; i < RHS.size(); i++){
                tempPos = RHS.get(i).getPosition();
                tempEnd = tempPos + RHS.get(i).getLength();
                if((position > tempPos && position < tempEnd) || (tempPos > position && tempPos < endpoint)){
                    System.out.println("Error: This position is already occupied.");
                    return false;
                }
            }
            return true;
        }
        return false;
    }
    /**
     * This function returns a list of all the information of the buildings on the street.
     * @return Detailed list of all the buildings.
     */
    @Override
    public String toString(){
        StringBuilder temp = new StringBuilder();
        temp.append("\nLeft side of the street:\n");
        for(int i = 0; i < LHS.size(); i++){
            temp.append(LHS.get(i).toString() + "\n\n");
        }
        temp.append("Right side of the street:\n");
        for(int i = 0; i < RHS.size(); i++){
            temp.append(RHS.get(i).toString() + "\n\n");
        }
        return temp.toString();
    }

    /**
     * This function returns a list of basic information of the buildings on the street.
     * @return Basic list of all the buildings.
     */
    public String listAll(){
        String temp = "";
        temp += "\nLeft side of the street:\n";
        for(int i = 0; i < LHS.size(); i++){
            temp += (i+1) + ": " + LHS.get(i).getClass().getSimpleName() + "\n";
        }
        temp += "\nRight side of the street:\n";
        for(int i = 0; i < RHS.size(); i++){
            temp += (i+1) + ": " + RHS.get(i).getClass().getSimpleName() + "\n";
        }
        return temp;
    }

    /**
     * This function returns the object on the street.
     * @param side Side of the street. 1 for left and 2 for right.
     * @param ind Index of object in the array
     * @return The object. Returns null if parameters are invalid.
     */
    public Land getLand(int side, int ind){
        if(side == 1 && ind < LHS.size()){
            return LHS.get(ind);
        }
        if(side == 2 && ind < RHS.size()){
            return RHS.get(ind);
        }
        return null;
    }

    /**
     * This function returns number of objects in one side of the street.
     * @param side Side of the street. 1 for left and 2 for right. 
     * @return Number of objects in one side of the street. Returns -1 if side is invalid.
     */
    public int getNumOfObjects(int side){
        if(side == 1){
            return LHS.size();
        }
        if(side == 2){
            return RHS.size();
        }
        return -1;
    }

    /**
     * This function draws the skyline silhouette of the street.
     */
    public void drawSilhouette(){
        // Height of tallest building will be noted to not print unnecessary spaces.
        int maxHeight = 0;
        
        /*Two dimensional char array to save silhouette into.*/
        char arr[][] = new char[55][this.length + 1];

        /*Fill array with whitespace.*/
        for(int i = 54; i >= 0; i--){
            for(int j = this.length; j >= 0; j--){
                arr[i][j] = ' ';
            }
        }

        /*In this for loop, walls and roof of buildings on the left are created and added to array.*/
        for(int i = 0; i < LHS.size(); i++){
            //Also, height of tallest building is noted.
            if(LHS.get(i).getHeight() > maxHeight){
                maxHeight = LHS.get(i).getHeight();
            }
            for(int j = LHS.get(i).getPosition(); j < LHS.get(i).getPosition() + LHS.get(i).getLength(); j++){
                arr[LHS.get(i).getHeight()][j] = '*';
            }
            for(int j = LHS.get(i).getHeight(); j > 0; j--){
                arr[j][LHS.get(i).getPosition()] = '*';
            }
            for(int j = LHS.get(i).getHeight(); j > 0; j--){
                arr[j][LHS.get(i).getPosition() + LHS.get(i).getLength()] = '*';
            }
        }

        //In this for loop, walls and roof of buildings on the right are created and added to array.
        for(int i = 0; i < RHS.size(); i++){
            if(RHS.get(i).getHeight() > maxHeight){
                maxHeight = RHS.get(i).getHeight();
            }
            for(int j = RHS.get(i).getPosition(); j < (RHS.get(i).getPosition() + RHS.get(i).getLength()); j++){
                arr[RHS.get(i).getHeight()][j] = '*';
            }
            for(int j = RHS.get(i).getHeight(); j > 0; j--){
                arr[j][RHS.get(i).getPosition()] = '*';
            }
            for(int j = RHS.get(i).getHeight(); j > 0; j--){
                arr[j][RHS.get(i).getPosition() + RHS.get(i).getLength()] = '*';
            }
        }


        //In next two for loops, insides of buildings are filled with whitespace
        //By this way, intersecting areas are cleared.
        for(int i = 0; i < RHS.size(); i++){
            for(int j = RHS.get(i).getPosition() + 1; j < (RHS.get(i).getPosition() + RHS.get(i).getLength()); j++){
                for(int k = RHS.get(i).getHeight() - 1; k > 0; k--){
                    arr[k][j] = ' ';
                }
            }
        }

        for(int i = 0; i < LHS.size(); i++){
            for(int j = LHS.get(i).getPosition() + 1; j < (LHS.get(i).getPosition() + LHS.get(i).getLength()); j++){
                for(int k = LHS.get(i).getHeight() - 1; k > 0; k--){
                    arr[k][j] = ' ';
                }
            }
        }


        //In next 3 for loops, it is checked if two buildings' any walls are in the same position.
        //If so, the overlapping walls are cleared. 
        //It is checked between buildings in left and right; buildings in left; buildings in right in 3 nested for loops
        for(int i = 0; i < RHS.size(); i++){
            for(int j = 0; j < LHS.size(); j++){
                if(RHS.get(i).getPosition() == (LHS.get(j).getPosition() + LHS.get(j).getLength())){
                    int lower;
                    if(RHS.get(i).getHeight() > LHS.get(j).getHeight()){
                        lower = LHS.get(j).getHeight();
                    }
                    else{
                        lower = RHS.get(i).getHeight();
                    }
                    for(int k = 0; k < lower; k++){
                        arr[k][RHS.get(i).getPosition()] = ' ';
                    }
                    break;
                }
                if(LHS.get(j).getPosition() == (RHS.get(i).getPosition() + RHS.get(i).getLength())){
                    int lower;
                    if(LHS.get(j).getHeight() > RHS.get(i).getHeight()){
                        lower = RHS.get(i).getHeight();
                    }
                    else{
                        lower = LHS.get(j).getHeight();
                    }
                    for(int k = 0; k < lower; k++){
                        arr[k][LHS.get(j).getPosition()] = ' ';
                    }
                    break;
                }
            }
        }

        for(int i = 0; i < RHS.size(); i++){
            for(int j = 0; j < RHS.size(); j++){
                if(i == j){
                    continue;
                }
                if(RHS.get(i).getPosition() == (RHS.get(j).getPosition() + RHS.get(j).getLength())){
                    int lower;
                    if(RHS.get(i).getHeight() > RHS.get(j).getHeight()){
                        lower = RHS.get(j).getHeight();
                    }
                    else{
                        lower = RHS.get(i).getHeight();
                    }
                    for(int k = 0; k < lower; k++){
                        arr[k][RHS.get(i).getPosition()] = ' ';
                    }
                    break;
                }
            }
        }

        for(int i = 0; i < LHS.size(); i++){
            for(int j = 0; j < LHS.size(); j++){
                if(i == j){
                    continue;
                }
                if(LHS.get(i).getPosition() == (LHS.get(j).getPosition() + LHS.get(j).getLength())){
                    int lower;
                    if(LHS.get(i).getHeight() > LHS.get(j).getHeight()){
                        lower = LHS.get(j).getHeight();
                    }
                    else{
                        lower = LHS.get(i).getHeight();
                    }
                    for(int k = 0; k < lower; k++){
                        arr[k][LHS.get(i).getPosition()] = ' ';
                    }
                    break;
                }
            }
        }

        //The silhouette is printed.
        for(int i = maxHeight + 2; i > 0; i--){
            for(int j = 0; j <=this.length; j++){
                System.out.print(arr[i][j]);
            }
            System.out.println();
        }

        //Meter measure is printed below.
        System.out.print("0");
        for(int i = 1; i <= this.length; i++){
            if(i % 5 == 0){
                System.out.print(i);
                i += digits(i) - 1;
            }
            else{
                System.out.print("-");
            }
        }
        System.out.println();
    }

    /**
     * This function enables to focus on a building
     * @param side Side of the building to be focused. 1 for left and 2 for right
     * @param ind Address of the building to be focused 
     * @return String that consists of focusing result.
     */
    public String focusOnBuilding(int side, int ind){
        if(side == 1 && ind <= LHS.size() && ind > 0){
            return LHS.get(ind-1).focus();
        }
        if(side == 2 && ind <= RHS.size() && ind > 0){
            return RHS.get(ind-1).focus();
        }
        return "Error: Invalid building info.";
    }

    private int digits(int n){
        int count = 0;
        do {
            n /= 10;
            ++count;
          } while (n != 0);
          return count;
    }


    /*
    public boolean delete(Land obj){
        for(int i = 0; i < LHS.size(); i++){
            if(obj.isEqual(LHS.get(i))){
                LHS.delete(i);
                return true;
            }
        }
        for(int i = 0; i < RHS.size(); i++){
            if(obj.isEqual(RHS.get(i))){
                RHS.delete(i);
                return true;
            }
        }
        return false;
    }
    */

}