 
#ifndef __MYOS__MULTITASKING_H
#define __MYOS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace myos
{
    
    struct CPUState
    {
        common::uint32_t eax;
        common::uint32_t ebx;
        common::uint32_t ecx;
        common::uint32_t edx;

        common::uint32_t esi;
        common::uint32_t edi;
        common::uint32_t ebp;

        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */
        common::uint32_t error;

        common::uint32_t eip;
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp;
        common::uint32_t ss;        
    } __attribute__((packed));
    
     
    class Thread{

        friend class ThreadManager;

        public:

            Thread();
            Thread(void(*_func)(void*));
            Thread(GlobalDescriptorTable* gdt, void(*_func)(void*));
            ~Thread();

            inline unsigned long int get(){return id;}

            bool set_cpu_state(GlobalDescriptorTable* gdt, void(*f)());
            void setCPUState(CPUState* cpustate);
            
            CPUState* getCPUState(){return cpustate;}
            

            void setState(int state);
            void setPC(int PC);
            void setJoin(int _join);
            void setYield(int yield);
            
            int getJoin();
            int getYield();
            int getid();

        private:

            static int static_id;
            int id;
            int status;
            int PC;
            int to_yield;
            int join;
            CPUState* cpustate;
            common::uint8_t stack[4096];

    };


    class ThreadManager{
        
        public:
            ThreadManager();
            ThreadManager(Thread* thread);
            ~ThreadManager();

            inline int getNumThread(){return num_threads;}
            inline int getCurrentThreadNum(){return current_thread;}
            
            Thread* getCurrentThread();
            CPUState* getCurrentCPUState();


            bool addThread(Thread* thread);
            void setThread(int num, Thread* thread);
            void setCurrentThread(int _current);
            Thread* getThread(int x);

            int getThread_id_index(int id);
            Thread* getThread_id(int id);

            void delete_thread(Thread* thread);

            CPUState* Schedule(CPUState* cpustate);
            
        private:
            Thread* threads[256];
            int num_threads;
            int current_thread;
    };


    class Task
    {
        friend class TaskManager;

        private:
            unsigned long int id;
            unsigned int size = 4096;
            int num_threads = 0;
            
            common::uint8_t* stack; // 4 KiB
            CPUState* cpustate;
            


        public:

            ThreadManager* thread_manager;
            Task();
            Task(GlobalDescriptorTable *gdt, void entrypoint());
            Task(GlobalDescriptorTable *gdt, Thread* thread);
            Task(ThreadManager* _thread_manager);

            Thread* getCurrentThread();

            void delete_thread(Thread* thread);

            void addThread(Thread* thread);

            void setThreadManager(ThreadManager* _thread_manager);

            CPUState* Schedule(CPUState* cpustate);

            ~Task();
    };


    class TaskManager
    {
        private:
            Task* tasks[256];
            int numTasks;
            int currentTask;

        public:
            TaskManager();
            ~TaskManager();

            bool AddTask(Task* task);
            
            CPUState* Schedule(CPUState* cpustate);
    };

}

#endif