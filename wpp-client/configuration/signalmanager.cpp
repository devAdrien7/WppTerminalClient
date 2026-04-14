#include "signalmanager.h"
#include <csignal>
#include <thread>

volatile std::atomic<bool> SignalManager::running = true;

void SignalManager::init()
{
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);

    pthread_sigmask(SIG_BLOCK, &set, nullptr);

    std::thread([set]() mutable {
        int sig;
        sigwait(&set, &sig);

        running = false;
        for(auto& value : SignalManager::runningsCv){
            value->notify_all();
        }
    }).detach();
}

bool SignalManager::isRunning() {
    return running;
}

void SignalManager::registerRuningCv(std::condition_variable* cv)
{
    runningsCv.push_back(cv);
}
