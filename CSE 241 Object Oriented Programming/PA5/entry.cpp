#include"entry.h"

/*This file contains member functions of entry class.*/

/*This function reads the data.txt file and assigns the information into the calling entry. It takes an ifstream to data.txt.*/
void entry::readEntry(ifstream& fp){
    char cBuffer;   /*A character buffer.*/
    string sBuffer; /*A string buffer.*/

    /*If there is missing field exception, it will be thrown.*/
    try{
        /*fp is moved one step ahead.*/
        fp.get(cBuffer);
        /*If end of file is reached, return and end the function.*/
        if(fp.eof())
            return;

        /*Get characters from file one by one and push_back them in string buffer until reaching " */
        do{
            fp.get(cBuffer);
            sBuffer.push_back(cBuffer);
        }while(cBuffer != '"');
        sBuffer.pop_back();     /*The ending " is also pushed back, pop back it.*/
        /*Assign the sBuffer into title and clear it.*/
        title = sBuffer;
        sBuffer.clear();

        /*Move to next " location by getting chars into cBuffer one by one.*/
        do{
            fp.get(cBuffer);
        }while(cBuffer != '"' && cBuffer != '\n' && !fp.eof());
        /*If cBuffer is newline or end of file, it means that there is missing field, throw exception.*/
        if(cBuffer == '\n' || fp.eof())
            throw 1;

        /*Same process is repeated for author/artist/director and year and tags/genre.*/
        do{
            fp.get(cBuffer);
            sBuffer.push_back(cBuffer);
        }while(cBuffer != '"');
        sBuffer.pop_back();
        creator = sBuffer;
        sBuffer.clear();

        do{
            fp.get(cBuffer);
        }while(cBuffer != '"' && cBuffer != '\n' && !fp.eof());
        if(cBuffer == '\n' || fp.eof())
            throw 1;

        
        do{
            fp.get(cBuffer);
            sBuffer.push_back(cBuffer);
        }while(cBuffer != '"');
        sBuffer.pop_back();
        year = sBuffer;
        sBuffer.clear();
        
        do{
            fp.get(cBuffer);
        }while(cBuffer != '"' && cBuffer != '\n' && !fp.eof());
        if(cBuffer == '\n' || fp.eof())
            throw 1;
        

        do{
            fp.get(cBuffer);
            sBuffer.push_back(cBuffer);
        }while(cBuffer != '"');
        sBuffer.pop_back();
        genre = sBuffer;
        sBuffer.clear();

        /*If catalogType is 3, it means that the catalog is movie catalog. Read starrings also.*/
        if(catalogType == 3){
            do{
            fp.get(cBuffer);
            }while(cBuffer != '"' && cBuffer != '\n' && !fp.eof());
            if(cBuffer == '\n' || fp.eof())
                throw 1;
            

            do{
                fp.get(cBuffer);
                sBuffer.push_back(cBuffer);
            }while(cBuffer != '"');
            sBuffer.pop_back();
            starring = sBuffer;
        }
    }

    /*If an exception is thrown, assign 1 to exception.*/
    catch(int){
        exception = 1;
    }
}

/*This function prints the entry information into file. It takes an ofstream to output.txt.*/
void entry::printEntry(ofstream& fp){
    /*Title is printed.*/
    fp << "\"" << title << "\"";

    /*Creator is printed if it isn't empty.*/
    if(!creator.empty())
        fp << " \"" << creator << "\"";
    /*If it is empty, a "" is printed instead. Same for the remaining fields.*/
    else
        fp << " \"\"";
    if(!year.empty())
        fp << " \"" << year << "\"";
    else
        fp << " \"\"";
    if(!genre.empty())
        fp << " \"" << genre << "\"";

    /*If catalog type isn't movie and there is missing field exception, don't print the following as it is missing for sure.*/
    else if (catalogType != 3 && exception != 1)
        fp << " \"\"";

    /*If it is movie catalog, starring is printed also (if there isn't missing field).*/
    if(catalogType == 3){
        if(!starring.empty())
            fp << " \"" << starring << "\"";
        else if(exception != 1)
            fp << " \"\"";
    }

    fp << "\n";
}