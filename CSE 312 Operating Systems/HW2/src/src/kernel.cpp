
#include <common/types.h>
#include <gdt.h>
#include <memorymanagement.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/vga.h>
#include <gui/desktop.h>
#include <gui/window.h>
#include <multitasking.h>



// #define GRAPHICSMODE
#define MAX 10


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;



void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }
        
        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}



void printint(int N)
{
    int arr[MAX];
    int i = 0;
    int j, r;
    if(N < 0){
        printf("-");
        N = N*(-1);
    }
    if(N == 0){
        printf("0");
    }
    while (N != 0) {
        r = N % 10;
        arr[i] = r;
        i++;
        N = N / 10;
    }
    for (j = i - 1; j > -1; j--) {
        switch (arr[j])
        {
        case 0:
            printf("0");
            break;
        case 1:
            printf("1");
            break;
        case 2:
            printf("2");
            break;
        case 3:
            printf("3");
            break;
        case 4:
            printf("4");
            break;
        case 5:
            printf("5");
            break;
        case 6:
            printf("6");
            break;
        case 7:
            printf("7");
            break;
        case 8:
            printf("8");
            break;
        case 9:
            printf("9");
            break;
        
        default:
            break;
        }
    }
}

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}




class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:
    
    MouseToConsole()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }
    
};





void taskA()
{
    while(true)
        printf("A");
}
void taskB()
{
    while(true)
        printf("B");
}






typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

class pageReplacement{
    public:
        virtual int get(int){}
        virtual void set(int,int){}
};

void swap(pageReplacement* arr, int xp, int yp){
    int temp = arr->get(xp);
    arr->set(xp, arr->get(yp));
    arr->set(yp, temp);
}
 
void bubbleSort(pageReplacement* arr, int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - i - 1; j++)
            if (arr->get(j) > arr->get(j + 1))
                swap(arr, j, j+1);
}

int partition (pageReplacement* arr, int low, int high)
{
    int pivot = arr->get(high);
    int i = (low - 1);
 
    for (int j = low; j <= high - 1; j++)
    {
        if (arr->get(j) < pivot)
        {
            i++;
            swap(arr, i, j);
        }
    }
    swap(arr, i+1, high);
    return (i + 1);
}
 
void quickSort(pageReplacement* arr, int low, int high){
    if (low < high){
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void insertionSort(pageReplacement* arr, int n)
{
    int i, key, j;
    for (i = 1; i < n; i++) {
        key = arr->get(i);
        j = i - 1;
        while (j >= 0 && arr->get(j) > key) {
            arr->set(j+1, arr->get(j));
            j = j - 1;
        }
        arr->set(j+1, key);
    }
}



#define INTPERPAGE 10
#define PAGES 10
#define ARRSIZE 500

class LRU : public pageReplacement{
    private:
        int numArray[ARRSIZE];
        int pages[PAGES];
        int refTime[PAGES];
        int clock;
        int hits;
        int misses;
    public:
        LRU();
        int get(int i);
        void set(int i, int val);
        int getHits(){return hits;}
        int getMisses(){return misses;}
        int getArrSize(){return ARRSIZE;}
};


LRU::LRU(){
    clock = 0;
    hits = 0;
    misses = 0;
    for(int i = 0; i < ARRSIZE; i++){
        int t = i;
        if(i % 2){
            t = i*(-1);
        }
        numArray[i] = t;
    }

    for(int i = 0; i < PAGES; i++){
        pages[i] = -1;
        refTime[i] = clock;
    }
}

int LRU::get(int i){
    clock++;

    int oldest = 0;
    for(int j = 0; j < PAGES; j++){
        if(i >= pages[j]*INTPERPAGE && i <= pages[j]*INTPERPAGE+INTPERPAGE-1){
            hits++;
            refTime[j] = clock;
            return numArray[i];
        }
        if(refTime[j] < refTime[oldest]){
            oldest = j;
        }
    }

    int newPage;
    for(int j = 0; j < ARRSIZE; j+=INTPERPAGE){
        if(i >= j && i <= j+INTPERPAGE - 1){
            newPage = j / INTPERPAGE;
            break;
        }
    }
    /*printf("MISS: "); printint(i); printf(" - ");
    for(int j = 0; j < PAGES; j++){
        printint(pages[j]); printf(" ");
    }
    printf("\n");*/
    pages[oldest] = newPage;
    refTime[oldest] = clock;
    misses++;
    
    return numArray[i];
}

void LRU::set(int i, int val){
    clock++;
    
    int oldest = 0;
    for(int j = 0; j < PAGES; j++){
        if(i >= pages[j]*INTPERPAGE && i <= pages[j]*INTPERPAGE+INTPERPAGE-1){
            hits++;
            refTime[j] = clock;
            numArray[i] = val;
            return;
        }
        if(refTime[j] < refTime[oldest]){
            oldest = j;
        }
    }

    int newPage;
    for(int j = 0; j < ARRSIZE; j+=INTPERPAGE){
        if(i >= j && i <= j+INTPERPAGE - 1){
            newPage = j / INTPERPAGE;
            break;
        }
    }

    pages[oldest] = newPage;
    refTime[oldest] = clock;
    misses++;
    numArray[i] = val;
}



class FIFO : public pageReplacement{
    private:
        int numArray[ARRSIZE];
        int pages[PAGES];
        int hits;
        int misses;
    public:
        FIFO();
        int get(int i);
        void set(int i, int val);
        int getHits(){return hits;}
        int getMisses(){return misses;}
        int getArrSize(){return ARRSIZE;}
};

FIFO::FIFO(){
    hits = 0;
    misses = 0;

    for(int i = 0; i < PAGES; i++){
        pages[i] = -1;
    }

    for(int i = 0; i < ARRSIZE; i++){
        int t = i;
        if(i % 2){
            t = i*(-1);
        }
        numArray[i] = t;
    }
}

int FIFO::get(int i){
    for(int j = 0; j < PAGES; j++){
        if(i >= pages[j]*INTPERPAGE && i <= pages[j]*INTPERPAGE+INTPERPAGE-1){
            hits++;
            return numArray[i];
        }
    }
    int newPage;
    for(int j = 0; j < ARRSIZE; j+=INTPERPAGE){
        if(i >= j && i <= j+INTPERPAGE - 1){
            newPage = j / INTPERPAGE;
            break;
        }
    }
    /*printf("HMISS: "); printint(i); printf(" - ");
    for(int j = 0; j < PAGES; j++){
        printint(pages[j]); printf(" ");
    }
    printf("\n");*/
    int flag = 0;
    for(int i = PAGES-1; i > 0; i--){
        pages[i] = pages[i-1];
    }
    pages[0] = newPage;
    misses++;
    return numArray[i];
}

void FIFO::set(int i, int val){
    for(int j = 0; j < PAGES; j++){
        if(i >= pages[j]*INTPERPAGE && i <= pages[j]*INTPERPAGE+INTPERPAGE-1){
            hits++;
            numArray[i] = val;
            return;
        }
    }
    int newPage;
    for(int j = 0; j < ARRSIZE; j+=INTPERPAGE){
        if(i >= j && i <= j+INTPERPAGE - 1){
            newPage = j / INTPERPAGE;
            break;
        }
    }
    /*printf("SMISS: "); printint(i); printf(" - ");
    for(int j = 0; j < PAGES; j++){
        printint(pages[j]); printf(" ");
    }
    printf("\n");*/
    int flag = 0;
    for(int i = PAGES-1; i > 0; i--){
        pages[i] = pages[i-1];
    }
    pages[0] = newPage;
    misses++;
    numArray[i] = val;
}

#define REFS PAGES/2

class secondChance : public pageReplacement{
    private:
        int numArray[ARRSIZE];
        int pages[PAGES];
        int refs[REFS];
        int hits;
        int misses;

    public:
        secondChance();
        int get(int i);
        void set(int i, int val);
        int getHits(){return hits;}
        int getMisses(){return misses;}
        int getArrSize(){return ARRSIZE;}
};

secondChance::secondChance(){
    hits = -2;
    misses = 2;
    for(int i = 0; i < PAGES; i++){
        pages[i] = -1;
        refs[i] = -2;
    }
    for(int i = 0; i < ARRSIZE; i++){
        int t = i;
        if(i % 2){
            t = i*(-1);
        }
        numArray[i] = t;
    }
}

int secondChance::get(int i){
    for(int j = 0; j < PAGES; j++){
        if(i >= pages[j]*INTPERPAGE && i <= pages[j]*INTPERPAGE+INTPERPAGE-1){
            hits++;
            int t = refs[i];
            for(int i = REFS-1; i > 0; i--){
                if(refs[i-1] == t){
                    refs[i] = -2;
                }
                else{
                    refs[i] = refs[i-1];
                }
            }
            refs[0] = j;
            return numArray[i];
        }
    }
    int newPage;
    for(int j = 0; j < ARRSIZE; j+=INTPERPAGE){
        if(i >= j && i <= j+INTPERPAGE - 1){
            newPage = j / INTPERPAGE;
            break;
        }
    }
    /*printf("HMISS: "); printint(i); printf(" - ");
    for(int j = 0; j < PAGES; j++){
        printint(pages[j]); printf(" ");
    }
    printf("\n");*/
    int flag;
    while(1){
        flag = 1;
        for(int i = 0; i < REFS; i++){
            if(pages[PAGES-1] == refs[i]){
                int t = pages[PAGES - 1];
                refs[i] = -2;
                for(int j = PAGES-1; j > 0; j--){
                    pages[j] = pages[j-1];
                }
                pages[0] = t;
                flag = 0;
                break;
            }
        }
        if(flag){
            break;
        }
    }
    for(int j = PAGES-1; j > 0; j--){
        pages[j] = pages[j-1];
    }
    pages[0] = newPage;
    for(int j = REFS-1; j > 0; j--){
        refs[j] = refs[j-1];
    }
    refs[0] = newPage;
    misses++;
    return numArray[i];
}

void secondChance::set(int i, int val){
    for(int j = 0; j < PAGES; j++){
        if(i >= pages[j]*INTPERPAGE && i <= pages[j]*INTPERPAGE+INTPERPAGE-1){
            hits++;
            int t = refs[i];
            for(int i = REFS-1; i > 0; i--){
                if(refs[i-1] == t){
                    refs[i] = -2;
                }
                else{
                    refs[i] = refs[i-1];
                }
            }
            refs[0] = j;
            numArray[i] = val;
            return;
        }
    }
    int newPage;
    for(int j = 0; j < ARRSIZE; j+=INTPERPAGE){
        if(i >= j && i <= j+INTPERPAGE - 1){
            newPage = j / INTPERPAGE;
            break;
        }
    }
    /*printf("SMISS: "); printint(i); printf(" - ");
    for(int j = 0; j < PAGES; j++){
        printint(pages[j]); printf(" ");
    }
    printf("*");
    for(int j = 0; j < REFS; j++){
        printint(refs[j]); printf(" ");
    }
    printf("\n");*/
    int flag;
    while(1){
        flag = 1;
        for(int i = 0; i < REFS; i++){
            if(pages[PAGES-1] == refs[i]){
                int t = pages[PAGES - 1];
                refs[i] = -2;
                for(int j = PAGES-1; j > 0; j--){
                    pages[j] = pages[j-1];
                }
                pages[0] = t;
                flag = 0;
                break;
            }
        }
        if(flag){
            break;
        }
    }
    for(int j = PAGES-1; j > 0; j--){
        pages[j] = pages[j-1];
    }
    pages[0] = newPage;
    for(int j = REFS-1; j > 0; j--){
        refs[j] = refs[j-1];
    }
    refs[0] = newPage;
    misses++;
    numArray[i] = val;
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Hello World! --- http://www.AlgorithMan.de\n");

    GlobalDescriptorTable gdt;
    
    
    uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
    size_t heap = 10*1024*1024;
    MemoryManager memoryManager(heap, (*memupper)*1024 - heap - 10*1024);
    
    printf("heap: 0x");
    printfHex((heap >> 24) & 0xFF);
    printfHex((heap >> 16) & 0xFF);
    printfHex((heap >> 8 ) & 0xFF);
    printfHex((heap      ) & 0xFF);
    
    void* allocated = memoryManager.malloc(1024);
    printf("\nallocated: 0x");
    printfHex(((size_t)allocated >> 24) & 0xFF);
    printfHex(((size_t)allocated >> 16) & 0xFF);
    printfHex(((size_t)allocated >> 8 ) & 0xFF);
    printfHex(((size_t)allocated      ) & 0xFF);
    printf("\n");
    
    TaskManager taskManager;
    /*
    Task task1(&gdt, taskA);
    Task task2(&gdt, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);
    */
    
    InterruptManager interrupts(0x20, &gdt, &taskManager);
    
    printf("Initializing Hardware, Stage 1\n");
    
    #ifdef GRAPHICSMODE
        Desktop desktop(320,200, 0x00,0x00,0xA8);
    #endif
    
    DriverManager drvManager;
    
        #ifdef GRAPHICSMODE
            KeyboardDriver keyboard(&interrupts, &desktop);
        #else
            PrintfKeyboardEventHandler kbhandler;
            KeyboardDriver keyboard(&interrupts, &kbhandler);
        #endif
        drvManager.AddDriver(&keyboard);
        
    
        #ifdef GRAPHICSMODE
            MouseDriver mouse(&interrupts, &desktop);
        #else
            MouseToConsole mousehandler;
            MouseDriver mouse(&interrupts, &mousehandler);
        #endif
        drvManager.AddDriver(&mouse);
        
        PeripheralComponentInterconnectController PCIController;
        PCIController.SelectDrivers(&drvManager, &interrupts);

        VideoGraphicsArray vga;
        
    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();
        
    printf("Initializing Hardware, Stage 3\n");

    #ifdef GRAPHICSMODE
        vga.SetMode(320,200,8);
        Window win1(&desktop, 10,10,20,20, 0xA8,0x00,0x00);
        desktop.AddChild(&win1);
        Window win2(&desktop, 40,15,30,30, 0x00,0xA8,0x00);
        desktop.AddChild(&win2);
    #endif


    interrupts.Activate();

    printf("\n\n\n\n\n\n\n\n");

    printf("---Least Recently Used---");
    printf("\n-Bubble sort-: ");
    LRU* lru = new LRU();
    bubbleSort(lru, lru->getArrSize());
    printf("Hits: ");printint(lru->getHits());printf(" - ");
    printf("Misses: ");printint(lru->getMisses());printf("\n");


    printf("\n-Quick sort-: ");
    lru = new LRU();
    quickSort(lru, 0, lru->getArrSize()-1);
    printf("Hits: ");printint(lru->getHits());printf(" - ");
    printf("Misses: ");printint(lru->getMisses());printf("\n");


    printf("\n-Insertion sort-: ");
    lru = new LRU();
    insertionSort(lru, lru->getArrSize());
    printf("Hits: ");printint(lru->getHits());printf(" - ");
    printf("Misses: ");printint(lru->getMisses());printf("\n");

    printf("\n---FIFO---");
    printf("\n-Bubble sort-: ");
    FIFO* fifo = new FIFO();
    bubbleSort(fifo, fifo->getArrSize());
    printf("Hits: ");printint(fifo->getHits());printf(" - ");
    printf("Misses: ");printint(fifo->getMisses());printf("\n");


    printf("\n-Quick sort-: ");
    fifo = new FIFO();
    quickSort(fifo, 0, fifo->getArrSize()-1);
    printf("Hits: ");printint(fifo->getHits());printf(" - ");
    printf("Misses: ");printint(fifo->getMisses());printf("\n");


    printf("\n-Insertion sort-: ");
    fifo = new FIFO();
    insertionSort(fifo, fifo->getArrSize());
    printf("Hits: ");printint(fifo->getHits());printf(" - ");
    printf("Misses: ");printint(fifo->getMisses());printf("\n");


     printf("\n---Second Chance---");
    printf("\n-Bubble sort-: ");
    secondChance* sc = new secondChance();
    bubbleSort(sc, sc->getArrSize());
    printf("Hits: ");printint(sc->getHits());printf(" - ");
    printf("Misses: ");printint(sc->getMisses());printf("\n");


    printf("\n-Quick sort-: ");
    sc = new secondChance();
    quickSort(sc, 0, sc->getArrSize()-1);
    printf("Hits: ");printint(sc->getHits());printf(" - ");
    printf("Misses: ");printint(sc->getMisses());printf("\n");


    printf("\n-Insertion sort-: ");
    sc = new secondChance();
    insertionSort(sc, sc->getArrSize());
    printf("Hits: ");printint(sc->getHits());printf(" - ");
    printf("Misses: ");printint(sc->getMisses());printf("\n");
    
    

    
    /*for(int i = 0; i < lru->getArrSize(); i++){
        printint(lru->get(i));
        printf(" ");
    }*/
    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}

