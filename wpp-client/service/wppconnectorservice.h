#include <string>
#include <vector>
#include <thread>

#ifndef WPPCONNECTORSERVICE_H
#define WPPCONNECTORSERVICE_H

class WppConnectorService
{
private:
    volatile bool alive = true;
    std::vector<std::thread> threads;
    int sock;

    void createNodeProccess();
    void readWpp();
    void sendCommand(std::string command);
public:
    WppConnectorService();
    ~WppConnectorService();

    void startWppCommunication();
    void kill();
};

#endif // WPPCONNECTORSERVICE_H
