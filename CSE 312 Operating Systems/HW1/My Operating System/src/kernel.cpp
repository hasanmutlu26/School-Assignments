/**
 * My implementations start at line 130. One small note before that:
 * GlobalDescriptorTable gdt and TaskManager taskManager variables are carried out of main
 * function to the global scope.
 */

#include <common/types.h>
#include <gdt.h>
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


using namespace myos;
using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;
using namespace myos::gui;

GlobalDescriptorTable gdt;
TaskManager taskManager;

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


/**
 * This is a helper function to delay threads executions. It is simply a long for loop.
 * @param dur Magnitude of the sleep amount.
 */
void sleep(int dur){
    for(int i = 0; i < dur; i++){
        for(unsigned int j = 0; j < 429496829; j++){
            //printf("Sleep.");
        }
    }
}

//Indicators that either scenarios are either running or finished. It will be used to make 
//main task wait for scenarios to end. They will be incremented after 
//threads end and when they become 2, mainTask() will continue.
int firstScenario = 0;
int secondScenario = 0;

//The shared variable between threads C and D.
int shared1 = 3;

/**
 * This is threadC. It is supposed to increment shared1's value but its increment will be lost 
 * due to race condition.
 */
void threadC()
{
    int x;      //shared1's initial value is stored in local variable x.
    x = shared1;
    
    //Initial value of shared1 is printed.
    printf("Thread C reads the value of shared variable as: ");
    for(int i = 0; i < shared1; i++){
        printf("*");
    }
    printf("\n");
    
    //Local variable x is incremented. It will be assigned back to shared1 but it will not have an effect.
    x++;    
    
    //Value of local variable after incrementation is printed.
    printf("Local increment by Thread C: ");
    for(int i = 0; i < x; i++){
        printf("*");
    }
    printf("\n");
    
    //Sleep is called. Thread D will start doing its own job during this sleep. 
    sleep(1);

    //Value of local variable is assigned back to shared1. This value will be overwritten by Thread D.
    shared1 = x;
    
    //Value of shared variable after Thread C
    printf("Value of shared variable updated by Thread C is: ");
    for(int i = 0; i < shared1; i++){
        printf("*");
    }
    printf("\n");
    firstScenario++;
    while(true){
        //printf("threadC");
    }
}

/**
 * This is threadD. It will decrement the value of shared1. It will decrement the value before threadC
 * finishes its incrementation.
 */
void threadD()
{
    //shared1's initial value is stored in local variable. 
    //This assignment is done before threadC assigns shared1's new value.
    int x;
    x = shared1;
    
    //Initial value of shared1 is printed.
    printf("Thread D reads the value of shared variable as: ");
    for(int i = 0; i < shared1; i++){
        printf("*");
    }
    printf("\n");
    
    //Local variable x is decremented. It will be assigned back to shared1.
    x--;
    
    //Value of local variable is printed.
    printf("Local decrement by Thread D: ");
    for(int i = 0; i < x; i++){
        printf("*");
    }
    printf("\n");
    
    //Sleep is called. threadC will finish its job during this sleep but it won't have an effect.
    sleep(1);

    //Local variable is assigned to shared1 and overwritten the result of threadC.
    shared1 = x;
    
    //Final value of shared1 is printed.
    printf("Value of shared variable updated by Thread D is: ");
    for(int i = 0; i < shared1; i++){
        printf("*");
    }
    printf("\n");
    firstScenario++;
    while(true){
        //printf("threadD");
    }
}

//Shared variable between threads A and B.
int shared2 = 3;

//Array of flags for Peterson's Algorithm. Its first two values will be used only.
bool flag[10] = {false};

//Turn indicator for Peterson's Algorithm.
int turn = 0;

/**
 * This is threadA. It increments shared2's value and provides required safety
 * with Peterson's Algorithm.
 */
void threadA()
{
    int x;  //Local variable.
    
    //Flags and indicators are set according to Peterson's algorithm. Critical region starts.
    flag[0] = true;
    turn = 1;
    while(flag[1] && turn == 1);
    
    //shared2 is assigned to x
    x = shared2;

    //Initial value of shared2 is printed.
    printf("Thread A reads the value of shared variable as: ");
    for(int i = 0; i < shared2; i++){
        printf("*");
    }
    printf("\n");
    
    //local variable is incremented.
    x++;
    
    //Local variable's value is printed.
    printf("Local increment by Thread A: ");
    for(int i = 0; i < x; i++){
        printf("*");
    }
    printf("\n");
    
    //Sleep is called. threadB will be stuck at Peterson Algorithm's while loop during this
    //sleep and will not cause any trouble.
    sleep(1);
    
    //Local variable is assigned to shared2.
    shared2 = x;

    //Critical region is ended.
    flag[0] = false;
    
    //Value of shared2 is printed
    printf("Value of shared variable updated by Thread A is: ");
    for(int i = 0; i < shared2; i++){
        printf("*");
    }
    printf("\n");
    secondScenario++;
    while(true){
        //printf("threadA");
    }
}

/**
 * This is threadB. It decrements shared2's value after threadA finishes its job. It 
 * provides safety with Peterson's Algorithm.
 */
void threadB()
{
    int x;  //Local variable is created.
    
    //Flags and indicators are set according to Peterson's algorithm. Critical region starts.
    //threadA is waited to finish its job.
    flag[1] = true;
    turn = 0;
    while(flag[0] && turn == 0);
    
    //Incoming value of shared2 is assigned to local variable.
    x = shared2;
    
    //Initial value of shared2 is printed. It should be the incremented value.
    printf("Thread B reads the value of shared variable as: ");
    for(int i = 0; i < shared2; i++){
        printf("*");
    }
    printf("\n");
    
    //Local variable decremented.
    x--;
    
    //Local variable is printed.
    printf("Local decrement by Thread B: ");
    for(int i = 0; i < x; i++){
        printf("*");
    }
    printf("\n");
    
    //Sleep is called. threadA might only print something to screen during this sleep.
    sleep(1);
    
    //Local variable is assigned to shared2 and critical region is ended.
    shared2 = x;
    flag[1] = false;
    
    //Value of shared2 is printed. It must be the same as initial value.
    printf("Value of shared variable updated by Thread B is: ");
    for(int i = 0; i < shared2; i++){
        printf("*");
    }
    printf("\n");
    secondScenario++;
    while(true){
        //printf("threadB");
    }
}


//Tasks that threads will be created into.
Task task1;
Task task2;
Task task3;
Task task4;

/**
 * mainTask() start and print result of two scenarios one by one.
 */
void mainTask(){
    printf("Initial value of shared variable is: ");
    for(int i = 0; i < shared2; i++){
        printf("*");
    }
    printf("\n\n");
    
    //Tasks are created.
    task1 = Task(&gdt, threadA);
    task2 = Task(&gdt, threadB);
    task3 = Task(&gdt, threadC);
    task4 = Task(&gdt, threadD);
    
    //First scenario, which is without Peterson's Algorithm starts.
    printf("--Starting scenario without Peterson's Solution.--\n");
    taskManager.AddTask(&task3);
    taskManager.AddTask(&task4);
    
    //Wait for threads C and D to finish.
    while(firstScenario != 2);
    
    //Print the final value of shared variable in first scenario.
    printf("Final value of shared variable without Peterson's solution: ");
    for(int i = 0; i < shared1; i++){
        printf("*");
    }
    printf("\n\n");
    
    //Second scenario, which provides safety with Peterson's Algorithm starts.
    printf("--Starting scenario with Peterson's Solution.--\n");
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);

    //Wait for threads A and B to finish.
    while(secondScenario != 2);
    
    //Print the final value of shared variable in second scenario.
    printf("Final value of shared variable with Peterson's solution: ");
    for(int i = 0; i < shared2; i++){
        printf("*");
    }
    printf("\n");
    
    while(true){
        //printf("Main");
    }
}





typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}


extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Hello World! --- http://www.AlgorithMan.de\n");
    //Task task1(&gdt, threadA);
    //Task task2(&gdt, threadB);
    //taskManager.AddTask(&task1);
    //taskManager.AddTask(&task2);

    //MainTask is created and called.
    Task task(&gdt, mainTask);
    taskManager.AddTask(&task);
    
    
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

    printf("\n\n\n\n\n\n\n\n\n\n");
    interrupts.Activate();
    
    while(1)
    {
        #ifdef GRAPHICSMODE
            desktop.Draw(&vga);
        #endif
    }
}
