import java.util.Scanner;

public class Main{
    Street st;
    Scanner buf;
    public static void main(String args[]){
        int select;
        Main m = new Main();
        m.buf = new Scanner(System.in);
        long startTime = System.nanoTime();
        m.driverFunction();
        long stopTime = System.nanoTime();
        System.out.println("Running time of driver function in ArrayList version is " + (stopTime - startTime) + " nanoseconds.\n");
        System.out.print("Enter 1 to continue with interactive program\nEnter another number to exit\nSelection: ");
        while (true){
            try{
                select = m.buf.nextInt();
                if(select == 1){
                    System.out.println("\nWould you like to keep driver function's data or create a new empty street?");
                    System.out.print("Enter 1 to keep existing street\nEnter another number to create new street.\nSelection: ");
                    select = m.buf.nextInt();
                    if(select != 1){
                        m.initialize();
                    }
                    m.mainMenu();
                }
                break;
            }
            catch(Exception inputMismatchException){
                System.out.print("You have entered a wrong input.\nSelect again: ");
                m.buf.nextLine();
            }
        }
        System.out.println("\nHave a nice day!");
        m.buf.close();
    }

    public void driverFunction(){
    System.out.println("HW3 Driver Function");
    System.out.println("Test setting street size.");
    System.out.println("Entered street size: 100");
    st = new Street(100);

    System.out.println("\n------------------------------------------------------------------------------");
    System.out.println("Test Adding Buildings");
    System.out.println("Add Playground");
    System.out.println("Side: Left\nPosition: 0\nLength: 20");
    if(st.add(1, new Playground(0, 20))){
        System.out.println("New Playground is added.");
    }

    System.out.println("\nAdd House");
    System.out.println("Side: Right\nPosition: 0\nLength: 15\nHeight: 20\nOwner: Ali Bey\nNumber of Rooms: 5\nColor: Green");
    if(st.add(2, new House(0, 15, 20, "Ali Bey", 5, "Green"))){
        System.out.println("New House is added.");
    }

    System.out.println("\nAdd Office");
    System.out.println("Side: Left\nPosition: 25\nLength: 25\nHeight: 25\nOwner: Veli Bey\nJob Type: Law Firm");
    if(st.add(1, new Office(25, 25, 25, "Veli Bey", "Law Firm"))){
        System.out.println("New Office is added.");
    }

    System.out.println("\nAdd Market");
    System.out.println("Side: Right\nPosition: 15\nLength: 10\nHeight: 10\nOwner: Zeynep Hanim\nOpening Time: 10\nClosing Time: 21");
    if(st.add(2, new Market(15, 10, 10, "Zeynep Hanim", 10, 21))){
        System.out.println("New Market is added.");
    }

    System.out.println("\nAdd Playground");
    System.out.println("Side: Left\nPosition: 55\nLength: 20");
    if(st.add(1, new Playground(55, 20))){
        System.out.println("New Playground is added.");
    }

    System.out.println("\nAdd House");
    System.out.println("Side: Left\nPosition: 80\nLength: 20\nHeight: 25\nOwner: Ahmet Bey\nNumber of Rooms: 7\nColor: Blue");
    if(st.add(1, new House(80, 20, 25, "Ahmet Bey", 7, "Blue"))){
        System.out.println("New House is added.");
    }

    System.out.println("\nAdd Office");
    System.out.println("Side: Right\nPosition: 35\nLength: 25\nHeight: 40\nOwner: Ayse Hanim\nJob Type: Technology Company");
    if(st.add(2, new Office(35, 25, 40, "Ayse Hanim", "Technology Company"))){
        System.out.println("New Office is added.");
    }

    System.out.println("\nAdd Market");
    System.out.println("Side: Right\nPosition: 65\nLength: 20\nHeight: 15\nOwner: Selim Bey\nOpening Time: 8\nClosing Time: 16");
    if(st.add(2, new Market(65, 20, 15, "Selim Bey", 8, 16))){
        System.out.println("New Market is added.");
    }
    System.out.println("------------------------------------------------------------------------------");


    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test adding to an already occupied place");
    System.out.println("\nAdd Invalid Playground");
    System.out.println("Side: Left\nPosition: 10\nLength: 20");
    if(st.add(1, new Playground(10, 20))){
        System.out.println("New Playground is added.");
    }
    else{
        System.out.println("This place is already occupied.");
    }

    System.out.println("\nAdd Invalid House");
    System.out.println("Side: Left\nPosition: 50\nLength: 20\nHeight: 25\nOwner: Noone\nNumber of Rooms: 7\nColor: Blue");
    if(st.add(2, new House(50, 20, 25, "Noone", 7, "Blue"))){
        System.out.println("New House is added.");
    }
    System.out.println("------------------------------------------------------------------------------");

    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test Adding Building Out of Bounds");
    System.out.println("\nAdd Invalid Playground: Goes beyond the street.");
    System.out.println("Side: Right\nPosition: 85\nLength: 30");
    if(st.add(2, new Playground(85, 30))){
        System.out.println("New Playground is added.");
    }
    else{
        System.out.println("This place is out of bounds.");
    }
    System.out.println("\nAdd Invalid Playground: Starting position is out of the street.");
    System.out.println("Side: Left\nPosition: 110\nLength: 10");
    if(st.add(1, new Playground(110, 10))){
        System.out.println("New Playground is added.");
    }
    else{
        System.out.println("This place is out of bounds.");
    }
    System.out.println("------------------------------------------------------------------------------");

    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test Deleting Building");
    System.out.println(st.listAll());

    System.out.println("\nRemove Playground(3) on left.");
    if(st.delete(1, 3)){
        System.out.println("Building removed.");
    }
    System.out.println("Remove Market(2) on right");
    if(st.delete(2, 2)){
        System.out.println("Building removed.");
    }

    System.out.println("\nAfter deletion:");
    System.out.println(st.listAll());
    System.out.println("------------------------------------------------------------------------------");

    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test deleting non-existing building");
    System.out.println("Remove building with address 5 in right side.");
    if(!st.delete(2, 5)){
        System.out.println("Invalid building address.");
    }
    System.out.println("Remove building with address 0 in left side.");
    if(!st.delete(1, 0)){
        System.out.println("Invalid building address.");
    }
    System.out.println("------------------------------------------------------------------------------");

    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test displaying the total remaining length of lands on the street");
    System.out.println("Total length of the street: " + st.getLength());
    System.out.println("Remaining length on the left side of the street: " + st.getRemainigLeft());
    System.out.println("Remaining length on the right side of the street: " + st.getRemainingRight());
    System.out.println("Total remaining length on the street: " + (st.getRemainigLeft() + st.getRemainingRight()));
    System.out.println("------------------------------------------------------------------------------");

    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test listing all buildings on the street");
    System.out.println("Basic Listing:");
    System.out.println(st.listAll());

    System.out.println("\nDetailed Listing:");
    System.out.println(st.toString());
    System.out.println("------------------------------------------------------------------------------");


    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test displaying the number and ratio of lenth of playgrounds on the street");
    int numOfPlaygrounds = 0;
    int totPlaygroundLength = 0;
    for(int i = 0; i <st.getNumOfObjects(1); i++){
        if(st.getLand(1, i) instanceof Playground){
            numOfPlaygrounds++;
            totPlaygroundLength += st.getLand(1, i).getLength();
        }
    }
    for(int i = 0; i <st.getNumOfObjects(2); i++){
        if(st.getLand(2, i) instanceof Playground){
            numOfPlaygrounds++;
            totPlaygroundLength += st.getLand(2, i).getLength();
        }
    }
    System.out.println("Number of playgrounds on the street: " + numOfPlaygrounds);
    System.out.println("Total length of the playgrounds on the street: " + totPlaygroundLength);
    System.out.println("Ratio of length of playgrounds on the street: " 
    + ((double)totPlaygroundLength / (double)(st.getLength()*2)));
    System.out.println("------------------------------------------------------------------------------");

    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test calculating the total length of street occupied by the markets, houses or offices");
    int totHousesLength = 0;
    int totMarketsLength = 0;
    int totOfficesLength = 0;

    for(int i = 0; i < st.getNumOfObjects(1); i++){
        if(st.getLand(1, i) instanceof House){
            totHousesLength += st.getLand(1, i).getLength();
        }
        else if(st.getLand(1, i) instanceof Office){
            totOfficesLength += st.getLand(1, i).getLength();
        }
        else if(st.getLand(1, i) instanceof Market){
            totMarketsLength += st.getLand(1, i).getLength();
        }
    }
    
    for(int i = 0; i < st.getNumOfObjects(2); i++){
        if(st.getLand(2, i) instanceof House){
            totHousesLength += st.getLand(2, i).getLength();
        }
        else if(st.getLand(2, i) instanceof Office){
            totOfficesLength += st.getLand(2, i).getLength();
        }
        else if(st.getLand(2, i) instanceof Market){
            totMarketsLength += st.getLand(2, i).getLength();
        }
    }

    System.out.println("Total length of houses on the street: " + totHousesLength);
    System.out.println("Total length of offices on the street: " + totOfficesLength);
    System.out.println("Total length of markets on the street: " + totMarketsLength);
    System.out.println("------------------------------------------------------------------------------");

    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test skyline silhouette");
    st.drawSilhouette();

    System.out.println("\n\n------------------------------------------------------------------------------");
    System.out.println("Test focusing on a building");
    System.out.println(st.listAll());
    System.out.println(st.focusOnBuilding(1, 1));
    System.out.println(st.focusOnBuilding(1, 2));
    System.out.println(st.focusOnBuilding(1, 3));
    System.out.println(st.focusOnBuilding(2, 1));
    System.out.println(st.focusOnBuilding(2, 2));
    System.out.println(st.focusOnBuilding(2, 3));
    System.out.println("\nTest with invalid info");
    System.out.println("Building on left with address 0");
    System.out.println(st.focusOnBuilding(1, 0));
    System.out.println("Building on right with address 5");
    System.out.println(st.focusOnBuilding(2, 5));
    System.out.println("------------------------------------------------------------------------------");
    
    
    
    System.out.println("\nDriver function ended.\n");
}

    public void initialize(){
        int length;
        System.out.println("\n\nWelcome to Homework 1");
        System.out.print("Please enter the length of the street(Between 0-150): ");
        length = readInput();
        st = new Street(length);
    }

    public void mainMenu(){
        int selection;
        while(true){
            System.out.println("\nYou have a street with a length of " + st.getLength());
            System.out.println("Select which mode do you want to enter:");
            System.out.println("1. Editing Mode");
            System.out.println("2. Viewing Mode");
            System.out.println("3. Exit");
            System.out.print("Selection: ");
            selection = readInput();
            switch (selection){
                case 1:
                    editMode();
                    break;
                case 2: 
                    viewMode();
                    break;
                case 3:
                    return;
                case 4:
                    //st.silhouette();
                    break;
                case 5:
                    System.out.println("Left: " + st.getRemainigLeft());
                    System.out.println("Right: " + st.getRemainingRight());
                    System.out.println("Total: " + (st.getRemainigLeft() + st.getRemainingRight()));
                    break;
                    
                default:
                    System.out.println("You have entered a wrong input.");
            }
        }
    }

    public void editMode(){
        int selection;
        while(true){
            System.out.println("\nYou have selected edit mode.");
            System.out.println("Select which option do you want to do:");
            System.out.println("1. Add Building");
            System.out.println("2. Delete Building");
            System.out.println("3. Return to the Main Menu");
            System.out.print("Selection: ");
            selection = readInput();
            switch (selection) {
                case 1:
                    addBuilding();
                    break;
                case 2:
                    deleteByAddress();
                    break;
                case 3:
                    return;
                default:
                    System.out.println("You have entered a wrong input.");
            }
        }
    }

    public void addBuilding(){
        int selection;
        while(true){
            System.out.println("\nYou have selected Add Building.");
            System.out.println("Select which kind of building do you want to add:");
            System.out.println("1. Playground");
            System.out.println("2. House");
            System.out.println("3. Office");
            System.out.println("4. Market");
            System.out.println("5. Return to Edit Mode Menu");
            System.out.print("Selection: ");
            selection = readInput();
            switch(selection){
                case 1:
                    long startTime = System.nanoTime();
                    addPlayground();
                    long endTime = System.nanoTime();
                    System.out.println("Running time for adding new Playground is " + (endTime - startTime) + " nanoseconds." );
                    break;
                case 2:
                    addHouse();
                    break;
                case 3:
                    addOffice();
                    break;
                case 4:
                    addMarket();
                    break;
                case 5:
                    return;
                default:
                    System.out.println("You have entered a wrong input.");
            }
        }
    }

    private void addPlayground() {
        int side;
        int position;
        int length;

        System.out.println("\nYou have selected Add Playground");
        System.out.println("Enter which side do you want to add the playground.");
        System.out.println("1. Left");
        System.out.println("2. Right");
        System.out.print("Side select: ");
        side = readInput();
        System.out.print("Enter playground's position: ");
        position = readInput();
        System.out.print("Enter playground's length: ");
        length = readInput();
        if(st.add(side, new Playground(position, length))){
            System.out.println("New playground is added.");
        }
        else{
            System.out.println("New playground could not be added.");
        }
    }

    private void addHouse() {
        int side;
        int position;
        int length;
        int height;
        int numOfRooms;
        String owner;
        String color;

        System.out.println("\nYou have selected Add House");
        System.out.println("Enter which side do you want to add the house.");
        System.out.println("1. Left");
        System.out.println("2. Right");
        System.out.print("Side select: ");
        side = readInput();
        System.out.print("Enter house's position: ");
        position = readInput();
        System.out.print("Enter house's length: ");
        length = readInput();
        System.out.print("Enter house's height: ");
        height = readInput();
        System.out.print("Enter the house owner's name: ");
        buf.nextLine();
        owner = buf.nextLine();
        System.out.print("Enter number of rooms in the house: ");
        numOfRooms = readInput();
        System.out.print("Enter the house's color: ");
        buf.nextLine();
        color = buf.nextLine();
        if(st.add(side, new House(position, length, height, owner, numOfRooms, color))){
            System.out.println("New house is added");
        }
        else{
            System.out.println("New house could not be added");
        }

    }

    private void addOffice() {
        int side;
        int position;
        int length;
        int height;
        String owner;
        String jobType;

        System.out.println("\nYou have selected Add Office");
        System.out.println("Enter which side do you want to add the office.");
        System.out.println("1. Left");
        System.out.println("2. Right");
        System.out.print("Side select: ");
        side = readInput();
        System.out.print("Enter office's position: ");
        position = readInput();
        System.out.print("Enter office's length: ");
        length = readInput();
        System.out.print("Enter office's height: ");
        height = readInput();
        System.out.print("Enter the office owner's name: ");
        buf.nextLine();
        owner = buf.nextLine();
        System.out.print("Enter the job type of the office: ");
        jobType = buf.nextLine();
        if(st.add(side, new Office(position, length, height, owner, jobType))){
            System.out.println("New office is added.");
        }
        else{
            System.out.println("New office could not be added.");
        }
    }

    private void addMarket() {
        int side;
        int position;
        int length;
        int height;
        String owner;
        int openTime;
        int closeTime;
        System.out.println("\nYou have selected Add Market");
        System.out.println("Enter which side do you want to add the market.");
        System.out.println("1. Left");
        System.out.println("2. Right");
        System.out.print("Side select: ");
        side = readInput();
        System.out.print("Enter market's position: ");
        position = readInput();
        System.out.print("Enter market's length: ");
        length = readInput();
        System.out.print("Enter market's height: ");
        height = readInput();
        System.out.print("Enter the market owner's name: ");
        buf.nextLine();
        owner = buf.nextLine();
        System.out.print("Enter the market's opening hour (0-24): ");
        openTime = readInput();
        System.out.print("Enter the market's closing hour (0-24): ");
        closeTime = readInput();

        if(st.add(side, new Market(position, length, height, owner, openTime, closeTime))){
            System.out.println("New market is added.");
        }
        else{
            System.out.println("New market could not be added.");
        }
        
    }

    private void deleteByAddress() {
        int side;
        int address;
        //System.out.println("\nYou have entered delete by address.");
        System.out.println(st.listAll());
        System.out.println("Enter which side do you want to delete the building from.");
        System.out.println("1. Left");
        System.out.println("2. Right");
        System.out.println("3. Return to delete select page");
        System.out.print("Side select: ");
        side = readInput();
        if(side == 3){
            return;
        }
        System.out.print("Enter the address of the building you want to delete from the list: ");
        address = readInput();
        if(st.delete(side, address)){
            System.out.println("The building is deleted.");
        }
        else{
            System.out.println("The building could not be deleted.");
        }
        
    }

    private void viewMode(){
        int selection;
        while(true){
            System.out.println("\nYou have selected view mode.");
            System.out.println("Select which option do you want to do:");
            System.out.println("1. Display the total remaining length of lands on the street.");
            System.out.println("2. Display the list of buildings on the street.");
            System.out.println("3. Display the number and ratio of lenth of playgrounds in the street.");
            System.out.println("4. Calculate the total length of street occupied by the markets, houses or offices.");
            System.out.println("5. Display the skyline silhouette of the street");
            System.out.println("6. Focus on a building.");
            System.out.println("7. Return to the Main Menu");
            System.out.print("Selection: ");
            selection = readInput();
            switch(selection){
                case 1:
                    displayRemaining();
                    break;
                case 2:
                    displayList();
                    break;
                case 3:
                    ratioPlaygrounds();
                    break;
                case 4:
                    lengthBuildings();
                    break;
                case 5:
                    System.out.println("You have selected to display the skyline silhouette of the street");
                    st.drawSilhouette();
                    System.out.print("Enter any number to return: ");
                    readInput();
                    break;
                case 6:
                    focusOnABuilding();
                    break;
                case 7:
                    return;
                default:
                    System.out.println("You have entered a wrong input.");
            }
        }
    }

    private void displayRemaining(){
        System.out.println("\nYou have selected to display the total remaining length of lands on the street");
        System.out.println("Total length of the street: " + st.getLength());
        System.out.println("Remaining length on the left side of the street: " + st.getRemainigLeft());
        System.out.println("Remaining length on the right side of the street: " + st.getRemainingRight());
        System.out.println("Total remaining length on the street: " + (st.getRemainigLeft() + st.getRemainingRight()));
        System.out.print("Enter any number to return: ");
        readInput();
    }

    private void displayList(){
        int selection;
        System.out.println("\nYou have selected to list buildings on the street.");
        System.out.println("Do you want to see a detailed list or a basic list?");
        System.out.print("1. Detailed\n2. Basic\nSelection: ");
        selection = readInput();
        switch(selection){
            case 1:
                System.out.println("Detailed list of the buildings on the street:");
                System.out.println(st.toString());
                break;
            case 2:
                System.out.println("Basic list of the buildings on the street:");
                System.out.println(st.listAll());
                break;
            default:
                System.out.println("You have entered a wrong input. Returning..");
        }
        System.out.print("Enter any number to return: ");
        readInput();
    }

    private void ratioPlaygrounds(){
        int numOfPlaygrounds = 0;
        int totPlaygroundLength = 0;
        for(int i = 0; i <st.getNumOfObjects(1); i++){
            if(st.getLand(1, i) instanceof Playground){
                numOfPlaygrounds++;
                totPlaygroundLength += st.getLand(1, i).getLength();
            }
        }
        for(int i = 0; i <st.getNumOfObjects(2); i++){
            if(st.getLand(2, i) instanceof Playground){
                numOfPlaygrounds++;
                totPlaygroundLength += st.getLand(2, i).getLength();
            }
        }
    
        System.out.println("\nYou have selected to display the number and ratio of lenth of playgrounds in the street.");
        System.out.println("Number of playgrounds on the street: " + numOfPlaygrounds);
        System.out.println("Total length of the playgrounds on the street: " + totPlaygroundLength);
        System.out.println("Ratio of length of playgrounds on the street: " 
        + ((double)totPlaygroundLength / (double)(st.getLength()*2)));
        System.out.print("Enter any number to continue: ");
        readInput();
    
    }

    private void lengthBuildings(){
        int totHousesLength = 0;
    int totMarketsLength = 0;
    int totOfficesLength = 0;

    for(int i = 0; i < st.getNumOfObjects(1); i++){
        if(st.getLand(1, i) instanceof House){
            totHousesLength += st.getLand(1, i).getLength();
        }
        else if(st.getLand(1, i) instanceof Office){
            totOfficesLength += st.getLand(1, i).getLength();
        }
        else if(st.getLand(1, i) instanceof Market){
            totMarketsLength += st.getLand(1, i).getLength();
        }
    }
    
    for(int i = 0; i < st.getNumOfObjects(2); i++){
        if(st.getLand(2, i) instanceof House){
            totHousesLength += st.getLand(2, i).getLength();
        }
        else if(st.getLand(2, i) instanceof Office){
            totOfficesLength += st.getLand(2, i).getLength();
        }
        else if(st.getLand(2, i) instanceof Market){
            totMarketsLength += st.getLand(2, i).getLength();
        }
    }

    System.out.println("\nYou have selected to calculate the total length of street occupied by the markets, houses or offices.");
    System.out.println("Total length of houses on the street: " + totHousesLength);
    System.out.println("Total length of offices on the street: " + totOfficesLength);
    System.out.println("Total length of markets on the street: " + totMarketsLength);
    System.out.print("Enter any number to return: ");
    readInput();

    }

    private void focusOnABuilding(){
        int side;
        int address;
        System.out.println("\nYou have selected to focus on a building.");
        System.out.println(st.listAll());
        System.out.println("Select a building from the list");
        System.out.println("Select the side of the building: ");
        System.out.print("1. Left\n2. Right\nSelection:");
        side = readInput();
        System.out.print("Enter the address of the building: ");
        address = readInput();
        System.out.println(st.focusOnBuilding(side, address));
        System.out.print("Enter a number to continue: ");
        readInput();
    }

    private int readInput(){
        int input;
        try {
            input = buf.nextInt();
            return input;
        } 
        catch (Exception InputMismatchException) {
            buf.nextLine();
            return 0;
        }
    }
}

