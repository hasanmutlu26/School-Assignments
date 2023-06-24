#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include "ITimer.h"

using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
using Millisecs = std::chrono::milliseconds;
using Timepoint = CLOCK::time_point;
using TPredicate = std::function<bool()>;

class Timer : public ITimer
{
public:
    Timer()
    {
        isRunning = true;
        timerThread = std::thread(&Timer::timerThreadFunc, this);
    }

    ~Timer()
    {
        {
            std::lock_guard<std::mutex> lock(mutx);
            isRunning = false;
        }
        cond.notify_all();
        if (timerThread.joinable())
            timerThread.join();
    }

    void registerTimer(const Timepoint &tp, const TTimerCallback &cb) override
    {
        TimerTask task(tp, cb);
        std::lock_guard<std::mutex> lock(mutx);
        taskVec.push_back(task);
        cond.notify_all();
    }

    void registerTimer(const Millisecs &period, const TTimerCallback &cb) override
    {
        TimerTask task(period, cb);
        std::lock_guard<std::mutex> lock(mutx);
        taskVec.push_back(task);
        cond.notify_all();
    }

    void registerTimer(const Timepoint &tp, const Millisecs &period, const TTimerCallback &cb) override
    {
        TimerTask task(tp, period, cb);
        std::lock_guard<std::mutex> lock(mutx);
        taskVec.push_back(task);
        cond.notify_all();
    }

    void registerTimer(const TPredicate &pred, const Millisecs &period, const TTimerCallback &cb) override
    {
        TimerTask task(pred, period, cb);
        std::lock_guard<std::mutex> lock(mutx);
        taskVec.push_back(task);
        cond.notify_all();
    }

private:
    enum timerType
    {
        ONCE,
        PERIODIC_FRVR,
        PERIODIC_UNTIL,
        PERIODIC_PRED
    };
    struct TimerTask
    {
        Timepoint tp;
        Timepoint endTp;
        Millisecs period;
        TPredicate pred;
        TTimerCallback callback;
        timerType type;

        TimerTask(const Timepoint &tp, const TTimerCallback &cb)
        {
            this->tp = tp;
            callback = cb;
            type = ONCE;
        }

        TimerTask(const Millisecs &period, const TTimerCallback &cb)
        {
            this->tp = CLOCK::now() + period;
            this->period = period;
            callback = cb;
            type = PERIODIC_FRVR;
        }

        TimerTask(const Timepoint &tp, const Millisecs &period, const TTimerCallback &cb)
        {
            this->tp = CLOCK::now() + period;
            this->endTp = tp;
            this->period = period;
            callback = cb;
            type = PERIODIC_UNTIL;
        }

        TimerTask(const TPredicate &pred, const Millisecs &period, const TTimerCallback &cb)
        {
            this->tp = CLOCK::now() + period;
            this->period = period;
            this->pred = pred;
            callback = cb;
            type = PERIODIC_PRED;
        }
    };

    void timerThreadFunc()
    {
        std::unique_lock<std::mutex> lock(mutx);
        int nextTaskIndex;
        std::cout << "Timer::thread function starting..." << std::endl;
        while (isRunning)
        {
            if (taskVec.empty())
            {
                cond.wait(lock);
            }
            else
            {
                nextTaskIndex = getNextTaskIndex();
                if (nextTaskIndex == -1)
                {
                    continue;
                }

                if (taskVec[nextTaskIndex].tp > CLOCK::now())
                {
                    cond.wait_until(lock, taskVec[nextTaskIndex].tp);
                }

                if (taskVec[nextTaskIndex].tp <= CLOCK::now())
                {
                    taskVec[nextTaskIndex].callback();

                    if (taskVec[nextTaskIndex].type > ONCE)
                    {
                        if (taskVec[nextTaskIndex].type == PERIODIC_UNTIL && taskVec[nextTaskIndex].endTp <= CLOCK::now() + taskVec[nextTaskIndex].period)
                        {
                            taskVec.erase(taskVec.begin() + nextTaskIndex);
                        }
                        else if (taskVec[nextTaskIndex].type == PERIODIC_PRED && !(taskVec[nextTaskIndex].pred()))
                        {
                            taskVec.erase(taskVec.begin() + nextTaskIndex);
                        }
                        else
                        {
                            taskVec[nextTaskIndex].tp = CLOCK::now() + taskVec[nextTaskIndex].period;
                        }
                    }
                    else
                    {
                        taskVec.erase(taskVec.begin() + nextTaskIndex);
                    }
                }
            }
        }
    }

    int getNextTaskIndex()
    {
        if (taskVec.empty())
            return -1;

        std::size_t nextIndex = 0;
        Timepoint nextTime = taskVec[0].tp;
        for (std::size_t i = 1; i < taskVec.size(); ++i)
        {
            if (taskVec[i].tp < nextTime)
            {
                nextTime = taskVec[i].tp;
                nextIndex = i;
            }
        }

        return nextIndex;
    }

    std::atomic<bool> isRunning;
    std::vector<TimerTask> taskVec;
    std::mutex mutx;
    std::condition_variable cond;
    std::thread timerThread;
};
