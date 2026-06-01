#include "wppconnectorservice.h"
#include "../util/OBSERVABLE_COMMAND.h"
#include "../model/entity/conversation.h"
#include "../model/entity/contact.h"
#include "../model/entity/message.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <qrencode.h>
#include <ncurses.h>

WppConnectorService::WppConnectorService() {}

WppConnectorService::~WppConnectorService() {}

WppConnectorService *WppConnectorService::getInstance()
{
    if(INSTANCE == nullptr){
        INSTANCE = new WppConnectorService();
    }

    return INSTANCE;
}

void WppConnectorService::killInstance()
{
    if(INSTANCE != nullptr){
        INSTANCE->kill();
        delete INSTANCE;
    }
}


void WppConnectorService::startWppCommunication()
{
    registerCommands();
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
    if(alive){
        alive = false;
        for(std::thread& thread : this->threads){
            thread.join();
        }

        close(sock);
    }
}

void WppConnectorService::readWpp()
{
    std::string buffer;

    char temp[1024];


    while (this->alive) {
        int n = read(this->sock, temp, sizeof(temp) - 1);

        if (n > 0) {
            buffer.append(temp, n);

            size_t pos;
            while ((pos = buffer.find('\n')) != std::string::npos) {
                std::string message = buffer.substr(0, pos);
                buffer.erase(0, pos + 1);

                try {
                    json j = json::parse(message);
                    std::string type = j.at("type");

                    auto command = commands.find(type);
                    if(command != commands.end()){
                        command->second(j);
                    }

                } catch (const std::exception& e) {
                    std::cerr << "JSON inválido: " << e.what() << std::endl;
                }
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

            if (t && b) qrCode.append("█");
            else if (t) qrCode.append("▀");
            else if (b) qrCode.append("▄");
            else qrCode.append(" ");
        }
        qrCode.append("\n");
    }
    //mvaddstr(4, 0, qrCode.c_str());
    //refresh();

    notify(OBSERVABLE_COMMAND::SHOW_QR, qrCode);

    QRcode_free(qr);
}

void WppConnectorService::registerCommands()
{
    commands["qr"] = [this](json arg){
        printQR(arg.at("data"));
    };

    commands["login"] = [this](json arg){
        if (arg.at("data") == "SUCCESS") {
            notify(OBSERVABLE_COMMAND::LOGIN_SUCCESS, "");
        }
    };

    commands["chats"] = [this](json arg){
        std::vector<Conversation*> conversations;

        for(json& chat : arg.at("data")){
            std::vector<Contact*> conversationMembers;

            std::string chatId = chat.at("id").get<std::string>();
            bool isArchived = chat.at("isArchived").get<bool>();
            std::vector<std::string> members = chat.at("participants").get<std::vector<std::string>>();
            bool isGroup = chat.at("isGroup").get<bool>();

            for(auto & member : members){
                conversationMembers.push_back(new Contact(member));
            }

            conversations.push_back(new Conversation(chatId, isArchived, isGroup, conversationMembers));
        }

        notify(OBSERVABLE_COMMAND::CHAT_RECEIVED, conversations);
    };

    commands["historic_messages"] = [this](json arg){
        std::vector<Message*> historicMessages;

        for(json& historicMessage : arg.at("data")){
            std::string id = historicMessage.at("id").get<std::string>();

            std::string typeJson = historicMessage.at("type").get<std::string>();
            MESSAGE_TYPE type = getStringToMessageType().at(typeJson);

            std::string messageText = historicMessage.at("message").get<std::string>();

            if(type == MESSAGE_TYPE::VIDEO && !messageText.empty())
                type = MESSAGE_TYPE::TEXT_AND_VIDEO;
            else if(type == MESSAGE_TYPE::IMAGE && !messageText.empty())
                type = MESSAGE_TYPE::TEXT_AND_IMAGE;

            std::string messageAudioVisual = "";
            time_t date = historicMessage.at("messageTimestamp").get<time_t>();
            bool read = historicMessage.at("read").get<bool>();
            bool authority = historicMessage.at("fromMe").get<bool>();
            std::string contactId = historicMessage.at("contactId").get<std::string>();
            std::string conversationId = historicMessage.at("conversationId").get<std::string>();

            Message* message = new Message(id, type, messageText, messageAudioVisual, date, read, authority, contactId, conversationId);
            historicMessages.push_back(message);
        }

        notify(OBSERVABLE_COMMAND::HISTORICAL_MESSAGE_RECEIVED, historicMessages);
    };

    commands["contacts"] = [this](json arg){
        std::vector<Contact*> contacts;

        for(json& contact: arg.at("data")){
            std::string id = contact.at("id").get<std::string>();
            std::string name = contact.at("name").get<std::string>();

            Contact* contactEntity = new Contact(id, name);
            contacts.push_back(contactEntity);
        }

        notify(OBSERVABLE_COMMAND::CONTACT_RECEIVED, contacts);
    };

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