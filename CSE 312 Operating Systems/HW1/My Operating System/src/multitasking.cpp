
#include <multitasking.h>

using namespace myos;
using namespace myos::common;


Task::Task(GlobalDescriptorTable *gdt, void entrypoint())
{
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
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;
    
}

Task::~Task()
{
}

Task::Task(){
}

Task::Task(const myos::Task& copy){
    cpustate = (CPUState*)(stack + 4096 - sizeof(CPUState));

    cpustate -> eax = copy.cpustate -> eax;
    cpustate -> ebx = copy.cpustate -> ebx;
    cpustate -> ecx = copy.cpustate -> ecx;
    cpustate -> edx = copy.cpustate -> edx;

    cpustate -> esi = copy.cpustate -> esi;
    cpustate -> edi = copy.cpustate -> edi;
    cpustate -> ebp = copy.cpustate -> ebp;
    
    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */
    
    // cpustate -> error = 0;    
   
    // cpustate -> esp = ;
    cpustate -> eip = copy.cpustate -> eip;
    cpustate -> cs = copy.cpustate -> cs;
    // cpustate -> ss = ;
    cpustate -> eflags = copy.cpustate -> eflags;
}
        
TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager()
{
}

bool TaskManager::AddTask(Task* task)
{
    if(numTasks >= 256)
        return false;
    tasks[numTasks++] = task;
    return true;
}
/*
bool TaskManager::AddTaskCopy(Task task){
    if(numTasks >= 256)
        return false;

    tasks[numTasks++] = new Task(task);
    return true;
}
*/

CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if(numTasks <= 0)
        return cpustate;
    
    if(currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;
    
    if(++currentTask >= numTasks)
        currentTask %= numTasks;
    return tasks[currentTask]->cpustate;
}
/*
int TaskManager::getCurrentTaskID(){
    return currentTask;
}

int TaskManager::getNumTasks(){
    return numTasks;
}

bool TaskManager::deleteTask(int taskID){
    for(int i = 0; i < numTasks - 1; i++){
        if(i + 1 == taskID){
            continue;
        }
        tasks[i] = tasks[i+1];
    }
    numTasks--;
}
*/
    