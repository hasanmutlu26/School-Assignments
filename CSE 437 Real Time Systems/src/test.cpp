#include <iostream>
#include "Timer.h"
#include <string>

using CLOCK = std::chrono::high_resolution_clock;
using TTimerCallback = std::function<void()>;
static CLOCK::time_point T0;
void logCallback(int id, const std::string &logstr)
{
    auto dt = CLOCK::now() - T0;
    std::cout << "[" << std::chrono::duration_cast<std::chrono::milliseconds>(dt).count()
              << "] (cb " << id << "): " << logstr << std::endl;
}


int main()
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    T0 = CLOCK::now();
    logCallback(-1, "main starting.");
    auto t1 = CLOCK::now() + std::chrono::seconds(1);
    auto t2 = t1 + std::chrono::seconds(1);
    timer.registerTimer(t2, [&]()
                        { logCallback(1, "callback str"); });
    timer.registerTimer(t1, [&]()
                        { logCallback(2, "callback str"); });
    timer.registerTimer(Millisecs(700), [&]()
                        { logCallback(3, "callback str"); });
    timer.registerTimer(t1 + Millisecs(300), Millisecs(500), [&]()
                        { logCallback(4, "callback str"); });
    timer.registerTimer([&]()
                        {
 static int count = 0; 
 return ++count < 3;},
                        Millisecs(500), [&]()
                        { logCallback(5, "callback str"); });
    std::this_thread::sleep_for(std::chrono::seconds(5));
    logCallback(-1, "main terminating.");
}


/*
Timer::thread function starting...
[0] (cb -1): main starting.
[512] (cb 4): callback str 
[514] (cb 5): callback str
[715] (cb 3): callback str
[1009] (cb 2): callback str
[1012] (cb 4): callback str
[1014] (cb 5): callback str
[1413] (cb 3): callback str
[1508] (cb 5): callback str
[2008] (cb 1): callback str
[2116] (cb 3): callback str
[2815] (cb 3): callback str
[3511] (cb 3): callback str
[4207] (cb 3): callback str
[4984] (cb 3): callback str
[5012] (cb -1): main terminating. Timer::thread function terminating...
*/