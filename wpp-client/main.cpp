#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <sys/prctl.h>
#include <signal.h>
#include "service/wppconnectorservice.h"

WppConnectorService *service = new WppConnectorService();
volatile bool running = true;

void handler(int signal) {
    running = false;
}

int main() {
    std::signal(SIGINT, handler);
    pid_t pid = fork();

    if (pid == 0) {
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        if (getppid() == 1) {
            exit(1);
        }
        execlp("node", "node", "/home/islaifer/Documentos/Projects/WppTerminal/wpp-socket/index.js", NULL);

        perror("Error when starting node...");
    } else if (pid > 0) {
        std::cout << "Node PID: " << pid << std::endl;
        sleep(1);
        service->startWppCommunication();
        while (running) {
            std::cout << "Running..." << std::endl;
            sleep(5);
        }

        service->kill();
        std::cout << "Wpp Service Dead..." << std::endl;
        std::cout << "Closing program..." << std::endl;
    } else {
        perror("fork failed");
    }


    return 0;
}
