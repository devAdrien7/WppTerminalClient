#ifndef CONVERSATIONREPOSITORY_H
#define CONVERSATIONREPOSITORY_H

#include "../model/entity/conversation.h"
#include "../configuration/databaseconnection.h"

class ConversationRepository
{
private:
    inline static ConversationRepository* INSTANCE = nullptr;

    DatabaseConnection* dbConn;

    ConversationRepository();
    ~ConversationRepository();
public:
    static ConversationRepository* getInstance();
    static void killInstance();

    Conversation* getConversationById(const std::string& id);
    std::vector<Conversation*> getAllConversations();


    void insertNewConversation(Conversation* conversation);
    void bashInsertNewConversations(std::vector<Conversation*> conversations);
    void updateConversation(const std::string& id, Conversation* conversation);

    void deleteConversation(Conversation* conversation);
};

#endif // CONVERSATIONREPOSITORY_H
