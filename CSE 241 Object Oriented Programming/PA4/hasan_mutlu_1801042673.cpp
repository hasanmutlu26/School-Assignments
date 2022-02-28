#include <iostream>
#include <fstream>
#include <string>

using namespace std;

/*Constant values for file names and gate identifier name size.*/
const char NAMESIZE = 15;
const string CIRCUITFILE = "circuit.txt";
const string INPUTFILE = "input.txt";

/*The base class. Identifier gatename is common in all gate classes.*/
class gate{
    protected:
        string gateName;
    public:
        virtual string getName(){};
        virtual void setName(string){};

        virtual int getOutputValue(){};
        virtual void setOutputValue(int){};
        virtual void setOutputValue(int,int){};

        virtual string getInput(){};
        virtual string getInput(int){};
        virtual void setInput(string){};
        virtual void setInput(string, int){};

        virtual string getOutput(){};
        virtual string getOutput(int){};
        virtual void setOutput(string){};
        virtual void setOutput(string, int){};
        

        virtual gate* getPointer(){};
        virtual void setPointer(gate*){};
        virtual void setPointer(gate*, int){};

        virtual int eval(){};
        virtual int checkType(){};
};

/*Default constructors of all gates set their output values to -1. It means that they aren't evaluated yet.*/
class inputGate : public gate{
    private:
        int outputValue;    /*The output value of gate.*/
    public:
        inputGate(){outputValue = -1;};
        /*Accessor and mutator functions for class's variables.*/
        using gate::setName;
        virtual string getName(){return gateName;};
        virtual void setName(string name){gateName = name;};

        virtual int getOutputValue(){return outputValue;};
        virtual void setOutputValue(int value){outputValue = value;};

        virtual int eval();
        virtual int checkType(){return 0;};     /*checkType function is for checking the type of the gate. It is 0 for this one.*/
};

class outputGate : public gate{
    private:
        int outputValue;    /*The output value of the gate.*/
        gate* ptr;          /*A pointer to which gate it is connected to.*/
    public:
        /*Remaining is same with inputGate class.*/
        outputGate(){outputValue = -1;};

        virtual string getName(){return gateName;};
        virtual void setName(string name){gateName = name;};

        virtual int getOutputValue(){return outputValue;};
        virtual void setOutputValue(int value){outputValue = value;};

        virtual gate* getPointer(){return ptr;};
        virtual void setPointer(gate* obj){ptr = obj;};

        virtual int eval();
        virtual int checkType(){return 1;};     /*output gate's type is 1.*/
};

class andGate : public gate{
    private:
        string input1;      /*input1 and input2 are the names of the gates it is connected to.*/
        string input2;
        int outputValue;
        gate* ptr1;         /*Pointers to the which gates they are connected to.*/
        gate* ptr2;
    public:
        andGate(){outputValue = -1;};

        virtual string getName(){return gateName;};
        virtual void setName(string name){gateName = name;};

        virtual int getOutputValue(){return outputValue;};
        virtual void setOutputValue(int value){outputValue = value;};

        /*This class has 2 input names and 2 pointers. So its  accessor and mutator functions get an index parameter to get/set which one.
        0 for first one and 1 is for second one.*/
        virtual string getInput(int code){
            if(!code) return input1; else return input2;};
        virtual void setInput(string buffer, int code){
            if(!code) input1 = buffer; else input2 = buffer;};

        virtual gate* getPointer(int code){
            if(!code) return ptr1; else return ptr2;};
        virtual void setPointer(gate* obj, int code){
            if(!code) ptr1 = obj; else ptr2 = obj;};

        virtual int eval();
        virtual int checkType(){return 2;};     /*And gate's type is 2.*/
};

/*This class has same properties as and gate.*/
class orGate : public gate{
    private:
        string input1;
        string input2;
        int outputValue;
        gate* ptr1;
        gate* ptr2;
    public:
        orGate(){outputValue = -1;};

        virtual string getName(){return gateName;};
        virtual void setName(string name){gateName = name;};

        virtual int getOutputValue(){return outputValue;};
        virtual void setOutputValue(int value){outputValue = value;};

        virtual string getInput(int code){
            if(!code) return input1; else return input2;};
        virtual void setInput(string buffer, int code){
            if(!code) input1 = buffer; else input2 = buffer;};

        virtual gate* getPointer(int code){
            if(!code) return ptr1; else return ptr2;};
        virtual void setPointer(gate* obj, int code){
            if(!code) ptr1 = obj; else ptr2 = obj;};
        
        virtual int eval();
        virtual int checkType(){return 3;};     /*Or gate's type is 3.*/
};

/*This gate has similar properties with previous ones.*/
class notGate : public gate{
    private:
        string input;
        int outputValue;
        gate* ptr;
    public:
        notGate(){outputValue = -1;};

        virtual string getName(){return gateName;};
        virtual void setName(string name){gateName = name;};

        virtual int getOutputValue(){return outputValue;};
        virtual void setOutputValue(int value){outputValue = value;};

        virtual string getInput(){return input;};
        virtual void setInput(string buffer){input = buffer;};

        virtual gate* getPointer(){return ptr;};
        virtual void setPointer(gate* obj){ptr = obj;};

        virtual int eval();
        virtual int checkType(){return 4;};     /*Not gate's type is 4.*/
};

/*This class has similar properties too except a memory cell. It is set to 0 by default constructor.*/
class flipFlop : public gate{
    private:
        string input;
        int mem;
        int outputValue;
        gate* ptr;
    public:
        flipFlop(){outputValue = -1;mem = 0;};

        virtual string getName(){return gateName;};
        virtual void setName(string name){gateName = name;};

        virtual int getOutputValue(){return outputValue;};
        virtual void setOutputValue(int value){outputValue = value;};

        virtual string getInput(){return input;};
        virtual void setInput(string buffer){input = buffer;};

        virtual gate* getPointer(){return ptr;};
        virtual void setPointer(gate* obj){ptr = obj;};

        virtual int eval();
        virtual int checkType(){return 5;};     /*Flipflop's type is 1.*/
};

class decoder : public gate{
    private:
        string input1;      /*Decoder has 2 input and 4 output strings.*/
        string input2;
        string output1;
        string output2;
        string output3;
        string output4;
        int outputValue1;   /*Decoder has 4 output values.*/
        int outputValue2;
        int outputValue3;
        int outputValue4;
        int checkFlag;      /*checkFlag is to trace which of 4 outputs to return in evaluate. It is set to 0 by constructor.*/
        gate* ptr1;
        gate* ptr2;
    public:
        decoder(){outputValue1 = -1;
                  checkFlag = 0;};

        virtual string getName(){return gateName;};
        virtual void setName(string name){gateName = name;};

        /*Decoder get/setOutputValue and get/setOutput functions take index parameters to decide which one to get/set.
        It takes index from 1 to 4.*/
        virtual int getOutputValue(int code){
            if(code == 1) return outputValue1;
            else if(code == 2) return outputValue2;
            else if(code == 3) return outputValue3;
            else if(code == 4) return outputValue4;};
        virtual void setOutputValue(int value, int code){
            if(code == 1) outputValue1 = value;
            else if(code == 2) outputValue2 = value;
            else if(code == 3) outputValue3 = value;
            else if(code == 4) outputValue4 = value;};

        virtual string getInput(int code){
            if(!code) return input1; else return input2;};
        virtual void setInput(string buffer, int code){
            if(!code) input1 = buffer; else input2 = buffer;};

        virtual string getOutput(int code){
            if(code == 1) return output1;
            else if(code == 2) return output2;
            else if(code == 3) return output3;
            else if(code == 4) return output4;};
        virtual void setOutput(string buffer, int code){
            if(code == 1) output1 = buffer;
            else if(code == 2) output2 = buffer;
            else if(code == 3) output3 = buffer;
            else if(code == 4) output4 = buffer;}

        virtual gate* getPointer(int code){
            if(code) return ptr1; 
            else return ptr2;};
        virtual void setPointer(gate* obj, int code){
            if(!code) ptr1 = obj; 
            else ptr2 = obj;};

        
        virtual int eval();
        virtual int checkType(){return 6;};
};

int number_of_gates();      /*Counts and returns number of gates in circuit file.*/
void readGates(gate** gateList);    /*Reads circuit data into created objects.*/
void connectGates(gate** gateList, int number_of_gates);    /*Connects gates.*/
void printResults(gate** gateList, int num_of_gates);       /*prints results.*/

int main(){
    gate** gateList;    /*An array of gate pointers is created.*/
    int num_of_gates = number_of_gates();   /*number_of_gates function is called and assigned to num_of_gates*/
    gateList = new gate*[num_of_gates];     /*The list of gates as much as num_of_gates is created.*/
    readGates(gateList);                    /*Required functions are called.*/
    connectGates(gateList, num_of_gates);
    printResults(gateList, num_of_gates);

    /*After program is done, the created objects are deleted.*/
    for(int i = 0; i< num_of_gates; i++){
        delete gateList[i];
    }
    delete[] gateList;
}

/*This function counts and returns number of gates in circuit.txt file.*/
int number_of_gates(){
    ifstream fp;        /*ifstream is created and circuit file is opened.*/
    fp.open(CIRCUITFILE);
    string buffer;      /*A buffer to read words into.*/
    int count = 0;      /*Counter to count number of gates.*/

    fp >> buffer;       /*Junk INPUT word is read.*/

    /*Number of input gates are read until encountering OUTPUT keyword.*/
    while(buffer != "OUTPUT"){
        fp >> buffer;
        count++;
    }
    /*The loop counted the OUTPUT word too, so it is decremented by 1.*/
    count--;
    /*Number of output gates are counted until encountering a keyword.*/
    while(  buffer != "AND" &&
            buffer != "OR" &&
            buffer != "NOT" &&
            buffer != "FLIPFLOP" &&
            buffer != "DECODER"  )
    {
        fp >> buffer;
        count++;
    }
    /*Number of keywords are counted until EOF.*/
    while(!fp.eof()){
        fp >> buffer;
        if( buffer == "AND" ||
            buffer == "OR" ||
            buffer == "NOT" ||
            buffer == "FLIPFLOP" ||
            buffer == "DECODER"  )
        {
            count++;
        }
    }
    fp.close();
    return count;   /*Result is returned.*/
}

/*This function reads gate's data from file, creates required objects and assigns the data into objects.
It takes array of gate pointers as parameter.*/
void readGates(gate** gateList){
    string buffer;      /*A buffer to read words into.*/
    int i = 0;          /*An indexing number to advance through gateList.*/
    ifstream fp;        /*ifstream is created and circuit file is opened.*/
    fp.open(CIRCUITFILE);
    /*Until reaching EOF, the first word in each line is read.*/
    for( ; !fp.eof() ; fp >> buffer){
        if(buffer == "INPUT"){      /*If the word read is INPUT..*/
            fp >> buffer;
            while(buffer != "OUTPUT")
            {
                gateList[i] = new inputGate();  /*identifier name for input gate's are read and inputGate objects are created...*/
                gateList[i]->setName(buffer);   /*..until encountering OUTPUT keyword.*/
                i++;
                fp >> buffer;
            }
        }
        
        if(buffer == "OUTPUT"){     /*If the word read is OUTPUT..*/
            fp >> buffer;
            while(  buffer != "AND" &&
                    buffer != "OR" &&
                    buffer != "NOT" &&
                    buffer != "FLIPFLOP" &&
                    buffer != "DECODER"  )
            {
                gateList[i] = new outputGate(); /*identifier name for input gate's are read and inputGate objects are created...*/
                gateList[i]->setName(buffer);   /*..until encountering another keyword.*/
                i++;
                fp >> buffer;
            }
        }
        /*If word read is AND, andGate object is created and its data is read and assigned according to format.*/
        if(buffer == "AND"){
            gateList[i] = new andGate();
            fp >> buffer;
            gateList[i]->setName(buffer);
            fp >> buffer;
            gateList[i]->setInput(buffer,0);
            fp >> buffer;
            gateList[i]->setInput(buffer,1);
            i++;
        }
        /*Works samely for remaining blocks.*/
        else if(buffer == "OR"){
            gateList[i] = new orGate();
            fp >> buffer;
            gateList[i]->setName(buffer);
            fp >> buffer;
            gateList[i]->setInput(buffer,0);
            fp >> buffer;
            gateList[i]->setInput(buffer,1);
            i++;
        }

        else if(buffer == "NOT"){
            gateList[i] = new notGate();
            fp >> buffer;
            gateList[i]->setName(buffer);
            fp >> buffer;
            gateList[i]->setInput(buffer);
            i++;
        }

        else if(buffer == "FLIPFLOP"){
            gateList[i] = new flipFlop();
            fp >> buffer;
            gateList[i]->setName(buffer);
            fp >> buffer;
            gateList[i]->setInput(buffer);
            i++;
        }

        else if(buffer == "DECODER"){
            gateList[i] = new decoder();
            fp >> buffer;
            gateList[i]->setOutput(buffer,1);
            fp >> buffer;
            gateList[i]->setOutput(buffer,2);
            fp >> buffer;
            gateList[i]->setOutput(buffer,3);
            fp >> buffer;
            gateList[i]->setOutput(buffer,4);
            fp >> buffer;
            gateList[i]->setInput(buffer,0);
            fp >> buffer;
            gateList[i]->setInput(buffer,1);
            i++;
        }
    }
    fp.close();
}

/*This function compares input names and identifier gateName's and connects gates
It takes array of gate pointers and num_of_gates as parameters.*/
void connectGates(gate** gateList, int number_of_gates){
    int type;   /*This is for checking types of gates.*/
    /*Whole gates are compared and connected if they match in nested loop.*/ 
    for(int i=0; i< number_of_gates; i++){
        /*gateList[i] gate's type is assigned to type variable.*/
        type = gateList[i]->checkType();
        for(int j=0; j<number_of_gates;j++){
            if(i == j)  /*If i == j, don't compare gates because they are same.*/
                continue;
            /*If ith gate is output gate, its name is compared with jth gate's name.
            If matches, it connects the output gate to regular gate.(gates other than decoder.)*/
            if(type == 1){
                if(gateList[i]->getName() == gateList[j]->getName())
                    gateList[i]->setPointer(gateList[j]);
            }
            /*If ith gate is and/or gate, its inputs are compared with jth gates name with indexed accessor/mutator functions.*/
            else if(type == 2 || type == 3){
                if(gateList[i]->getInput(0) == gateList[j]->getName())
                    gateList[i]->setPointer(gateList[j],0);
                else if(gateList[i]->getInput(1) == gateList[j]->getName())
                    gateList[i]->setPointer(gateList[j],1);
            }
            /*If ith gate is not/flipflop gate, its only input is compared.*/
            else if(type == 4 || type == 5){
                if(gateList[i]->getInput() == gateList[j]->getName())
                    gateList[i]->setPointer(gateList[j]);
            }
            /*If gate is decoder, its all inputs and outputs are compared.*/
            else if(type == 6){
                /*If inputs match, decoder is connected to ith gate.*/
                if(gateList[i]->getInput(0) == gateList[j]->getName())
                    gateList[i]->setPointer(gateList[j],0);
                else if(gateList[i]->getInput(1) == gateList[j]->getName())
                    gateList[i]->setPointer(gateList[j],1);
                /*If outputs match, the jth gate(output gate) is connected to decoder.*/
                else if(gateList[i]->getOutput(1) == gateList[j]->getName())
                    gateList[j]->setPointer(gateList[i]);
                else if(gateList[i]->getOutput(2) == gateList[j]->getName())
                    gateList[j]->setPointer(gateList[i]);
                else if(gateList[i]->getOutput(3) == gateList[j]->getName())
                    gateList[j]->setPointer(gateList[i]);
                else if(gateList[i]->getOutput(4) == gateList[j]->getName())
                    gateList[j]->setPointer(gateList[i]);
            }
        }
    }
}

/*This function reads input bits from input.txt, assigns them to input gates, evaluates output gates and prints to results.*/
/*It takes array of gate pointers and num_of_gates as parameters.*/
void printResults(gate** gateList, int num_of_gates){
    ifstream fp;
    int i,      /*i is for gateList indexing.*/
        buffer,    /*buffer is for reading from input.txt file. It is created as -1. This will be used to indicate if we got to EOF.*/
        num_of_inputs=0;    /*An integer for number of input gates.*/
    
    fp.open(INPUTFILE);     /*input file is opened.*/

    for(i=0;gateList[i]->checkType() == 0;i++)  /*Number of input gates is counted.*/
        num_of_inputs++;
    
    /*A loop until EOF.*/
    while(!fp.eof()){
        buffer = -1;    /*Buffer is set to -1. If there is whitespace at the end of input file, we will check EOF with this.*/
        fp >> buffer;   /*First number in line is read.*/
        if(buffer == -1)    /*If buffer stays as -1, it means that we got to EOF, loop is broken.*/
            break;
        /*input value of first input gate is assigned manually.*/
        gateList[0]->setOutputValue(buffer);
        /*The rest of input values are read and assigned to input gates.*/
        for(i = 1; i< num_of_inputs; i++){
            fp >> buffer;
            gateList[i]->setOutputValue(buffer);
        }
        /*Next gates in list are output gates, their results are evaluated and printed until reaching another type of gate.*/
        while(gateList[i]->checkType() == 1){
            cout << gateList[i]->eval() << " ";
            i++;
        }
        cout << endl;
        /*After one set of input results is printed, output values of all gates are set back to -1*/
        for(i=0;i<num_of_gates;i++){
            if(gateList[i]->checkType() != 6)   /*If gate type isn't decoder, call the function with 1 parameter.*/
                gateList[i]->setOutputValue(-1);
            else                                /*Else, set first output value of decoder to -1.*/
                gateList[i]->setOutputValue(-1,1);
        }
    }
    fp.close();
}

/*eval function of inputGate simply returns its outputvalue. It is the base case of recursive eval function calls.*/
int inputGate::eval(){
    return outputValue;
}

/*eval function of outputGate calls eval function for gate in its pointer and returns the output value.*/
int outputGate::eval(){
    outputValue = this->ptr->eval();
    return outputValue;
}

/*eval function of andGate checks if its outputValue is -1. If it is, it calls eval function for gates in its pointers and makes &&
operation and assigns it to outputValue. If it isn't, it means that this gate is already evaluated and its output is returned without eval.
All gates if(outputValue == -1) check works in this way.*/
int andGate::eval(){
    if(outputValue == -1)
        outputValue = this->ptr1->eval() && this->ptr2->eval();

    return outputValue;
}
/*eval function of orGate does || operation if it hasn't done before.*/
int orGate::eval(){
    if(outputValue == -1)
        outputValue = this->ptr1->eval() || this->ptr2->eval();
    
    return outputValue;
}
/*eval function of notGate reverses input value if it hasn't done before.*/
int notGate::eval(){
    int temp;
    if(outputValue == -1){
        temp = this->ptr->eval();
        if(temp)
            outputValue = 0;
        else    
            outputValue = 1;
    }
        
    return outputValue;
}
/*eval function of flipFlop reverses the memory inside and returns it, if it hasn't done before.*/
int flipFlop::eval(){
    if(outputValue == -1){
        if(this->ptr->eval() == 1){
            if(mem)
                mem = 0;
            else
                mem = 1;
            outputValue = mem;
        }
        else
            outputValue = mem;
    }

    return outputValue;
}
/*eval function of decoder obtains input values and assigns outputValues according to them, if it hasn't evaluated before.*/
int decoder::eval(){
    if(outputValue1 == -1){
        int i1, i2;
        i1 = this->ptr1->eval();
        i2 = this->ptr2->eval();
        if(i1 == 0 && i2 == 0){
            outputValue1 = 1;
            outputValue2 = 0;
            outputValue3 = 0;
            outputValue4 = 0;
        }
        else if(i1 == 0 && i2 == 1){
            outputValue1 = 0;
            outputValue2 = 1;
            outputValue3 = 0;
            outputValue4 = 0;
        }
        else if(i1 == 1 && i2 == 0){
            outputValue1 = 0;
            outputValue2 = 0;
            outputValue3 = 1;
            outputValue4 = 0;
        }else if(i1 == 1 && i2 == 1){
            outputValue1 = 0;
            outputValue2 = 0;
            outputValue3 = 0;
            outputValue4 = 1;
        }
    }
    /*It chooses which of the outputValues to return according to the status of the checkFlag variable.*/
    if(checkFlag == 0){
        checkFlag++;
        return outputValue1;
    }
    else if(checkFlag == 1){
        checkFlag++;        /*checkFlag is incremented every time an output is returned.*/
        return outputValue2;
    }
    else if(checkFlag == 2){
        checkFlag++;
        return outputValue3;
    }
    else if(checkFlag == 3){
        checkFlag = 0;      /*After checkFlag reaches 3, it is set back to 0.*/
        return outputValue4;
    }
}