#include <multitasking.h>

using namespace myos;
using namespace myos::common;

void printf2(char* str)
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

void printfHex2(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf2(foo);
}


/*
        //////// THREAD CLASS METHODS \\\\\\\
*/
int Thread::static_id = 0;  

Thread::Thread(){
    id = static_id++;
    to_yield = -1;
    join = -1;
}

Thread::Thread(void(*_func)(void*)){

}

Thread::Thread(GlobalDescriptorTable* gdt, void(*_func)(void*)){

    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)_func;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;

    id = static_id++;
    to_yield = -1;
    join = -1;
}

Thread::~Thread(){

}


void Thread::setCPUState(CPUState* cpustate){
    this->cpustate = cpustate;
}

bool Thread::set_cpu_state(GlobalDescriptorTable* gdt, void(*f)()){

    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)f;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;

    return true;
}
      
void Thread::setState(int _status){
    status = _status;
}

void Thread::setPC(int _PC){
    PC = _PC;
}

void Thread::setJoin(int _join){
    join = _join;
}

void Thread::setYield(int yield){
    to_yield = yield;
}

int Thread::getJoin(){
    return join;
}

int Thread::getYield(){
    return to_yield;
}

int Thread::getid(){
    return id;
}



/*
        //////// THREAD MANAGER CLASS METHODS \\\\\\\
*/

ThreadManager::ThreadManager(){

}

ThreadManager::ThreadManager(Thread* thread){
    addThread(thread);
    current_thread = -1;
}

ThreadManager::~ThreadManager(){

}


Thread* ThreadManager::getThread(int x){
    
    if (x > num_threads)
        return nullptr;

    return threads[x];
}

Thread* ThreadManager::getCurrentThread(){
    return threads[current_thread];
}

CPUState* ThreadManager::getCurrentCPUState(){
    return threads[current_thread]->getCPUState();
}


void ThreadManager::setCurrentThread(int _current){
    current_thread = _current;
}

void ThreadManager::setThread(int num, Thread* thread){
    threads[num] = thread;
}

bool ThreadManager::addThread(Thread* thread){
    if(num_threads >= 256)
        return false;
        
    threads[num_threads++] = thread;
    return true;
}

int ThreadManager::getThread_id_index(int id){
    for(int i = 0; i < num_threads; i++){
        if(threads[i]->getid() == id)
            return i;
    }
    return -1;
}

Thread* ThreadManager::getThread_id(int id){
    for(int i = 0; i < num_threads; i++){
        if(threads[i]->getid() == id)
            return threads[i];
    }
    return nullptr;
}

void ThreadManager::delete_thread(Thread* thread){
    int index = -1;
    for(int i=0; i<num_threads; ++i){
        if(thread->getid() == threads[i]->getid()){
            while(i<num_threads-1){
                threads[i] = threads[i+1];
                i++;
            }
            threads[i] = nullptr;
        }
    }
    num_threads--;
}


CPUState* ThreadManager::Schedule(CPUState* cpustate){
    
    if(num_threads<=0)
        return cpustate;

    if(current_thread<num_threads-1)
        current_thread++;
    else
        current_thread = 0;

    /*
        If a thread yield by another thread giving it its turn
    */
    if( threads[current_thread]->getYield() != -1){
        // printf2("curr :"); 
        // printfHex2(current_thread);

        int yield = threads[current_thread]->getYield();
        // printf2("yield:"); 
        // printfHex2(yield);
        int index = getThread_id_index(yield);
        
        // printf2("index:"); 
        // printfHex2(index);
        threads[current_thread]->setYield(-1);
        
        if(index != -1)
            current_thread = index;
        // printf2("send:"); 
    }


    /*
        If a thread join by another thread giving it its turn until it is done
    */
    if(threads[current_thread]->getJoin() != -1){
        int join = threads[current_thread]->getJoin();
        int index = getThread_id_index(join);
        
        if(index == -1)
            threads[current_thread]->setJoin(-1);
        else
            current_thread = index;    
    }
    // printfHex2(current_thread);
    
    return threads[current_thread]->getCPUState();
}