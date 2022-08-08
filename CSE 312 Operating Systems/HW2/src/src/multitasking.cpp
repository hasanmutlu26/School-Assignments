
#include <multitasking.h>

using namespace myos;
using namespace myos::common;

/*
        //////// TASK CLASS METHODS \\\\\\\
*/
Task::Task()
{}

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

Task::Task(GlobalDescriptorTable *gdt, Thread* thread){
    thread_manager->addThread(thread);
    cpustate = thread->getCPUState();
}

Task::Task(ThreadManager* _thread_manager){
    thread_manager = _thread_manager;
    num_threads = _thread_manager->getNumThread();
    cpustate = thread_manager->getThread(0)->getCPUState();
}

Task::~Task()
{}

Thread* Task::getCurrentThread(){
    return thread_manager->getCurrentThread();
}

void Task::delete_thread(Thread* thread){
    thread_manager->delete_thread(thread);
    num_threads--;    
}

void Task::addThread(Thread* thread){
    thread_manager->addThread(thread);
    num_threads++;
}

void Task::setThreadManager(ThreadManager* _thread_manager){
    thread_manager = _thread_manager;
}

CPUState* Task::Schedule(CPUState* cpustate){
    if(num_threads <= 0)
        return cpustate;
    
    int ct = thread_manager->getCurrentThreadNum();
    if(ct >= 0)
        cpustate = thread_manager->getThread(ct)->getCPUState();
    
    if(++ct >= num_threads)
        thread_manager->setCurrentThread(ct%=num_threads);

    return cpustate;
}



/*
        //////// TASK MANAGER CLASS METHODS \\\\\\\
*/
        
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

/* CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if(numTasks <= 0)
        return cpustate;
    
    if(currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;
    
    
    if(++currentTask >= numTasks)
        currentTask %= numTasks;

    return tasks[currentTask]->cpustate;
} */

CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if(numTasks <= 0)
        return cpustate;

    if(currentTask >= 0)
        tasks[currentTask]->getCurrentThread()->setCPUState(cpustate);

    if(++currentTask >= numTasks)
        currentTask %= numTasks;
    
    return tasks[currentTask]->thread_manager->Schedule(cpustate);
}

    