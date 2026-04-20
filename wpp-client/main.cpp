#include "configuration/signalmanager.h"
#include "core/application.h"

Application* app;

int main() {
    SignalManager::init();
    app = new Application();
    app->run();
    delete app;
    return 0;
}
