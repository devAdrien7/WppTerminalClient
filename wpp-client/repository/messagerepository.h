#ifndef MESSAGEREPOSITORY_H
#define MESSAGEREPOSITORY_H

#include "../model/entity/message.h"
#include "../configuration/databaseconnection.h"

class MessageRepository
{
private:
    inline static MessageRepository* INSTANCE = nullptr;

    DatabaseConnection* dbConn;

    MessageRepository();
    ~MessageRepository();
public:
    static MessageRepository* getInstance();
    static void killInstance();

    Message* getMessageById(const std::string& id);
    std::vector<Message*> getAllMessages();


    void insertNewMessage(Message* message);
    void bashInsertNewMessages(std::vector<Message*> messages);
    void updateMessage(const std::string& id, Message* message);

    void deleteMessage(Message* message);
};

#endif // MESSAGEREPOSITORY_H
