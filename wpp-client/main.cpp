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

        int opc = 0;
        std::string number;
        std::string message;
        while (running && opc != 9) {
            std::cout << "Running..." << std::endl;
            std::cout << "1- Mandar Mensagem" << std::endl;
            std::cout << "2- Sair da Sessão wpp" << std::endl;
            std::cout << "9- Sair" << std::endl;
            std::cout << "Qual opc: ";
            std::cin >> opc;
            switch(opc){
                case 1:
                    std::cout << "Digite o número da pessoa: ";
                    std::cin >> number;

                    std::cout << "Digite a mensagem: ";
                    std::cin >> message;

                    number += "@s.whatsapp.net";
                    service->sendTextMessage(number, message);
                    break;
                case 2:
                    service->sendCommand("logout");
                    break;
                case 9:
                    std::cout << "Saindo..." << std::endl;
                    break;
                default:
                    std::cerr << "Opc Inválida" << std::endl;
            }
        }

        service->kill();
        std::cout << "Wpp Service Dead..." << std::endl;
        std::cout << "Closing program..." << std::endl;
    } else {
        perror("fork failed");
    }


    return 0;
}
