#include "wppconnectorservice.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>

WppConnectorService::WppConnectorService() {}

WppConnectorService::~WppConnectorService() {}


void WppConnectorService::startWppCommunication()
{
    this->sock = socket(AF_UNIX, SOCK_STREAM | SOCK_NONBLOCK, 0);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/tmp/wpp.sock");

    connect(sock, (sockaddr*)&addr, sizeof(addr));

    this->threads.emplace_back([this](){
        this->readWpp();
    });

    std::cout << "Node connection success! Start Wpp Communication!" << std::endl;

    this->sendCommand("start");
}

void WppConnectorService::kill()
{
    this->alive = false;
    for(std::thread& thread : this->threads){
        thread.join();
    }

    close(sock);
}

void WppConnectorService::createNodeProccess()
{

}

void WppConnectorService::readWpp()
{
    char buffer[1024];

    while (this->alive) {
        int n = read(this->sock, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        } else if (n == 0) {
            std::cout << "Connection Closed." << std::endl;
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // opcional
                continue;
            } else {
                perror("read");
                break;
            }
        }
    }

    std::cout << "Stop Reading" << std::endl;
}

void WppConnectorService::sendCommand(std::string command)
{
    std::string send = "{\"type\":\"command\",\"instruction\":\"" + command + "\"}\n";
    write(sock, send.c_str(), send.size());
}