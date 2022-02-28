#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<fstream>
#include"entry.h"

/*This file contains main function and standalone functions.*/

using namespace std;

/*Filenames.*/
const string DATA = "data.txt";
const string COMMANDS = "commands.txt";
const string OUTPUT = "output.txt";

/*A struct to read commans into.*/
struct cmd{
    string command; /*command type, either search or sort.*/
    string word;    /*The word to be searched.*/
    string field;   /*The field in command.*/
};

vector<entry> list;     /*A vector of entries.*/
int catalogType;        /*Type of the catalog. 1: Book, 2: Music, 3: Movie.*/

void readData();
void duplicateCheck();
void createOutput();
void catalogRead(ofstream& fp);
void readCommands(ofstream& fp);
void searchAndPrint(ofstream& fp, string word, int searchIndex);
void sortAndPrint(ofstream& fp, int sortIndex);

/*In main function, data is read, duplicates are checked and output is created.*/
int main(){
    readData();
    duplicateCheck();
    createOutput();
}

/*This function opens data.txt file and reads the data in it.*/
void readData(){
    ifstream fp;    /*input file stream.*/
    fp.open(DATA);
    string sBuffer; /*A string buffer.*/
    char cBuffer;   /*A character buffer.*/

    /*Catalog type word is read and catalogType is assigned according to it.*/
    fp >> sBuffer;
    if(sBuffer == "book")   /*1 is for book.*/
        catalogType = 1;
    else if(sBuffer == "music") /*2 is for music.*/
        catalogType = 2;
    else if(sBuffer == "movie") /*3 is for movie.*/
        catalogType = 3;

    /*A loop until end of file.*/
    while(1){
        /*fp is moved to next line.*/
        do{
            fp.get(cBuffer);
        }while(cBuffer != '\n' && !fp.eof());

        /*if fp reached end of file, end the loop.*/
        if(fp.eof())
            break;
        
        /*A new entry created. entry::readEntry function is called to read whole line into the newEntry. 
        Then, the newEntry is added to list if it is not created empty.*/
        entry newEntry;
        newEntry.readEntry(fp);
        if(!newEntry.checkEmpty())
            list.push_back(newEntry);
    }
    fp.close();
}

/*This function checks if there are duplicate entries and marks their exception.*/ 
void duplicateCheck(){
    vector<entry>::iterator i;  /*Iterators.*/
    vector<entry>::iterator j;

    /*In a nested loop, entries' titles are compared and if matched, the entry which is added to list later is marked as
    duplicate entry exception(2). (If it doesn't also ghave missing field.)*/
    for(i = list.begin(); i < list.end(); i++){
        for(j = list.begin(); j < i; j++){
            if(i->getTitle() == j->getTitle() && i->getException() != 1){
                i->setException(2);
            }
        }
    }
}

/*This function opens output.txt file and calls required functions to created output.*/
void createOutput(){
    ofstream fp;
    fp.open(OUTPUT);
    
    catalogRead(fp);
    readCommands(fp);

    fp.close();
}

/*This function prints catalog read part of the output. It takes an ofstream to output.txt*/
void catalogRead(ofstream& fp){
    vector<entry>::iterator i;
    int numOfUnique;

    /*Catalog Read: line is printed according to catalogType.*/
    if(catalogType == 1)
        fp << "Catalog Read: book\n";
    else if(catalogType == 2)
        fp << "Catalog Read: music\n";
    else if(catalogType == 3)
        fp << "Catalog Read: movie\n";

    /*In a loop, the entries which has exception are printed.*/ 
    for(i = list.begin(); i < list.end(); i++){
        /*If an entry has exception, it is printed and then, removed from the list.*/
        if(i->getException() == 1){
            fp << "Exception: missing field\n";
            i->printEntry(fp);
            list.erase(i);
            i--;
        }
        else if(i->getException() == 2){
            fp << "Exception: duplicate entry\n";
            i->printEntry(fp);
            list.erase(i);
            i--;
        }
    }
    /*At the end, the number of remaining entries in the list, number of unique entries is printed.*/
    numOfUnique = list.size();
    fp << numOfUnique << " unique entries\n";
}

/*This function reads commands.txt file and execute them.*/
void readCommands(ofstream& fp){
    ifstream cp;    /*An input file stream to read commands.txt.*/
    char cBuffer;   /*A character buffer.*/
    string sBuffer; /*A string buffer.*/
    struct cmd c;   /*A command struct to read command strings into.*/
    int funcIndex = 0;  /*An integer to send to search or sort functions as index.*/

    cp.open(COMMANDS);

    /*A loop until end of file.*/
    while(1){
        /*There are a lot of exception about command format.*/
        try{
            /*Type of command is read.*/
            cp >> c.command;
            /*If command is search, read next words.*/
            if(c.command == "search"){
                /*Read the word.*/
                cp >> c.word;
                /*If word isn't written inside " ", throw exception code 2.*/  
                if(c.word[0] != '"' || c.word[c.word.size()-1] != '"')
                    throw 2;
                
                /*Read "in" word. If it isn't "in", throw exception code 3.*/
                cp >> sBuffer;
                if(sBuffer != "in")
                    throw 3;

                /*Read field.*/
                cp >> c.field;
                /*According to the field, it is checked if it is correct and funcIndex is assigned.*/
                if(c.field == "\"title\"")
                    funcIndex = 1;
                else if((catalogType == 1 && c.field == "\"authors\"")  ||
                        (catalogType == 2 && c.field == "\"artist\"")   ||
                        (catalogType == 3 && c.field == "\"director\""))
                    funcIndex = 2;
                else if(c.field == "\"year\"")
                    funcIndex = 3;
                else if((catalogType == 1 && c.field == "\"tags\"") || c.field == "\"genre\"")
                    funcIndex = 4;
                else if(catalogType == 3 && c.field == "\"starring\"")
                    funcIndex = 5;
                /*If field doesn't satisfy anything above, it is wrong. Throw exception code 4.*/
                else
                    throw 4;

                /*The "" characters at the beginnig and end of word string are deleted.*/
                c.word.erase(c.word.end()-1); c.word.erase(c.word.begin());
                sBuffer.clear();

                /*The remaining of the line is read into sBuffer.*/
                getline(cp,sBuffer);
                /*If sBuffer size exceeds 1, it means that there are extra information(or even space) in the command. 
                Throw exception code 5.*/
                if(sBuffer.size() > 1)
                    throw 5;
                    
                /*If program comes to this point, the command is correct. The command is printed into file and 
                searchAndPrint function is called.*/
                fp << c.command << " \"" << c.word << "\" in " << c.field << endl; 
                searchAndPrint(fp, c.word, funcIndex);
            }

            /*If command is sort, read field information.*/
            else if(c.command == "sort"){
                cp >> c.field;

                /*According to the field, it is checked if it is correct and funcIndex is assigned.*/
                if(c.field == "\"title\"")
                    funcIndex = 1;
                else if((catalogType == 1 && c.field == "\"authors\"")  ||
                        (catalogType == 2 && c.field == "\"artist\"")   ||
                        (catalogType == 3 && c.field == "\"director\""))
                    funcIndex = 2;
                else if(c.field == "\"year\"")
                    funcIndex = 3;
                else if((catalogType == 1 && c.field == "\"tags\"") || c.field == "\"genre\"")
                    funcIndex = 4;
                else if(catalogType == 3 && c.field == "\"starring\"")
                    funcIndex = 5;
                /*If field doesn't satisfy anything above, it is wrong. Throw exception code 6.*/
                else
                    throw 6;
                
                /*The command is printed and sortAndPrint function is called.*/
                fp << c.command << " " << c.field << endl;
                sortAndPrint(fp, funcIndex);
            }

            /*If command name isnt search or sort, command is wrong. Throw exception code 1.*/
            else
                throw 1;
        }

        /*Catch errors and print the wrong command correctly.*/
        catch(int errortype){
            fp << "Exception: command is wrong\n";

            /*If 1, command name is wrong. Print the command and the remaining or the line.*/
            if(errortype == 1){
                getline(cp,sBuffer);
                fp << c.command << sBuffer << "\n";
            }

            /*If 2, word format is wrong, print the command, word and remaining of the line.*/
            else if(errortype == 2){
                fp << c.command << " " << c.word;
                getline(cp,sBuffer);
                fp << sBuffer << endl;
            }

            /*If 3, third word in command isn't "in", print the command so far and the remaining of the line.*/
            else if(errortype == 3){
                fp << c.command << " " << c.word << " " << sBuffer;
                getline(cp,sBuffer);
                fp << sBuffer << endl;
            }

            /*If 4, field is wrong, print the command so far and the remaining of the line.*/
            else if(errortype == 4){
                fp << c.command << " " << c.word << " in " << c.field;
                getline(cp,sBuffer);
                fp << sBuffer << endl;
            }

            /*If 5, there is extra information in command line. Print the whole line.*/
            else if(errortype == 5){
                fp << c.command << " " << c.word << " in " << c.field << sBuffer;
            }

            /*If 6, field of sort command is wrong, print the command, field and remaining of the line.*/
            else if(errortype == 6){
                fp << c.command << " " << c.field;
                getline(cp,sBuffer);
                fp << sBuffer << endl;
            }
        }

        /*If end of file is reached, break the loop.*/
        if(cp.eof())
            break;
    }
}

/*This function executes search command. It takes an ofstream to output.txt, a string to search for and searcIndex to
determine in which field we will make search.*/ 
void searchAndPrint(ofstream& fp, string word, int searchIndex){
    vector<entry>::iterator i;  /*An iterator to go through the list.*/
    size_t found;   /*A size_t variable to assign the return value of string::find(string) function.*/

    /*If searchIndex is 1, search in title.*/
    if(searchIndex == 1){
        for(i = list.begin(); i < list.end(); i++){
            found = i->title.find(word);
            if(found != std::string::npos){ /*If found != std::string::npos, it means that it is found.*/
                i->printEntry(fp);          /*Current entry is printed and loop is ended.*/
                break;
            }    
        }
    }

    /*If searchIndex is 1, search in author/artist/director.*/
    else if(searchIndex == 2){
        for(i = list.begin(); i < list.end(); i++){
            found = i->creator.find(word);
            if(found != std::string::npos){
                i->printEntry(fp);   
                break;
            }    
        }
    }

    /*If searchIndex is 1, search in year.*/
    else if(searchIndex == 3){
        for(i = list.begin(); i < list.end(); i++){
            found = i->year.find(word);
            if(found != std::string::npos){
                i->printEntry(fp);   
                break;
            }    
        }
    }

    /*If searchIndex is 4, search in tags/genre.*/
    else if(searchIndex == 4){
        for(i = list.begin(); i < list.end(); i++){
            found = i->genre.find(word);
            if(found != std::string::npos){
                i->printEntry(fp);   
                break;
            }    
        }
    }

    /*If searchIndex is 5, search in starring.*/
    else if(searchIndex == 5){
        for(i = list.begin(); i < list.end(); i++){
            found = i->starring.find(word);
            if(found != std::string::npos){
                i->printEntry(fp);   
                break;
            }    
        }
    }
}

/*This function sorts the list and prints it. It takes an ofstream to output.txt and a sortIndex to determine which field 
will be sorted.*/
void sortAndPrint(ofstream& fp, int sortIndex){
    vector<entry>::iterator i;

    /*If sortIndex is 1, sort according to title and print the list.*/
    if(sortIndex == 1){
        sort(list.begin(), list.end(), [](const entry& obj1, const entry& obj2){
            return obj1.title < obj2.title;});
            for(i = list.begin(); i < list.end(); i++){
                if(!i->title.empty()){
                    i->printEntry(fp);
                }
            }
    }

    /*If sortIndex is 2, sort according to author/artist/director and print the list.*/
    if(sortIndex == 2){
        sort(list.begin(), list.end(), [](const entry& obj1, const entry& obj2){
            return obj1.creator < obj2.creator;});
            for(i = list.begin(); i < list.end(); i++){
                if(!i->creator.empty()){
                    i->printEntry(fp);
                }
            }
    }

    /*If sortIndex is 3, sort according to year and print the list.*/
    if(sortIndex == 3){
        sort(list.begin(), list.end(), [](const entry& obj1, const entry& obj2){
            return obj1.year < obj2.year;});
            for(i = list.begin(); i < list.end(); i++){
                if(!i->year.empty()){
                    i->printEntry(fp);
                }
            }
    }

    /*If sortIndex is 4, sort according to tags/genre and print the list.*/
    if(sortIndex == 4){
        sort(list.begin(), list.end(), [](const entry& obj1, const entry& obj2){
            return obj1.genre < obj2.genre;});
            for(i = list.begin(); i < list.end(); i++){
                if(!i->genre.empty()){
                    i->printEntry(fp);
                }
            }
    }

    /*If sortIndex is 5, sort according to starring and print the list.*/
    if(sortIndex == 5){
        sort(list.begin(), list.end(), [](const entry& obj1, const entry& obj2){
            return obj1.starring < obj2.starring;});
            for(i = list.begin(); i < list.end(); i++){
                if(!i->starring.empty()){
                    i->printEntry(fp);
                }
            }
    }
}