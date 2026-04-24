#include "../util/Observable.h"
#include <string>
#include <vector>
#include <thread>
#include <nlohmann/json.hpp>

#ifndef WPPCONNECTORSERVICE_H
#define WPPCONNECTORSERVICE_H

using json = nlohmann::json;

class WppConnectorService : public Observable
{
private:
    inline static WppConnectorService* INSTANCE;

    WppConnectorService();
    ~WppConnectorService();

    volatile bool alive = true;
    std::vector<std::thread> threads;
    int sock;

    void readWpp();
    void printQR(const std::string& text);
public:
    static WppConnectorService* getInstance();
    static void killInstance();

    void startWppCommunication();
    void kill();
    void sendTextMessage(const std::string& jid, const std::string& message);
    void sendCommand(const std::string& command);
};

#endif // WPPCONNECTORSERVICE_H
