#include "configuration/signalmanager.h"
#include "core/application.h"

#include <csignal>

volatile sig_atomic_t running = 1;
Application* app;

int main() {
    SignalManager::init();
    app = new Application();
    app->run();
    delete app;
    return 0;
}
