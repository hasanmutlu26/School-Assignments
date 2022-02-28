#ifndef ENTRY_H
#define ENTRY_H

#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<fstream>

using namespace std;

/*This file contains entry class definition and definition of accessor/mutator functions.*/

/*catalogType is global variable from main.cpp*/
extern int catalogType;

/*There is only one class for any type of catalog.*/
class entry{
    private:
        string title;   /*String for title.*/
        string creator; /*String for author/artist/director.*/
        string year;    /*String for year.*/
        string genre;   /*String for tags/genre.*/
        string starring;    /*String for starring.*/
        int exception;  /*An integer to save exception type. 1: missing field, 2: duplicate entry.*/
    public:
        /*Default constructor creates an empty entry.*/
        entry(){
            title.clear();
            creator.clear();
            year.clear();
            genre.clear();
            starring.clear();
            exception = 0;};

        /*Required accessor and mutator functions.*/
        string getTitle(){return title;};

        string getCreator(){return creator;};

        string getYear(){return year;};

        string getGenre(){return genre;};

        int getException(){return exception;};
        void setException(int exceptionBuffer){exception = exceptionBuffer;};

        string getStarring(){};

        void readEntry(ifstream& fp);
        void printEntry(ofstream& fp);
        bool checkEmpty(){return title.empty();};

        friend void searchAndPrint(ofstream& fp, string word, int searchIndex);
        friend void sortAndPrint(ofstream& fp, int searchIndex);
};

#endif