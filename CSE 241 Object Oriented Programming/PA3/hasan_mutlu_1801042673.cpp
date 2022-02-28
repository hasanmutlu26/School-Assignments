#include <iostream>
#include <fstream>
#include <vector>

/**********************************************************
NEW FEATURES ADDED IN PA3 AND THEIR ROW NUMBERS IN THIS FILE 
Operator functions declerations: 69. 
Standalone functions declerations: 83.
New main function skeleton: 88.
Standalone functions definitions: 496.
Operator functions definitions: 537. 
There isn't any change in the rest of the function blocks from PA2.
**********************************************************/


/*A small note about commenting: The comments inside "quotation marks" will be the quotations from PDF file.
"You are going to define a class named ppmImage", for example.*/

using namespace std;

/*The default image format for this assignment, P3*/
const string IMAGE_FORMAT = "P3";

/*A struct for pixel. R,G,B are red, green and blue values.*/
typedef struct pixel{
    int R;
    int G;
    int B;
}pixel;

/*ppmImage class.*/
class ppmImage{
    private:
        string imageFormat = IMAGE_FORMAT;  /*Image format of file. P3 is assigned to it.*/

        /*These next values are assigned to 0 initially. We will use this while checking for validity.*/
        int rows = 0;       /*Number of rows in ppm Image.*/
        int columns = 0;    /*Number of columns in ppm Image.*/
        int maxColorValue = 0;  /*Maximum color value of ppm image.*/

        vector<pixel> imageArr; /*A 1D pixel vector to store color data. 
        We will use imageArr[i*columns + j] index operation to reach to desired pixel.*/

    public:
        /*Constructors.*/
        ppmImage(){}    /*Default constructor.*/
        ppmImage(const string source_ppm_file_name);    /*Constructor to create ppmImage by file name.*/
        ppmImage(int rowData, int columnData);      /*Constructor to create ppmImage by row and column data.
                                                    It isn't used in this test scenario.*/
        ppmImage(const ppmImage &oldObject);    /*Constructor to create a copy ppmImage.*/

        void readAndAssign(const string source_ppm_file_name);  /*A member function to read ppm image from a file.*/
        void writeImage(ofstream& fp) const;    /*A member function to save ppm image to a file.*/

        /*Accessor functions.*/
        string getFormat();     /*Returns imageFormat data.*/
        int getRows();          /*Returns rows data.*/
        int getColumns();       /*Returns columns data.*/
        int getmaxColorValue();     /*Returns maxColorValue data.*/
        int getPixel(int i, int j, int color);      /*Returns color data of specific pixel.*/

        /*Mutator functions.*/
        void setFormat(string formatData);  /*Assigns formatData into imageFormat.*/
        void setRows(int rowData);          /*Assigns rowData into rows.*/
        void setColumns(int columnData);    /*Assigns columnData into columns.*/
        void setmaxColorValue(int maxColorValueData);       /*Assigns maxColorValue data into maxColorValue.*/
        void setPixel(int i, int j, int color, int colorData);      /*Assigns colorData into one specific pixel's one color data.*/

        /*PA3 Functions*/
        /*const keyword is not used in argument list because () operator is used in these functions.*/
        friend ppmImage operator+(ppmImage& image1, ppmImage& image2);
        friend ppmImage operator-(ppmImage& image1, ppmImage& image2);
        friend ostream& operator<<(ostream& term, ppmImage& image);
        int& operator()(const int rowData, const int columnData, const int colorData);
};

/*Standalone functions.*/
int read_ppm(const string source_ppm_file_name, ppmImage& destination_object);
int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object);
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice);
ppmImage single_color(const ppmImage& source, int color_choice);

/*PA3 Standalone functions.*/
int test_addition(const string filename_image1, const string filename_image2, const string filename_image3);
int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3);
int test_print(const string filename_image1);

/*Main function. It has the same skeleton as it is stated in PDF.*/
int main(int argc, char** argv){
    ppmImage myImage;   /*ppmImage is created.*/

    if(argv[1][0] == '1'){
        if(argc == 5){
            test_addition(argv[2],argv[3],argv[4]);
        }
    }

    if(argv[1][0] == '2'){
        if(argc == 5){
            test_subtraction(argv[2],argv[3],argv[4]);
        }
    }
    /*This next block is hidden inside comments because it was present in PA2 but not in PA3.*/
    /*If choice number is 2, file is read. If file is valid, swap_channel function is called with required index parameter.
    New data is written into o2.ppm.*/
    /*else if(argv[1][0] == '2'){
        if(read_ppm(argv[2], myImage)){
            swap_channels(myImage, 1);
            write_ppm("o2.ppm", myImage);
        }
    }*/
    /*If choice number is 3, the number of command line arguments is checked. If it is okay, file is read, 
    swap_channel function is called with required index parameter. New data is written into ppm_file_name2.*/
    else if(argv[1][0] == '3'){
        if(argc == 4){
            if(read_ppm(argv[2], myImage)){
                swap_channels(myImage, 2);
                write_ppm(argv[3], myImage);
            }
        }
    }
    /*Same with this block.*/
    else if(argv[1][0] == '4'){
        if(argc == 4){
            if(read_ppm(argv[2], myImage)){
                swap_channels(myImage, 3);
                write_ppm(argv[3], myImage);
            }
        }
    }

    /*If choice number is 5, the number of command line arguments is checked. If it is okay, file is read. 
    If file is valid, a new ppmImage is created. single_color function is called with 
    required index parameter and function returning ppmImage is assigned into newImage. New data is written into ppm_file_name2.*/
    else if(argv[1][0] == '5'){
        if(argc == 4){
            if(read_ppm(argv[2], myImage)){
                ppmImage newImage = single_color(myImage, 1);
                write_ppm(argv[3], newImage);
            }
        }
    }
    /*Same with next 2 blocks.*/
    else if(argv[1][0] == '6'){
        if(argc == 4){
            if(read_ppm(argv[2], myImage)){
                ppmImage newImage = single_color(myImage, 2);
                write_ppm(argv[3], newImage);
            }
        }
    }

    else if(argv[1][0] == '7'){
        if(argc == 4){
            if(read_ppm(argv[2], myImage)){
                ppmImage newImage = single_color(myImage, 3);
                write_ppm(argv[3], newImage);
            }
        }
    }

    /*Else, the command line arguments are invalid. Program exits.*/
    else
        return 0;
}


/************- STANDALONE FUNCTIONS-************/

/* "returns 1 if the operation is successful. otherwise, returns 0.
reads ppm data from file named as source_ppm_file_name. stores data in destination_object. "*/
int read_ppm(const string source_ppm_file_name, ppmImage& destination_object){

    /*Source file name is sent to readAndAssign member function. File is read and assigned there.*/
    destination_object.readAndAssign(source_ppm_file_name);

    /*maxColorValue is used to check validity. If it comes out as 0, it means that there is something wrong.
    0 is returned to indicate unsuccessful read.*/
    if(destination_object.getmaxColorValue() == 0)
        return 0;

    /*Else, the file is read successfully and 1 is returned.*/
    else
        return 1;
}

/* "returns 1 if the operation is successful. otherwise, returns 0.
writes ppm data from source_object to the file named destination_ppm_file_name" */
int write_ppm(const string destination_ppm_file_name, const ppmImage& source_object){

    /*An output file stream is created and opened destination ppm file.
    Then it is sent to writeImage function to save image into file.*/
    ofstream fp(destination_ppm_file_name);
    source_object.writeImage(fp);

    /*After writing is completed, file is closed.*/
    fp.close();
    return 1;
}

/* "this function swaps the color values of every pixel in a given ppm image.
this function does not create a new object but modifies the given one" */
int swap_channels(ppmImage& image_object_to_be_modified, int swap_choice){

    int temp,   /*Holds temporary color data.*/
        color1, /*color1 and color2 will be used as indexing for setPixel and getPixel functions. 1=Red, 2=Green, 3=Blue.*/
        color2;

    /*If swap choice is 1, color1 and color2 are assigned to 1 and 2, to indicate red and green.*/
    if(swap_choice == 1){
        color1 = 1; color2 = 2;
    }
    /*If swap choice is 2, color1 and color2 are assigned to 1 and 3, to indicate red and blue.*/
    else if(swap_choice == 2){
        color1 = 1; color2 = 3;
    }
    /*If swap choice is 3, color1 and color2 are assigned to 2 and 3, to indicate green and blue.*/
    else if(swap_choice == 3){
        color1 = 2; color2 = 3;
    }

    /*All pixels are individually covered in a nested loop. Colors are swapped.*/
    for(int i = 0; i < image_object_to_be_modified.getRows(); i++){
        for(int j = 0; j < image_object_to_be_modified.getColumns(); j++){

            /*color1 value of pixel[i][j] is assigned into temp.*/
            temp = image_object_to_be_modified.getPixel(i,j,color1);

            /*color2 value of pixel[i][j] is obtained from getPixel function(it is 4th parameter), 
            and assigned into color1 value of pixel.*/
            image_object_to_be_modified.setPixel(i,j,color1,image_object_to_be_modified.getPixel(i,j,color2));

            /*color1 value of pixel[i][j], which was saved in temp is assigned to color2 value of pixel.*/
            image_object_to_be_modified.setPixel(i,j,color2,temp);
        }
    }
    return 1;
}

/* "creates and returns a copy of a new ppmImage object which stores only one color at each" */
ppmImage single_color(const ppmImage& source, int color_choice){

    /*A new ppmImage is created and source ppmImage is copied into it via copy constructor.*/
    ppmImage newImage(source);

    int color1, /*Same as the function above, color1 and color2 will be used as indexing for setPixel and getPixel functions.*/
        color2; /*1=Red, 2=Green, 3=Blue.*/

    /*If color choice is 1, color1 and color2 are assigned to 2 and 3, to delete green and blue.*/
    if(color_choice == 1){
        color1 = 2; color2 = 3;
    }
    /*If color choice is 2, color1 and color2 are assigned to 1 and 3, to delete red and blue.*/
    else if(color_choice == 2){
        color1 = 1; color2 = 3;
    }
    /*If color choice is 3, color1 and color2 are assigned to 1 and 2, to delete red and green.*/
    else if(color_choice == 3){
        color1 = 1; color2 = 2;
    }

    /*All pixels are individually covered in a nested loop. color1 and color2 values of the pixel are assigned to 0*/
    for(int i = 0; i < newImage.getRows(); i++){
        for(int j = 0; j < newImage.getColumns(); j++){
            newImage.setPixel(i,j,color1,0);
            newImage.setPixel(i,j,color2,0);
        }
    }
    /*The newly preserved-colored image is returned.*/
    return newImage;
}


/************- CONSTRUCTORS AND MEMBER FUNCTIONS -************/

/* "A constructor which takes a ppm file name as argument and creates object form file."
It isn't used in this program's test scenario. However the upcoming readAndAssign function is used and literally the same with this.*/
ppmImage::ppmImage(const string source_ppm_file_name){

    /*An input file stream is created and source ppm file is opened.*/
    ifstream fp;
    fp.open(source_ppm_file_name,std::ios_base::in);

    int buffer,     /*buffer will be used to read data from file.*/
        i;      /*i will be used as vector indexing and for invalidity checking.*/

    /*The header data is read and assigned into required variables.*/
    fp >> imageFormat;
    fp >> columns;
    fp >> rows;
    fp >> maxColorValue;

    /*A loop until file stream reaches end of file.*/
    for(i=0; fp >> buffer; i++){
        /*A new element is added and three consecutive inputs are assigned into pixels of current element of array.*/
        imageArr.push_back(pixel());
        imageArr[i].R = buffer;
        fp >> buffer;
        imageArr[i].G = buffer;
        fp >> buffer;
        imageArr[i].B = buffer;
    }

    /*If at least one of the following conditions is satisfied, it means that the file is invalid. 
    Every element is set to 0, array is cleared. maxColorValue being 0 will inform read_ppm function about file being invalid.*/
    if(imageFormat != IMAGE_FORMAT || columns <= 0 || rows <= 0 || maxColorValue <= 0){
        imageFormat = '\0'; columns = 0; rows = 0; maxColorValue = 0; imageArr.clear();
    }

    /*If index i came out of loop lower than rows*columns value, it means that there are less pixel information than expected.
    However, this is not invalid. ppm image compiler GIMP filled the remaining missing pixels with black pixels. So will we do.
    The imageArray and output file may not contain literally same data with input file, we will have additional 0's. However, 
    the image representation will stay the same. We do this operation to not confuse our write_ppm function.
    In this loop, we fill the remaining of the image with black pixels by assigning 0.*/
    while(i < rows * columns){
        imageArr.push_back(pixel());
        imageArr[i].R = 0;
        imageArr[i].G = 0;
        imageArr[i].B = 0;
        i++;
    }

    /*File is closed.*/
    fp.close();
}

/* "A constructor which creates an object according to the given dimensions. The image data should be
allocated an initialized in a way to represent a blank (white) image" .
It isn't used in this program's test scenario.*/ 
ppmImage::ppmImage(int rowData, int columnData){

    /*row and column datas are assigned into classes row and column data.*/ 
    rows = rowData;
    columns = columnData;
    imageFormat = IMAGE_FORMAT;     /*imageFormat is set to P3.*/
    maxColorValue = 255;            /*maxColorValue is set to 255 as default.*/

    /*A loop until reaching rows*columns value fills the image array with blank white pixels by assigning 255 to every color.*/
    for(int i=0; i < rows*columns; i++){
        imageArr.push_back(pixel());
        imageArr[i].R = 255;
        imageArr[i].G = 255;
        imageArr[i].B = 255;
    }
}

/*A constructor to create a new copy ppmImage. All data of oldObject are simply assigned into newObject.*/ 
ppmImage::ppmImage(const ppmImage &oldObject){
    imageFormat = oldObject.imageFormat;
    rows = oldObject.rows;
    columns = oldObject.columns;
    maxColorValue = oldObject.maxColorValue;
    imageArr = oldObject.imageArr;
}

/* "A member function to read ppm image from a file."
It is used in this program's scenario. It is literally same with ppmImage(const string source_ppm_file_name) constructor.
Comments are same too.*/
void ppmImage::readAndAssign(const string source_ppm_file_name){

    /*An input file stream is created and source ppm file is opened.*/
    ifstream fp;
    fp.open(source_ppm_file_name,std::ios_base::in);

    int buffer,     /*buffer will be used to read data from file.*/
        i;      /*i will be used as vector indexing and for invalidity checking.*/

    /*The header data is read and assigned into required variables.*/
    fp >> imageFormat;
    fp >> columns;
    fp >> rows;
    fp >> maxColorValue;

    /*A loop until file stream reaches end of file.*/
    for(i=0; fp >> buffer; i++){
        /*A new element is added and three consecutive inputs are assigned into pixels of current element of array.*/
        imageArr.push_back(pixel());
        imageArr[i].R = buffer;
        fp >> buffer;
        imageArr[i].G = buffer;
        fp >> buffer;
        imageArr[i].B = buffer;
    }

    /*If at least one of the following conditions is satisfied, it means that the file is invalid. 
    Every element is set to 0, array is cleared. maxColorValue being 0 will inform read_ppm function about file being invalid.*/
    if(imageFormat != IMAGE_FORMAT || columns <= 0 || rows <= 0 || maxColorValue <= 0){
        imageFormat = '\0'; columns = 0; rows = 0; maxColorValue = 0; imageArr.clear();
    }

    /*If index i came out of loop lower than rows*columns value, it means that there are less pixel information than expected.
    However, this is not invalid. ppm image compiler GIMP filled the remaining missing pixels with black pixels. So will we do.
    The imageArray and output file may not contain literally same data with input file, we will have additional 0's. However, 
    the image representation will stay the same. We do this operation to not confuse our write_ppm function.
    In this loop, we fill the remaining of the image with black pixels by assigning 0.*/
    while(i < rows * columns){
        imageArr.push_back(pixel());
        imageArr[i].R = 0;
        imageArr[i].G = 0;
        imageArr[i].B = 0;
        i++;
    }

    /*File is closed.*/
    fp.close();
}

/* "A member function in order to save ppm image to a file."
This function takes an output file stream as parameter and writes the data of ppmImage into the file.*/
void ppmImage::writeImage(ofstream& fp) const{

    /*The header data is written.*/
    fp << imageFormat << endl;
    fp << columns << " " << rows << endl;
    fp << maxColorValue << endl;
    
    /*All pixel data is written with a nested loop.*/
    for(int i= 0; i < rows; i++){
        for (int j = 0; j < columns; j++){
            fp << imageArr[i*columns + j].R << " ";
            fp << imageArr[i*columns + j].G << " ";
            fp << imageArr[i*columns + j].B << " ";
        }
        fp << endl;
    }
}

/*Next 4 accessor functions simply return the desired variable's value.*/
string ppmImage::getFormat(){
    return imageFormat;
}

int ppmImage::getRows(){
    return rows;
}

int ppmImage::getColumns(){
    return columns;
}

int ppmImage::getmaxColorValue(){
    return maxColorValue;
}

/* "A Member function which returns individual pixel information"
This accessor function takes i and j as row and column indexes and color as color code. 
1 = Red, 2 = Green, 3 = Blue. It returns the desired color value of pixel with i and j indexes in array.*/
int ppmImage::getPixel(int i, int j, int color){
    if(color == 1)
        return imageArr[i*columns + j].R;
    else if(color == 2)
        return imageArr[i*columns + j].G;
    else if(color == 3)
        return imageArr[i*columns + j].B;
}

/*Next 4 mutator functions simply assign incoming parameters into required variable.*/
void ppmImage::setFormat(const string formatData){
    imageFormat = formatData;
}

void ppmImage::setRows(int rowData){
    rows = rowData;
}

void ppmImage::setColumns(int columnData){
    columns = columnData;
}

void ppmImage::setmaxColorValue(int maxColorValueData){
    maxColorValue = maxColorValueData;
}

/* "A member function which changes individual pixel values"
This accessor function takes i and j as row, column indexes and color as same color code and colorData as data to be assigned.
1 = Red, 2 = Green, 3 = Blue. It assigns colorData into desired color value of specific pixel with i and j indexes in array.*/
void ppmImage::setPixel(int i, int j, int color, int colorData){
    if(color == 1){
        imageArr[i*columns + j].R = colorData;
        return;
    }
    else if(color == 2){
        imageArr[i*columns + j].G = colorData;
        return;
    }
    else if(color == 3){
        imageArr[i*columns + j].B = colorData;
        return;
    }
}

/*********************************************/
/*********** New Functions for PA3 ***********/
/*********************************************/

/*Standalone functions*/

/*"reads images from filename_image1 and filename_image2. Adds them and saves the resulting image to filename_image3"*/
int test_addition(const string filename_image1, const string filename_image2, const string filename_image3){
    /*Two ppmImages are created with filename data, one created with default constructor.*/
    ppmImage image1(filename_image1), image2(filename_image2), image3;
    /*image1 and image2 are added and result is assigned into image3.*/
    image3 = image1 + image2;

    /*An output stream is created with filename_image3. ppmImage data is saved into file with << operator. Then file is closed.*/
    ofstream fp(filename_image3);
    fp << image3;
    fp.close();

    return 1;
}

/*"reads images from filename_image1 and filename_image2. Subtracts filename_image2 from filename_image1 
saves the resulting image to filename_image3"*/
int test_subtraction(const string filename_image1, const string filename_image2, const string filename_image3){
    /*Two ppmImages are created with filename data, one created with default constructor.*/
    ppmImage image1(filename_image1), image2(filename_image2), image3;
    /*image2 is substracted by image1 and the result is assigned to image3.*/
    image3 = image1 - image2;

    /*An output stream is created with filename_image3. ppmImage data is saved into file with << operator. Then file is closed.*/
    ofstream fp(filename_image3);
    fp << image3;
    fp.close();

    return 1;
}

/*"reads images from filename_image1 and prints it to stdout" It isn't used in this assignment's test scenario.*/
int test_print(const string filename_image1){
    /*ppmImage is created with filename data. Then it is printed in stdout.*/ 
    ppmImage image(filename_image1);
    cout << image;
    return 1;
}

/*Operator functions.*/

/*Adds two ppmImages and returns the result as ppmImage.
const keyword is not used in argument list because () operator is used in these functions, same for - and << operators too.*/
ppmImage operator+(ppmImage& image1, ppmImage& image2){
    /*If row and column data of image1 and image2 does not match, an empty ppmImage is created and returned.*/
    if(image1.rows != image2.rows || image1.columns != image2.columns){
        ppmImage image3;
        return image3;
    }
    
    /*Else, a new ppmImage object is created with row and column data. it's maxColorValue is also assigned.*/
    ppmImage image3(image1.rows,image1.columns);
    image3.maxColorValue = image1.maxColorValue;
    
    /*In nested loop, RGB values of image1 and image2 are added and assigned to image3 using () operator.*/
    for(int i = 0; i < image3.rows; i++){
        for(int j = 0; j < image3.columns; j++){
            /*Add red color channels and assign.*/
            image3(i,j,1) = image1(i,j,1) + image2(i,j,1);
            /*If result is addition exceeded max color value, it is pulled back to maxColorValue.*/
            if(image3(i,j,1) > image3.maxColorValue)
                image3(i,j,1) = image3.maxColorValue;
            
            /*Same with blue and green channels.*/
            image3(i,j,2) = image1(i,j,2) + image2(i,j,2);
            if(image3(i,j,2) > image3.maxColorValue)
                image3(i,j,2) = image3.maxColorValue;
            
            image3(i,j,3) = image1(i,j,3) + image2(i,j,3);
            if(image3(i,j,3) > image3.maxColorValue)
                image3(i,j,3) = image3.maxColorValue;
        }
    }
    /*The result ppmImage object is returned.*/
    return image3;
}

/*Substracts image2 from image1 and returns the result as ppmImage object.*/
ppmImage operator-(ppmImage& image1, ppmImage& image2){
    /*If row and column data of image1 and image2 does not match, an empty ppmImage is created and returned.*/
    if(image1.rows != image2.rows || image1.columns != image2.columns){
        ppmImage image3;
        return image3;
    }
    
    /*Else, a new ppmImage object is created with row and column data. it's maxColorValue is also assigned.*/
    ppmImage image3(image1.rows,image1.columns);
    image3.maxColorValue = image1.maxColorValue;

    /*In nested loop, results are calculated and assigned to image3 using () operator.*/
    for(int i = 0; i < image3.rows; i++){
        for(int j = 0; j < image3.columns; j++){
            /*Red channel data is subcstracted and assigned.*/
            image3(i,j,1) = image1(i,j,1) - image2(i,j,1);
            /*If result goes below 0, it is pulled back to 0.*/
            if(image3(i,j,1) < 0)
                image3(i,j,1) = 0;

            /*Same with blue and green channels.*/
            image3(i,j,2) = image1(i,j,2) - image2(i,j,2);
            if(image3(i,j,2) < 0)
                image3(i,j,2) = 0;
            
            image3(i,j,3) = image1(i,j,3) - image2(i,j,3);
            if(image3(i,j,3) < 0)
                image3(i,j,3) = 0;
        }
    }
    /*The result ppmImage object is returned.*/
    return image3;
}

/*" Prints image details and image data to stdout". Works similarly to my writeImage member function.
It returns ostream reference to allow cascading.*/
ostream& operator<<(ostream& term, ppmImage& image){
    /*The image data is sent to ofstream term to be printed.*/ 
    term << image.imageFormat << endl;
    term << image.columns << " " << image.rows << endl;
    term << image.maxColorValue << endl;

    for(int i= 0; i < image.rows; i++){
        for (int j = 0; j < image.columns; j++){
            term << image(i,j,1) << " ";
            term << image(i,j,2) << " ";
            term << image(i,j,3) << " ";
        }
        term << endl;
    }
    /*After printing is done, ofstream term is returned to allow cascading.*/ 
    return term;
}

/*"This operator returns a reference to the pixel value". It is a member function of ppmImage class.*/
int& ppmImage::operator()(const int rowData, const int columnData, const int colorData){
    /*Depending on the incoming color channel data, it returns the calling object's required color channel's data as reference.*/ 
    if(colorData == 1)
        return imageArr[rowData * columns + columnData].R;
    else if(colorData == 2)
        return imageArr[rowData * columns + columnData].G;
    else if(colorData == 3)
        return imageArr[rowData * columns + columnData].B;
}

