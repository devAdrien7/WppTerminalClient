#include "conversationrepository.h"

ConversationRepository::ConversationRepository()
{
    dbConn = DatabaseConnection::getInstance();
}

ConversationRepository::~ConversationRepository() {}

ConversationRepository *ConversationRepository::getInstance()
{
    if(INSTANCE == nullptr){
        INSTANCE = new ConversationRepository();
    }

    return INSTANCE;
}

void ConversationRepository::killInstance()
{
    delete INSTANCE;
}

Conversation *ConversationRepository::getConversationById(const std::string &id)
{
    std::map<std::string, std::string> map = { {"id", id} };
    std::vector<Conversation*> conversations = dbConn->getSearch<Conversation>("conversations", map);

    if(conversations.empty())
        return nullptr;

    return conversations[0];
}

std::vector<Conversation *> ConversationRepository::getAllConversations()
{
    return dbConn->getAll<Conversation>("conversations");
}

void ConversationRepository::insertNewConversation(Conversation *conversation)
{
    dbConn->insert(conversation);
}

void ConversationRepository::bashInsertNewConversations(std::vector<Conversation *> conversations)
{
    std::vector<Entity*> entities;
    for(const auto& conversation : conversations){
        entities.push_back(conversation);
    }

    dbConn->bashInsert(entities);
}

void ConversationRepository::updateConversation(const std::string &id, Conversation *conversation)
{
    std::map<std::string, std::string> map = { {"id", id} };
    dbConn->update(conversation, map);
}

void ConversationRepository::deleteConversation(Conversation *conversation)
{
    dbConn->deleteEntity(conversation);
    delete conversation;
}
