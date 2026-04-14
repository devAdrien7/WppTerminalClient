#include "wppconnectorservice.h"
#include "../util/OBSERVABLE_COMMAND.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <qrencode.h>
#include <ncurses.h>

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

void WppConnectorService::readWpp()
{
    char buffer[1024];

    while (this->alive) {
        int n = read(this->sock, buffer, sizeof(buffer) - 1);
        if (n > 0) {
            buffer[n] = '\0';
            //std::cout << "Received: " << buffer << std::endl;

            std::string jsonContent(buffer);
            json j = json::parse(jsonContent);

            std::string type = j.at("type").get<std::string>();
            if(type == "qr"){
                std::string qrCode = j.at("data").get<std::string>();
                printQR(qrCode);
            }
        } else if (n == 0) {
            std::cout << "Connection Closed." << std::endl;
            break;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                continue;
            } else {
                perror("read");
                break;
            }
        }
    }

    std::cout << "Stop Reading" << std::endl;
}

void WppConnectorService::printQR(const std::string& text) {
    QRcode* qr = QRcode_encodeString(text.c_str(), 2, QR_ECLEVEL_L, QR_MODE_8, 1);

    std::string qrCode;
    for (int y = 0; y < qr->width; y += 2) {
        for (int x = 0; x < qr->width; x++) {

            unsigned char top = qr->data[y * qr->width + x];
            unsigned char bottom =
                (y + 1 < qr->width) ? qr->data[(y + 1) * qr->width + x] : 0;

            bool t = top & 1;
            bool b = bottom & 1;

            //if (t && b) printw("█");
            //else if (t) printw("▀");
            //else if (b) printw("▄");
            //else printw(" ");

            if (t && b) qrCode.append("█");
            else if (t) qrCode.append("▀");
            else if (b) qrCode.append("▄");
            else qrCode.append(" ");

            //if (t && b) std::cout << "█";
            //else if (t) std::cout << "▀";
            //else if (b) std::cout << "▄";
            //else std::cout << " ";
        }
        //std::cout << "\n";
        qrCode.append("\n");
    }
    //mvaddstr(4, 0, qrCode.c_str());
    //refresh();

    notify(OBSERVABLE_COMMAND::SHOW_QR, qrCode);

    QRcode_free(qr);
}

void WppConnectorService::sendCommand(const std::string& command)
{
    std::string send = "{\"type\":\"command\",\"instruction\":\"" + command + "\"}\n";
    write(sock, send.c_str(), send.size());
}

void WppConnectorService::sendTextMessage(const std::string& jid, const std::string& message)
{
    std::string send = "{\"type\":\"send\",\"jid\":\"" + jid + "\",\"text\":\"" + message + "\"}\n";
    write(sock, send.c_str(), send.size());
}