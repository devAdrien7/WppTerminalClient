#ifndef SIGNALMANAGER_H
#define SIGNALMANAGER_H

#include <condition_variable>
#include <atomic>
#include <vector>

class SignalManager {
public:
    inline static std::vector<std::condition_variable*> runningsCv;
    static void init();
    static bool isRunning();
    static void registerRuningCv(std::condition_variable* cv);
    static volatile std::atomic<bool> running;
};

#endif // SIGNALMANAGER_H
