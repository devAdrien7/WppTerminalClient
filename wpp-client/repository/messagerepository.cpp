#include "messagerepository.h"

MessageRepository::MessageRepository()
{
    dbConn = DatabaseConnection::getInstance();
}

MessageRepository::~MessageRepository() {}

MessageRepository *MessageRepository::getInstance()
{
    if(INSTANCE == nullptr){
        INSTANCE = new MessageRepository();
    }

    return INSTANCE;
}

void MessageRepository::killInstance()
{
    delete INSTANCE;
}

Message *MessageRepository::getMessageById(const std::string &id)
{
    std::map<std::string, std::string> map = { {"id", id} };
    std::vector<Message*> messages = dbConn->getSearch<Message>("messages", map);

    if(messages.empty())
        return nullptr;

    return messages[0];
}

std::vector<Message *> MessageRepository::getAllMessages()
{
    return dbConn->getAll<Message>("messages");
}

void MessageRepository::insertNewMessage(Message *message)
{
    dbConn->insert(message);
}

void MessageRepository::bashInsertNewMessages(std::vector<Message *> messages)
{
    std::vector<Entity*> entities;
    for(const auto& message : messages){
        entities.push_back(message);
    }

    dbConn->bashInsert(entities);
}

void MessageRepository::updateMessage(const std::string &id, Message *message)
{
    std::map<std::string, std::string> map = { {"id", id} };
    dbConn->update(message, map);
}

void MessageRepository::deleteMessage(Message *message)
{
    dbConn->deleteEntity(message);
    delete message;
}


